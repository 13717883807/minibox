#ifndef __LED__
#define __LED__

#include <string>
#include <windows.h>
using namespace std;

class CLed
{
public:
	CLed(void);
	CLed(int);
	~CLed(void);			 	
private:		
	//ledµÆµÄÑÕÉ«
	string m_color;
	string m_cmd;
	PROCESS_INFORMATION m_pro;
public:
	void setColor(int);
	string& getColor();
	void turn_on();
};

#endif
