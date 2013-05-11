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

	this->m_p_protocol = NULL;
	/*if (this->device_type == 1) {

		m_p_protocol = new DSCProtocol("dsc");
	} 
	else
	{
		m_p_protocol = new ModbusProtocol("modbus");
	}*/
	
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


Protocol* Device::getProtocol()
{
	if (this->m_p_protocol)
		return this->m_p_protocol;
	return NULL;
}

void Device::setProtocol(Protocol* protocol)
{
	this->m_p_protocol = protocol;
}

int Device::getDeviceType()
{
	return this->device_type;
}

BOOL Device::isActive()
{
	return this->m_is_active;
}

BOOL Device::isDeviceOpen()
{
	return this->m_is_device_open;
}

void Device::setActive(BOOL active)
{
	this->m_is_active = active;
}

void Device::setDeviceType(int type)
{
	this->device_type = type;
}

void Device::closeDevice()
{
	m_is_device_open = false;
	// close serial port here.
}


void Device::sendCommand()
{
	this->m_is_timeout = false;
	// convert cmd through protocol implement class.
	char* data = this->m_p_protocol->ParseDataToSerialPort();
	// write data to serial 
	if (this->m_p_Port->m_hComm) {
		this->m_p_Port->WriteToPort(data);
	}
}

ProtocolData* Device::getCommandResponse()
{
	// read data from serial port.
	for (;;){
		if (m_is_timeout) {
			m_is_timeout = false;
			this->getProtocol()->ParseDataFromSerialPort(NULL); 
			return this->getProtocol()->GetProtocolData();
		}
		// sleep pull thread some time.
		::Sleep(100);
	}
}

void Device::handleTimeout(int interval)
{
	// receive timeout event.
	// try to verify received data from serial port.
	this->m_is_timeout = true;
}


void Device::convertToDeviceData(DeviceData * d_data, ProtocolData* p_data)
{
	for (int i = 0; i < BODY_LEN; i++){
		if (p_data[i].code == "!last" ||p_data[i].code == ""  )
		{
			break;
		}
		else if (p_data[i].code == "01"){
			d_data->GRANDTEMPERATURE = p_data[i].value;
			d_data->GRANDTEMPERATUREFLG = p_data[i].valid;
			continue;
		}
		else if (p_data[i].code == "68"){
			d_data->GRIPLEVEL = p_data[i].value;
			d_data->GRIPLEVELFLG = p_data[i].valid;
			continue;
		}
		else if (p_data[i].code == "30"){
			d_data->GRANDTEMPERATURE = p_data[i].value;
			d_data->GRANDTEMPERATUREFLG = p_data[i].valid;
			continue;
		}
		else if (p_data[i].code == "02"){
			d_data->GRANDHUMIDITY = p_data[i].value;
			d_data->GRANDHUMIDITYFLG = p_data[i].valid;
			continue;
		}
		else if (p_data[i].code == "72"){
			d_data->WATERPERCENT = p_data[i].value;
			d_data->WATERPERCENTFLG = p_data[i].valid;
			continue;
		}
		else if (p_data[i].code == "ns"){ // from modbus protocol.
			d_data->NOISE = p_data[i].value;
			d_data->NOISEFLG = p_data[i].valid;
			break;
		}
		else if (p_data[i].code == "ws"){ // from wmt protocol.
			d_data->WINDSPEED = p_data[i].value;
			d_data->WINDSPEEDFLG = p_data[i].valid;
			continue;
		}
		else if (p_data[i].code == "wd"){ // from wmt protocol.
			d_data->WINDDIRECT = p_data[i].value;
			d_data->WINDDIRECTFLG = p_data[i].valid;
			break;
		}
		else if (p_data[i].code == "PTU_P"){ // from ptu protocol.
			d_data->PRESSURE = p_data[i].value;
			d_data->PRESSUREFLG = p_data[i].valid;
			continue;
		}
		else if (p_data[i].code == "PTU_RH"){ // from ptu protocol.
			d_data->HUMIDITY = p_data[i].value;
			d_data->HUMIDITYFLG = p_data[i].valid;
			continue;
		}
		else if (p_data[i].code == "PTU_T"){ // from ptu protocol.
			d_data->TEMPERATURE = p_data[i].value;
			d_data->TEMPERATUREFLG= p_data[i].valid;
			continue;
		}
	}
	// convert protocol data to device data.
}

