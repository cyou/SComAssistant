#include "stdafx.h"
#include "Protocol.h"

#ifndef PTUPROTOCOL_H
#define PTUPROTOCOL_H

#define HEADER_LEN  4
//#define BODY_LEN  215


class PTUProtocol : public Protocol
{
public:
	PTUProtocol(CString name);
	virtual	~PTUProtocol();

	CString GetName();
	char* ParseDataToSerialPort();
	void ParseDataFromSerialPort(const char* szMsg);
private:
	void convertToProtocolData();
	void removeSpaces(char * dst, const char * src);
	int  dscData_length;

public:
    CString strHeader[HEADER_LEN];
    CString strBody[BODY_LEN];
};

#endif
