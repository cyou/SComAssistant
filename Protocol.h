#include "stdafx.h"

#ifndef PROTOCOL_H
#define PROTOCOL_H

class Protocol
{
public:
	Protocol(CString name);
	virtual	~Protocol();

	virtual CString GetName() = 0;
	virtual char* ParseDataToSerialPort(char* szMsg) = 0; // convert data from protocol sub class to serial port.
	virtual char* ParseDataFromSerialPort(char* szMsg) = 0; // convert data from serial to upper protocol class.
	void AddDataToBuffer(char data);

protected:
	CString m_name;

	char* m_buffer;
	int m_buffer_length = 128;
	int m_buffer_index;
};
#endif
