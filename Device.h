#include "stdafx.h"
#include "SerialPort.h"
#include "Protocol.h"


#ifndef DEVICE_H
#define DEVICE_H

class Device
{
public:
	Device(int device_id, int ncom, int type); //pass device id, comm id and device type.
	virtual	~Device();

	void setCommInfo(int m_nBaud, char m_cParity, int m_nDatabits, int m_nStopbits);
	void setProtocol(Protocol* protocol);
	BOOL openDevice();
	void closeDevice();

	void sendCommand(char* cmd);
	char* getCommandResponse();
	void writeDB(char* res);

private:
	CString m_name;
	BOOL m_is_device_open;
	int device_id;
	int device_type; // 1 means DSC device, 2 means Modbus device.

	Protocol* m_p_protocol;
	int m_nBaud;         //波特率
	int m_nCom;          //串口号
	char m_cParity;      //校验
	int m_nDatabits;     //数据位
	int m_nStopbits;     //停止位
	CSerialPort m_Port;  //CSerialPort类对象
	DWORD m_dwCommEvents;
};
#endif
