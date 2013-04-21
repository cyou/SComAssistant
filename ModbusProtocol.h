#include "stdafx.h"
#include "Protocol.h"
#include "type.h"

#ifndef MODBUSPROTOCOL_H
#define MODBUSPROTOCOL_H

class ModbusProtocol : public Protocol
{
public:
	ModbusProtocol(CString name);
	virtual	~ModbusProtocol();

	CString GetName();
	char* ParseDataToSerialPort();
	void ParseDataFromSerialPort(const char* szMsg);
	uint16 ModbusProtocol::Modbus_CRC16(uint8 *Buff_addr,uint16 len);
};
#endif
