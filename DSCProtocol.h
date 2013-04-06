#include "stdafx.h"
#include "Protocol.h"

#ifndef DSCPROTOCOL_H
#define DSCPROTOCOL_H

class DSCProtocol : public Protocol
{
public:
	DSCProtocol(CString name);
	virtual	~DSCProtocol();

	CString GetName();
	char* ParseDataToSerialPort(char* szMsg);
	char* ParseDataFromSerialPort(char* szMsg);
};
#endif
