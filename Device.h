#include "stdafx.h"
#include "SerialPort.h"
#include "Protocol.h"
#include "CommInfo.h"
#include "DeviceData.h"


#ifndef DEVICE_H
#define DEVICE_H

class Device
{
public:
	Device(int device_id, int type); //pass device id and device type.
	virtual	~Device();

	void setCommInfo(CommInfo commInfo);
	void setProtocol(Protocol* protocol);
	Protocol* getProtocol();
	void setSerialPort(CSerialPort* serialPort);
	BOOL openDevice();
	void closeDevice();
	int getDeviceType();
	void setDeviceType(int type);

	void sendCommand();
	ProtocolData* getCommandResponse();
	void convertToDeviceData(DeviceData* d_data, ProtocolData* p_data);

	BOOL isActive();
	BOOL isDeviceOpen();

	void setActive(BOOL active);

	void handleTimeout(int interval);

private:
	CString m_name;
	BOOL m_is_device_open;
	BOOL m_is_active;
	BOOL m_is_timeout;
	int device_id;
	int device_type; // 0 means DSC device, 1 means Modbus device.

	int timeout;

	Protocol* m_p_protocol;
	CSerialPort*  m_p_Port;  //CSerialPort¿‡∂‘œÛ

	CommInfo m_commInfo;
	DWORD m_dwCommEvents;
};
#endif
