#include "stdafx.h"

#include "Protocol.h"

Protocol::Protocol(CString name)
{

    m_cmdReader = new CIniReader(CMD_CFG);
	m_name = name;
	m_buffer_index = 0;
	m_buffer_length = 1024;
	m_buffer = new char[m_buffer_length];
	memset(m_buffer, 0, m_buffer_length);

	m_send_cmd = NULL;
	m_isResponseReady = TRUE;
}

Protocol::~Protocol()
{
	if (m_buffer)
		delete[] m_buffer;

	if (m_cmdReader)
		delete m_cmdReader;
	if (m_send_cmd)
		delete[] m_send_cmd;
}


void Protocol::AddDataToBuffer(char data)
{
	if (m_buffer_index < m_buffer_length)
	{
		m_buffer[m_buffer_index] = data;
		m_buffer_index ++;
	}
	else
	{
		m_buffer_index = 0; //if buffer is full, overwrite it...
	}
}

void Protocol::ResetBuffer()
{
	memset(this->m_buffer, 0, this->m_buffer_length);
	this->m_buffer_index = 0;
}

ProtocolData* Protocol::GetProtocolData()
{
	return this->m_data;
}

BOOL Protocol::checkResponseValid()
{
	return this->m_isResponseReady;
}