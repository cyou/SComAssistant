#include "stdafx.h"
#include "Protocol.h"
#include "type.h"
#include "CRC16.h"

#ifndef MODBUSPROTOCOL_H
#define MODBUSPROTOCOL_H

class ModbusProtocol : public Protocol
{
private:
	CRC16 &m_CRC16;
public:
	ModbusProtocol(CString name);
	virtual	~ModbusProtocol();

	CString GetName();
	char* ParseDataToSerialPort();
	void ParseDataFromSerialPort(const char* szMsg);
};
#endif
