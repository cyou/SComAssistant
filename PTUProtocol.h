#include "stdafx.h"
#include "Protocol.h"

#ifndef PTUPROTOCOL_H
#define PTUPROTOCOL_H

class PTUProtocol : public Protocol
{
public:
	PTUProtocol(CString name);
	virtual	~PTUProtocol();

	CString GetName();
	char* ParseDataToSerialPort();
	void ParseDataFromSerialPort(const char* szMsg);
	BOOL checkResponseValid();
};

#endif
