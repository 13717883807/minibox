#include "DealWith.h"
#include "MySocket.h"
#include "mylog.h"
#include <fstream>
#include <io.h>
#include <direct.h>

CDealWith::CDealWith(void)
{
}

CDealWith::CDealWith(const CMonitor* mon,const string &path):m_Mon(mon)
{
	m_absPath = path;
	m_Sql = "D:/Sql/";
	DWORD dwAttrib = GetFileAttributesA(m_Sql.c_str());
	if(!(INVALID_FILE_ATTRIBUTES != dwAttrib && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY))){
		mylog << "The database DIR to was not found: " + m_Sql + "auto create";
		_mkdir(m_Sql.c_str());
	}
}

CDealWith::~CDealWith(void)
{
}

void CDealWith::run()
{
	if(m_absPath.empty())
		return;
	//通知服务器
	m_Mon->m_socket->sendMsg("new file: "+m_absPath);
	m_Mon->m_led->setColor(CMonitor::blue);
	m_Mon->m_led->turn_on();
	//挪到中间文件夹
	if(!move())
		mylog << "move failed...." + m_absPath;
	//判断是否为csv文件
	if(isCSV()){
 		mylog << m_absPath + " is csv";
		deal();
	}else{
		//删除文件
		DeleteFileA(m_absPath.c_str());
		mylog << "remove isnot 'csv' type file: " + m_absPath;
	}
}

bool CDealWith::move()
{
	char path[MAX_PATH] = {0};
	sprintf(path,"%s%s",m_Mon->m_info->m_store.c_str(),strrchr(m_absPath.c_str(),'/'));
	//说明文件已存在，名字冲突
	if(_access(path,0) != -1){
		char *p = strrchr(path,'.');
		//把文件名后缀提出来
		char tmp[8];
		strcpy(tmp,p);
		*p = '_';
		*(p+1) = '1';
		*(p+2) = '\0';
		strcat(path,tmp);
	}
	while(_access(path,0) != -1)
		*(strrchr(path,'.')-1) +=1;			//.前方数字+1;
	if(0 != MoveFileA(m_absPath.c_str(),path)){
		m_absPath = path;
		return true;
	}
	return false;
}

bool CDealWith::isCSV()
{																 
	if(m_absPath.substr(m_absPath.size()-4) == string(".csv")){
		//是csv文件
		return true;
	}else
		return false;
}
void CDealWith::deal()
{
	//处理文件
	//在B最终文件夹下创建处理后的数据
	string dealPath = this->m_Mon->m_info->m_deal+m_absPath.substr(m_absPath.find_last_of('/'));
	dealPath.insert(dealPath.find_last_of('.'),"(deal)");
	fstream infile(m_absPath,fstream::in);
 	fstream outfile(dealPath,fstream::out);
	//读取写入处理
	string buf;
	while(!infile.eof()){
		getline(infile,buf);  
		int pos;
		outfile << atoi(buf.c_str())*2;
		while((pos = buf.find(',')) != -1){
			buf = buf.substr(pos+1);
			outfile << ','<< atoi(buf.c_str())*2;
		}
		outfile << endl;
	}
	infile.close();
	outfile.close();
	//删除文件夹A中的原始数据
	//把原始数据和处理后的数据都复制文件夹C中，C暂时假设为数据库。C定在D:/sql;
	CopyFileA(m_absPath.c_str(),(m_Sql+m_absPath.substr(m_absPath.find_last_of('/'))).c_str(),false);
	DeleteFileA(m_absPath.c_str());
	CopyFileA(dealPath.c_str(),(m_Sql+dealPath.substr(m_absPath.find_last_of('/'))).c_str(),false);
	//通知服务器
	m_Mon->m_socket->sendMsg("deal with: "+m_Sql+dealPath.substr(m_absPath.find_last_of('/')));
	mylog << "sendmsg success";
}
