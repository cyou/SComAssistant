#include "stdafx.h"

#include "PTUProtocol.h"

PTUProtocol::PTUProtocol(CString name):Protocol(name)

{
	m_name = name;
	this->m_send_cmd = m_cmdReader->ReadString("Device", "ptu", "SEND 1");

	int cmd_length = strlen(this->m_send_cmd);
	this->m_send_cmd[cmd_length] = '\r';
	this->m_send_cmd[cmd_length+1] = '\n';
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
	float pressure,tempeture,humity;
	int	index =0;
	// if convert fail, set valid to 0, otherwise, set it to 1.
	
	//if (2 == sscanf("2004-07-05 03:48:04 RH= 98.5 %RH T= 31.1 'C", "%*s%*s%*s%f%*s%*s%f", &ws,&wd))

	if (1 == sscanf(this->m_buffer, "%*[^P=]P=%f", &pressure))
	{
		this->m_data[index].code = "PTU_P";
		this->m_data[index].value = pressure;
		this->m_data[index].valid = 1;
		index ++;
	}

	if (1 == sscanf(this->m_buffer, "%*[^RH=]RH=%f", &humity))
	{
		this->m_data[index].code = "PTU_RH";
		this->m_data[index].value = humity;
		this->m_data[index].valid = 1;
		index++;
	}

	if (1 == sscanf(this->m_buffer, "%*[^T=]T=%f", &tempeture))
	{
		this->m_data[index].code = "PTU_T";
		this->m_data[index].value = humity;
		this->m_data[index].valid = 1;
		index++;
	}
	this->m_data[index].code = "!last";
	this->m_data[index].valid = 0;

	//reset buffer for next protocol data.
	this->ResetBuffer(); 
}