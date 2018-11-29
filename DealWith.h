#pragma once
#include <string>
#include "Monitor.h"
using namespace std;

class CDealWith
{
public:
	CDealWith(void);
	CDealWith(const CMonitor*,const string& path);
	~CDealWith(void);
public:
	const CMonitor *m_Mon;
	string m_absPath;
	string m_Sql;
	void run();
private:
	bool move();
	bool isCSV();
	void deal();
};

