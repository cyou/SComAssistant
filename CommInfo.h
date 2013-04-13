#include "stdafx.h"
#include "SerialPort.h"

#ifndef COMMINFO_H
#define COMMINFO_H

class CommInfo
{
public:
	CommInfo();
	CommInfo(int m_nCom, int m_nBaud, char m_cParity, int m_nDatabits, int m_nStopbits);
	virtual	~CommInfo();

	void setBand(int m_nBaud);
	int getBand();

	void setCom(int m_nCom);
	int getCom();

	void setParity(char m_cParity);
	char getParity();

	void setDatabits(int m_nDatabits);
	int getDatabits();

	void setStopbits(int m_nStopbits);
	int getStopbits();

private:

	int m_nBaud;         //波特率
	int m_nCom;          //串口号
	char m_cParity;      //校验
	int m_nDatabits;     //数据位
	int m_nStopbits;     //停止位
};
#endif
