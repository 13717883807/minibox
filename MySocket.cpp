#include "MySocket.h"
#include "mylog.h"
#pragma comment(lib,"ws2_32.lib")


CMySocket::CMySocket(const CMonitor* const mon):m_Mon(mon)
{
	m_ipAddr = m_Mon->m_info->m_ipaddr;
	m_port = m_Mon->m_info->m_port;
	m_wd = new WSADATA;
	m_addrsev = new SOCKADDR_IN;
	::WSAStartup(MAKEWORD(2,2),m_wd);
	//建立与服务器的连接
	if(!conn()){
		mylog << "3 seconds to exit......";
		Sleep(3000);
		exit(0);
	}
	//验证账号密码
	check();
}

CMySocket::~CMySocket(void)
{
}

bool CMySocket::conn()
{
	//创建套接字
	m_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	m_addrsev->sin_family = AF_INET;
	m_addrsev->sin_port = htons(m_port);
	m_addrsev->sin_addr.S_un.S_addr = inet_addr(m_ipAddr.c_str());
	if(0 == connect(m_socket,(SOCKADDR*)m_addrsev,sizeof(*m_addrsev))){
		mylog << "connect success...";		 
		return true;
	}else{
		mylog << "connect failed.....";
		return false;
	}

}
bool CMySocket::sendMsg(const string &msg)
{
	//将信息编码成unicode
	int nLength = MultiByteToWideChar(CP_ACP, 0, msg.c_str(), -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[nLength + 1];
	memset(pUnicode, 0, (nLength+1)*sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, msg.c_str(), -1, pUnicode, nLength);
	//将unicode转换成utf8
	nLength = WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, NULL, 0, NULL, NULL);
	BYTE* pUTF8Code = new BYTE[nLength + 1];
	memset(pUTF8Code, 0, (nLength+1)*sizeof(BYTE));
	WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, (LPSTR)pUTF8Code, nLength, NULL, NULL);
	send(m_socket,(char *)pUTF8Code,nLength,0);
	return true;
}

void CMySocket::check()
{
	string username,password;
	cout << "please login:" << endl;
	char buf[10] = {0};
	int i = 3;
	while(i--){
		cout << "username: ";
		cin >> username;
		cout << "password: ";
		cin >> password;
		sendMsg(username+"@"+password);
		recv(m_socket,buf,128,0);
		if(strncmp(buf,"yes",3) == 0){
			cout << "success connect" << endl;
			break;
		}
		else{
			cout << "username or password error" << endl; 
			cout << "Please re-enter:" << " You can also input "<< i <<  " times." << endl;
			if(i==0) {
				cout << "input error" << endl;
				system("PAUSE");
				exit(0);
			}
		}
	}
}
