#include "Info.h"
#include "mylog.h"
#include <fstream>
#include <windows.h>
#include <io.h>
#include <direct.h>


CInfo::CInfo(void)
{
	m_port = 0;
	if(true == init())
		mylog << "Initialization successful";
	else{
		mylog << "Exit after 3 seconds";
		Sleep(3000);
		exit(-1);
	}
}


CInfo::~CInfo(void)
{
}

//�����Ƿ���Ч
void CInfo::isval()
{		
	if(m_listenDir.size()<1){
		//û��Ҫ������Ŀ¼
		mylog << "The file to listen to was not found";
		exit(-1);
	}
	if(m_store.empty()){
		mylog << "store dir default 'D:/store'";
		m_store = "d:/store";
		_mkdir(m_store.c_str());
	}else{
		DWORD dwAttrib = GetFileAttributesA(m_store.c_str());
		if(!(INVALID_FILE_ATTRIBUTES != dwAttrib && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY))){
			mylog << "The store DIR to was not found: " + m_store;
			_mkdir(m_store.c_str());
		}
	}
	if(m_deal.empty()){
		 mylog << "deal dir default 'D:/deal'";
		 m_deal = "d:/deal";
		 _mkdir(m_deal.c_str());
	}else{
		DWORD dwAttrib = GetFileAttributesA(m_deal.c_str());
		if(!(INVALID_FILE_ATTRIBUTES != dwAttrib && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))
			mylog << "The deal DIR to was not found: " + m_deal ;
			_mkdir(m_deal.c_str());
	}
	if(m_ipaddr.empty())
		m_ipaddr = "127.0.0.1";
	if(m_port == 0)
		m_port = 8080;
}

static void screen(){
	fstream sc;
	sc.open("../screen.dat");
	if(!sc)
		return ;
	char szLine[MAX_PATH] = {0};
	int i=0;
	while(i++<17){
		sc.getline(szLine,MAX_PATH);
		cout << szLine << endl;
	}
	sc.close();
}
//��ʼ����Ҫ����Ϣ
bool CInfo::init()
{
	screen();
	fstream f;
	f.open("../DIR.ini");
	if(!f){
		mylog << string("File Opening Failed") + "\tPlease configure";
		return false;
	}
	char buf[MAX_PATH] = {0};
	while(!f.eof()){
		f.getline(buf,MAX_PATH);
		if(strlen(buf)>0){
			//�ж��Ƿ�Ϊ����Ŀ¼
			if(strncmp(buf,ISDIR,strlen(ISDIR)) == 0)
				this->m_listenDir.push_back(buf+strlen(ISDIR));
			//�ж��Ƿ�Ϊ�洢·��
			else if(strncmp(buf,ISSTORE,strlen(ISSTORE)) == 0)
				m_store = buf+strlen(ISSTORE);
			else if(strncmp(buf,ISDEAL,strlen(ISDEAL)) == 0)
				m_deal = buf+strlen(ISDEAL);
			//�ж��Ƿ���IP��ַ
			else if(strncmp(buf,ISIPADDR,strlen(ISIPADDR)) == 0)
				m_ipaddr = buf+strlen(ISIPADDR);
			//�ж��Ƿ��Ƕ˿ں�
			else if(strncmp(buf,ISPORT,strlen(ISPORT)) == 0)
				m_port = atoi(buf+strlen(ISPORT));
			//��0
			memset(buf,0,sizeof(buf));
		}
	}
	//�ж��Ƿ����,�����ڸ�Ĭ��ֵ
	isval();
	return true;
}

