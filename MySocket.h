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
	const CMonitor * const m_Mon;		//指向CMonitor对象；
	bool conn();
	bool sendMsg(const string &);	      //发送文件路径
	void check();									 //验证账号密码
private:
	SOCKET m_socket;
	SOCKADDR_IN *m_addrsev;	//服务器地址
	WSADATA *m_wd;
	string m_ipAddr;
	short m_port;
};

#endif
