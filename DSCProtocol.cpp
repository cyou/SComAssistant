#include "stdafx.h"

#include "DSCProtocol.h"

DSCProtocol::DSCProtocol(CString name):Protocol(name)
{
	m_name = name;
}

DSCProtocol::~DSCProtocol()
{
}

CString DSCProtocol::GetName()
{
	return m_name;
}


char* DSCProtocol::ParseDataToSerialPort(char* szMsg)
{
	//convert messages from DSC potocal here, then you can send out it by serial port.
	// 
	return "";
}

char* DSCProtocol::ParseDataFromSerialPort(char* szMsg)
{
	//parse results from serial port and covert to details message then write to DB later.
	// 
	return "";
}
