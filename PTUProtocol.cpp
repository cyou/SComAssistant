#include "stdafx.h"

#include "PTUProtocol.h"

PTUProtocol::PTUProtocol(CString name):Protocol(name)

{
	m_name = name;
	this->m_send_cmd = m_cmdReader->ReadString("Device", "ptu", "SEND 1");
}

PTUProtocol::~PTUProtocol()
{
}

CString PTUProtocol::GetName()
{
	return m_name;
}


char* PTUProtocol::ParseDataToSerialPort()
{
	//convert messages from DSC potocal here, then you can send out it by serial port.
	// 
	return this->m_send_cmd;
}

void PTUProtocol::ParseDataFromSerialPort(const char* szMsg)
{
	//parse results from serial port and covert to details message then write to DB later.
	//
	//CString data(szMsg);
	float ws,wd;
	// if convert fail, set valid to 0, otherwise, set it to 1.
	
	//if (2 == sscanf("2004-07-05 03:48:04 RH= 98.5 %RH T= 31.1 'C", "%*s%*s%*s%f%*s%*s%f", &ws,&wd))
	if (2 == sscanf(this->m_buffer, "%*s%*s%*s%f%*s%*s%f", &ws,&wd))
	{
	   this->m_data[0].code = "ws";
	   this->m_data[0].value = ws; 
	   this->m_data[0].valid = 1;

	   this->m_data[1].code = "wd";
	   this->m_data[1].value = wd;
	   this->m_data[1].valid = 1;
	}
	else
	{
	   this->m_data[0].code = "ws";
	   this->m_data[0].value = 0; 
	   this->m_data[0].valid = 0;

	   this->m_data[1].code = "wd";
	   this->m_data[1].value = 0;
	   this->m_data[1].valid = 0;
	}
}