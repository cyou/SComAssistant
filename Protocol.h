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

protected:
	CString m_name;
};
#endif
