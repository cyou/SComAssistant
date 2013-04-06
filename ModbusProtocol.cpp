#include "stdafx.h"

#include "ModbusProtocol.h"

ModbusProtocol::ModbusProtocol(CString name):Protocol(name)
{
	m_name = name;
}

ModbusProtocol::~ModbusProtocol()
{
}

CString ModbusProtocol::GetName()
{
	return m_name;
}

char* ModbusProtocol::ParseDataToSerialPort(char* szMsg)
{
	//convert messages from modbus potocal here, then you can send out it by serial port.
	// 
	return "";
}

char* ModbusProtocol::ParseDataFromSerialPort(char* szMsg)
{
	//parse results from serial port and covert to details message by modbus protocol then write to DB later.
	// 
	return "";
}
