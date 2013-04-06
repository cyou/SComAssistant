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
	void setSerialPort(CSerialPort* serialPort);
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

	int timeout;

	Protocol* m_p_protocol;
	CSerialPort*  m_p_Port;  //CSerialPort�����

	int m_nBaud;         //������
	int m_nCom;          //���ں�
	char m_cParity;      //У��
	int m_nDatabits;     //����λ
	int m_nStopbits;     //ֹͣλ
	DWORD m_dwCommEvents;
};
#endif
