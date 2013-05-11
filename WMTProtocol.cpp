#include "stdafx.h"

#include "WMTProtocol.h"

WMTProtocol::WMTProtocol(CString name):Protocol(name)

{
	m_name = name;
	this->m_send_cmd = m_cmdReader->ReadString("Device", "wmt", "$1 POLL,21");
	int cmd_length = strlen(this->m_send_cmd);
	this->m_send_cmd[cmd_length] = '\r';
	this->m_send_cmd[cmd_length+1] = '\n';
}

WMTProtocol::~WMTProtocol()
{
}

CString WMTProtocol::GetName()
{
	return m_name;
}


char* WMTProtocol::ParseDataToSerialPort()
{
	//convert messages from DSC potocal here, then you can send out it by serial port.
	// 
	return this->m_send_cmd;
}

void WMTProtocol::ParseDataFromSerialPort(const char* szMsg)
{
	//parse results from serial port and covert to details message then write to DB later.
	//
	//CString data(szMsg);

	float ws,wd;
	// if convert fail, set valid to 0, otherwise, set it to 1.
	if (2 == sscanf(this->m_buffer, "$%f,%f", &ws,&wd))
	{
	   this->m_data[0].code = "ws";
	   this->m_data[0].value = ws; // get the windspeed
	   this->m_data[0].valid = 1;

	   this->m_data[1].code = "wd";
	   this->m_data[1].value = wd; // get the wind direction.
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

	//reset buffer for next protocol data.
	this->ResetBuffer(); 
}