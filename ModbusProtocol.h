#include "stdafx.h"
#include "Protocol.h"

#ifndef MODBUSPROTOCOL_H
#define MODBUSPROTOCOL_H

class ModbusProtocol : public Protocol
{
public:
	ModbusProtocol(CString name);
	virtual	~ModbusProtocol();

	CString GetName();
	char* ParseDataToSerialPort(char* szMsg);
	char* ParseDataFromSerialPort(char* szMsg);
};
#endif
