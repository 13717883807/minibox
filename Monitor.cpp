#include "Monitor.h"
#include "Led.h"
#include "MySocket.h"
#include <io.h>
#include "DealWith.h"

using namespace std;


CMonitor::CMonitor(void)
{
	//创建事件
	m_hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_info = new CInfo();
	m_led = new CLed(red);
	m_socket = new CMySocket(this);	   //连接服务器
	this->run();
}


CMonitor::~CMonitor(void)
{
	//等待所有线程结束
	wait();
	//关闭线程句柄
	CloseHandle(m_hEvent);
	CloseHandle(m_hTPrintInfo);
	CloseHandle(m_hTSendMsg);
}

void CMonitor::run()
{
	for(vector<string>::iterator i=m_info->m_listenDir.begin();i != m_info->m_listenDir.end();++i){
		Arg *arg = new Arg;
		arg->mt = this;
		arg->path =  *i;
		//创建多线程监听文件夹,句柄存到数组
		HANDLE hThread = CreateThread(NULL,0,listenDir,(void*)arg,0,NULL);
		m_hThread.push_back(hThread);
	}
	this->m_hTSendMsg = CreateThread(NULL,0,sendMsg,this,0,NULL);
	this->m_hTPrintInfo = CreateThread(NULL,0,printInfo,NULL,0,NULL);
}
void CMonitor::wait()
{
	// 等待所有线程返回
	for(unsigned int i=0;i < m_hThread.size();i++){
		WaitForSingleObject(m_hThread[i],INFINITE);
		CloseHandle(m_hThread[i]);
	}
}

static DWORD CALLBACK listenDir(void *arg)
{
	HANDLE hDir; 
	char notify[1024];
	DWORD cbBytes;
	Arg *pArg = (Arg*)arg;
	FILE_NOTIFY_INFORMATION *pnotify=(FILE_NOTIFY_INFORMATION *)notify;
	if(_access(pArg->path.c_str(),0) == -1) {
		mylog << "not found dir : " + pArg->path;
		return -1;
	}
	hDir = CreateFileA(pArg->path.c_str(), FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, 
		OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);
	if(INVALID_HANDLE_VALUE == hDir){ 										 
		return -1;
	}
	mylog << "start  listen : " +  pArg->path;
	while (true) 
	{ 	
		if(ReadDirectoryChangesW(hDir, &notify, sizeof(notify),
			FALSE, FILE_NOTIFY_CHANGE_FILE_NAME| FILE_NOTIFY_CHANGE_LAST_WRITE, 
			&cbBytes, NULL, NULL)) 
		{ 
			char file_name[MAX_PATH] = {0}; //设置文件名
			if(pnotify->FileName){
				WideCharToMultiByte(CP_ACP,0,pnotify->FileName,pnotify->FileNameLength/2,file_name,99,NULL,NULL);
			} 
			switch(pnotify->Action) 
			{ 
			case FILE_ACTION_ADDED:{
				mylog  << pArg->path + "：new file...";
				//保存新文件的路径
				pArg->mt->m_queue.push(pArg->path+'/'+file_name);
				//触发事件
				::SetEvent(pArg->mt->m_hEvent);
				break;
								   } 
			case FILE_ACTION_REMOVED:
				mylog  << pArg->path + "：remove file...";
				break; 
			case FILE_ACTION_MODIFIED:	
				//mylog  << pArg->path + "update file content\n";
				break; 
			case FILE_ACTION_RENAMED_OLD_NAME:
				mylog << pArg->path + "：update filename...";
				break; 
			case FILE_ACTION_RENAMED_NEW_NAME:
				mylog << pArg->path + "：update filename...";
				break; 
			default: 
				break; 
			}
		}
	}				   
	return NULL;
}

static DWORD CALLBACK printInfo(void *arg)
{
	while(true){
		Sleep(10000);
		mylog << "Process running....";
	}
}

static DWORD CALLBACK sendMsg(void *arg)
{
	CMonitor *pMt = (CMonitor*)arg; 
	while(true){
		::WaitForSingleObject(pMt->m_hEvent,INFINITE);  // 等待事件句柄有信号
		::ResetEvent(pMt->m_hEvent);
		while(!pMt->m_queue.empty()){
			//获取队头元素
			string path = pMt->m_queue.front();
			pMt->m_queue.pop();
			//拿到文件路径，做处理
			CDealWith deal(pMt,path);
			deal.run();
			Sleep(2000);
		}
	}
	return NULL;
}

