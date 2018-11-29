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
	queue<string> m_queue;		  //�����������ļ�
	vector<HANDLE> m_hThread;  //�����߳̾��
	HANDLE m_hTPrintInfo;		//��ӡ�߳̾��
	HANDLE m_hTSendMsg;		//����֪ͨ�߳̾��
	HANDLE m_hEvent;				//�¼����		
	CMySocket *m_socket;
	CLed *m_led;
	
};

//ֻΪ����
struct Arg{
	CMonitor *mt;	//����this
	string path;	   //���������Ŀ¼��
};

//����Ŀ¼
DWORD CALLBACK listenDir(void *arg);
//ÿ��10���ӡһ������������Ϣ
DWORD CALLBACK printInfo(void *arg);
//�Ӷ�����ȡ���ļ����͵�������
DWORD CALLBACK sendMsg(void *arg);

#endif

