#include "stdafx.h"

#include "ModbusProtocol.h"

ModbusProtocol::ModbusProtocol(CString name):Protocol(name), m_CRC16(CRC16::instance())
{
	m_name = name;
	this->m_send_cmd = new char[8];
	for (int i = 0; i < 8; i++){
		this->m_send_cmd[i] = 0;
	}

	//m_CRC16 = CRC16::instance();
}

ModbusProtocol::~ModbusProtocol()
{
	if (this->m_send_cmd) {
		delete[] m_send_cmd;
	}
}

CString ModbusProtocol::GetName()
{
	return m_name;
}

char* ModbusProtocol::ParseDataToSerialPort()
{
	//convert messages from modbus potocal here, then you can send out it by serial port.
	// 
	this->m_send_cmd[0] = m_cmdReader->ReadInteger("Device", "modbus", 1);
	this->m_send_cmd[1] = (char)0x04;
	this->m_send_cmd[2] = (char)0x00;
	this->m_send_cmd[3] = (char)0x00;
	this->m_send_cmd[4] = (char)0x00;
	this->m_send_cmd[5] = (char)0x02;

	//this->m_send_cmd[5] = (char)0x02;
	this->m_CRC16.Modbus_CRC16((uint8 *) this->m_send_cmd, 6);


	this->m_send_cmd[6] = (char)this->m_CRC16.CRC16_Lo();
	this->m_send_cmd[7] = (char)this->m_CRC16.CRC16_Hi();
	
	return this->m_send_cmd;
}

void ModbusProtocol::ParseDataFromSerialPort(const char* szMsg)
{
	//parse results from serial port and covert to details message by modbus protocol then write to DB later.
	//
	// setup sample
	this->AddDataToBuffer((char)0x01);
	this->AddDataToBuffer((char)0x04);
	this->AddDataToBuffer((char)0x04);
	
	this->AddDataToBuffer((char)0x42);
	this->AddDataToBuffer((char)0xc3);
	this->AddDataToBuffer((char)0x99);
	this->AddDataToBuffer((char)0x9a);

	this->AddDataToBuffer((char)0xf5);
	this->AddDataToBuffer((char)0xfb);


	float tmp_value = 0.0f;

	//convert data to protocol data.
	this->m_data[0].code = "99"; // use code.
	this->m_data[0].value = 0;
	this->m_data[0].valid = 0;

	int iCMDCode = m_cmdReader->ReadInteger("Device", "modbus", 1);
	if (this->m_buffer[0] != (char)iCMDCode) {
		this->ResetBuffer();
		return;
	}
	
	/*uint16 crc = this->Modbus_CRC16((uint8 *) this->m_buffer, 7);
	//if ()
	uint8 highCRC = (uint8)(crc & 0x00FF);
	uint8 lowCRC  = (uint8)(crc>>8 & 0xFF);*/

	this->m_CRC16.Modbus_CRC16((uint8 *) this->m_send_cmd, 6);


	uint8 lowCRC = (char)this->m_CRC16.CRC16_Lo();
	uint8 highCRC = (char)this->m_CRC16.CRC16_Hi();

	if (lowCRC != (uint8)this->m_buffer[7] || highCRC != (uint8)this->m_buffer[8]){
		this->ResetBuffer();
		return;
	}
	
	char *p = (char *)&tmp_value;
	p[0] = this->m_buffer[6];
	p[1] = this->m_buffer[5];
	p[2] = this->m_buffer[4];
	p[3] = this->m_buffer[3];

	//reset buffer for next protocol data.
	this->ResetBuffer(); 

	this->m_data[0].value = tmp_value;
	this->m_data[0].valid = 1;
}


