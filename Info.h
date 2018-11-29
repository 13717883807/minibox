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
	string m_store;						//��ת�ļ���·��
	string m_deal;						//�������ļ�����·��
	string m_ipaddr;					//������IP
	short  m_port;				        //�˿�
	vector<string> m_listenDir;	//������Ҫ������Ŀ¼������
private:
	void isval();							//�ж��Ƿ���Ч
};

#endif

												   