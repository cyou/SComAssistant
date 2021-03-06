#include "stdafx.h"
#include "Protocol.h"

#ifndef DSCPROTOCOL_H
#define DSCPROTOCOL_H

#define HEADER_LEN  4
//#define BODY_LEN  215


class DSCProtocol : public Protocol
{
public:
	DSCProtocol(CString name);
	virtual	~DSCProtocol();

	CString GetName();
	char* ParseDataToSerialPort();
	void ParseDataFromSerialPort(const char* szMsg);
	BOOL checkResponseValid();
private:
	void convertToProtocolData();
	void removeSpaces(char * dst, const char * src);
	int  dscData_length;

public:
    CString strHeader[HEADER_LEN];
    CString strBody[BODY_LEN];
};

#endif
