#include "stdafx.h"
#include "SerialPort.h"
#include "Device.h"
#include "DSCProtocol.h"
#include "ModbusProtocol.h"


Device::Device(int device_id, int type)
{
	this->device_id = device_id;
//	this->m_nCom = ncom;
	this->m_is_device_open = false;
	this->device_type = type;
	if (this->device_type == 1) {

		m_p_protocol = new DSCProtocol("dsc");
	} 
	else
	{
		m_p_protocol = new ModbusProtocol("modbus");
	}
	
}

Device::~Device()
{	
	if (this->m_is_device_open) {
		this->closeDevice();
	}

	delete m_p_protocol;
}

void Device::setSerialPort(CSerialPort* p_serialPort)
{
	this->m_p_Port = p_serialPort;
}

void Device::setCommInfo(CommInfo commInfo)
{
	m_commInfo.setCom(commInfo.getCom());
	m_commInfo.setBand(commInfo.getBand());
	m_commInfo.setParity(commInfo.getParity());
	m_commInfo.setDatabits(commInfo.getDatabits());
	m_commInfo.setStopbits(commInfo.getStopbits());
}

BOOL Device::openDevice()
{
	m_is_device_open = true;
	// open and initial serial port here.
	return true;
}


void Device::closeDevice()
{
	m_is_device_open = false;
	// close serial port here.
}


void Device::sendCommand(char* cmd)
{
	// convert cmd through protocol implement class.

	// write data to serial 
}

char* Device::getCommandResponse()
{
	// read data from serial port.

	// covert it to meanful string through protocol convertor.
	return "result";
}


void Device::writeDB(char* res)
{
	// write data to database.
}

