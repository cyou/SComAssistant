#include "stdafx.h"

#include "Protocol.h"

Protocol::Protocol(CString name)
{
	m_name = name;
	m_buffer_index = 0;
	m_buffer_length = 256;
	m_buffer = new char[m_buffer_length];
	memset(m_buffer, 0, m_buffer_length);
}

Protocol::~Protocol()
{
	if (m_buffer)
		delete[] m_buffer;
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

ProtocolData* Protocol::GetProtocolData()
{
	return this->m_data;
}