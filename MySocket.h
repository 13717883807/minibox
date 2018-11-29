#ifndef __MYSOCKET__
#define __MYSOCKET__

#include <windows.h>
 #include "Monitor.h"

class CMySocket
{
public:
	CMySocket(void);
	CMySocket(const CMonitor *const mon);
	~CMySocket(void);
public:
	const CMonitor * const m_Mon;		//ָ��CMonitor����
	bool conn();
	bool sendMsg(const string &);	      //�����ļ�·��
	void check();									 //��֤�˺�����
private:
	SOCKET m_socket;
	SOCKADDR_IN *m_addrsev;	//��������ַ
	WSADATA *m_wd;
	string m_ipAddr;
	short m_port;
};

#endif
