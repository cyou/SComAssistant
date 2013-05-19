#include "stdafx.h"
#include "Protocol.h"

#ifndef WMTPROTOCOL_H
#define WMTPROTOCOL_H

class WMTProtocol : public Protocol
{
public:
	WMTProtocol(CString name);
	virtual	~WMTProtocol();

	CString GetName();
	char* ParseDataToSerialPort();
	void ParseDataFromSerialPort(const char* szMsg);
	BOOL checkResponseValid();
};

#endif
