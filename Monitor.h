#ifndef __MONITOR__
#define __MONITOR__

#include "mylog.h"
#include "Info.h"
#include <vector>
#include <queue>
#include "Led.h"


class CMySocket;
class CMonitor
{
public:
	CMonitor(void);
	~CMonitor(void);
	void run();
	void wait();
	static enum color{red,green,blue};
public:
	CInfo *m_info;
	queue<string> m_queue;		  //保存待处理的文件
	vector<HANDLE> m_hThread;  //保存线程句柄
	HANDLE m_hTPrintInfo;		//打印线程句柄
	HANDLE m_hTSendMsg;		//发送通知线程句柄
	HANDLE m_hEvent;				//事件句柄		
	CMySocket *m_socket;
	CLed *m_led;
	
};

//只为传参
struct Arg{
	CMonitor *mt;	//保存this
	string path;	   //保存监听的目录名
};

//监听目录
DWORD CALLBACK listenDir(void *arg);
//每隔10秒打印一次正在运行信息
DWORD CALLBACK printInfo(void *arg);
//从队列中取出文件发送到服务器
DWORD CALLBACK sendMsg(void *arg);

#endif

