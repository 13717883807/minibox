#ifndef __MYLOG__
#define __MYLOG__

#include <iostream>
#include <Windows.h>
#include <fstream>
using namespace std;

class MyLog{
public:
	MyLog();
	~MyLog(){}
	void open(const char *);
	ostream& operator<<(const char *a);
	ostream& operator<<(const string &s);
	fstream *m_f;
};
extern MyLog mylog;

#endif