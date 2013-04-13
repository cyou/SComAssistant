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

	int m_nBaud;         //������
	int m_nCom;          //���ں�
	char m_cParity;      //У��
	int m_nDatabits;     //����λ
	int m_nStopbits;     //ֹͣλ
};
#endif
