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
	// Convert the data from keyword of "$"
	if (2 == sscanf(this->m_buffer, "%*[^$]$%f,%f", &ws,&wd))
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
	this->m_data[2].code = "!last";

	//reset buffer for next protocol data.
	this->ResetBuffer(); 
}


static int find_position_of_keywords(char *source, char *keywords)
{
	int source_length,keywords_length,i;
	int flag = -1;
	source_length = strlen(source);
	keywords_length = strlen(keywords);
	if (keywords_length > source_length) return -1;
	for (i = 0; i <= source_length - keywords_length; i++) {
		if  (strncmp(source + i, keywords, keywords_length) == 0) 
			return i;
	}
	return -1;
}
BOOL WMTProtocol::checkResponseValid()
{
	//return this->m_isResponseReady;

#if 1
	int start_pos =0;
	int end_pos =0;

	char *data = this->m_buffer; // get data from buffer.

	start_pos = find_position_of_keywords(data, "$");
	if(start_pos >= 0)
	{
		data += start_pos;
		end_pos = find_position_of_keywords(data, "\n");
		if(end_pos)
			return TRUE;
	}
#else
	for (int i = 0; i < m_buffer_index; i++){
		if (m_buffer[i] == 10 || m_buffer[i] == 13){
			return true;
		}
	}
#endif
	return false;
}