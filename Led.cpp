#include "Led.h"
#include <windows.h>

CLed::CLed(void)
{
	m_cmd = "led.exe ";
}
CLed::CLed(int color)
{
	m_cmd = "led.exe ";
	setColor(color);
}

CLed::~CLed(void)
{
	
}

void CLed::setColor(int color)
{
	switch(color){
	case 0://红色
		m_color = "255 0 0";
		break;
	case 1://绿色
		m_color = "0 255 0";
		break;
	case 2://蓝色
		m_color = "0 0 255";
		break;
	default:
		m_color = "255 255 255";
	}
}

string& CLed::getColor()
{
	return this->m_color;
}

void CLed::turn_on()
{	
	m_cmd = "led.exe ";
	STARTUPINFOA si = {0};
	si.cb = sizeof(si);
	m_cmd += m_color;
	char cmd[64];
	strcpy(cmd,m_cmd.c_str());
	CreateProcessA(NULL,cmd , NULL, NULL, FALSE, 0, NULL, NULL, &si, &m_pro);
	WaitForSingleObject(m_pro.hProcess,INFINITE);
	//关闭资源
	CloseHandle(m_pro.hProcess);
	CloseHandle(m_pro.hThread);
}
