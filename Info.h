#ifndef __INFO__
#define __INFO__

#include <string>
#include <vector>
using namespace std;

#define ISSTORE   "STORE="
#define ISDIR		  "DIR="
#define ISIPADDR "IP="
#define ISPORT	  "PORT="
#define ISDEAL	  "DEAL="

class CInfo
{
public:
	CInfo(void);
	~CInfo(void);
	bool init();
public:
	string m_store;						//中转文件的路径
	string m_deal;						//处理后的文件所在路径
	string m_ipaddr;					//服务器IP
	short  m_port;				        //端口
	vector<string> m_listenDir;	//保存需要监听的目录的容器
private:
	void isval();							//判断是否有效
};

#endif

												   