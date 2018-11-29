#include "mylog.h"
#include <iostream>
#include <time.h>
#include <stdio.h>
using namespace std;

MyLog::MyLog()
{
	m_f = new fstream;
}

ostream& MyLog::operator<<(const char *a){
	time_t t = time(0);
	struct tm *ptm = localtime(&t);
	char szCurrentDateTime[MAX_PATH] = {0};
	sprintf(szCurrentDateTime, "[%d-%.2d-%.2d %.2d:%.2d:%.2d]  ",     
		ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,     
		ptm->tm_hour, ptm->tm_min, ptm->tm_sec);	 
	cout << szCurrentDateTime << a << endl;
	*m_f << szCurrentDateTime << a << endl;
	return cout;
}

ostream& MyLog::operator<<(const string &s){
	return this->operator<<(s.c_str());
}

void MyLog::open(const char *lg)
{
	time_t t = time(0);
	struct tm *ptm = localtime(&t);
	char szCurrentDateTime[MAX_PATH] = {0};
	sprintf(szCurrentDateTime, "%d%02d%02d%02d%02d%02d.txt",     
		ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,     
		ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	string sc(lg);
	sc+= szCurrentDateTime;
	m_f->open(sc,fstream::out);
}

MyLog mylog;

