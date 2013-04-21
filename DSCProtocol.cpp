#include "stdafx.h"

#include "DSCProtocol.h"


DSCProtocol::DSCProtocol(CString name):Protocol(name)
{
	m_name = name;
	dscData_length = 0;
}

DSCProtocol::~DSCProtocol()
{
}

CString DSCProtocol::GetName()
{
	return m_name;
}


char* DSCProtocol::ParseDataToSerialPort(char* szMsg)
{
	//convert messages from DSC potocal here, then you can send out it by serial port.
	// 
	return "";
}

void DSCProtocol::ParseDataFromSerialPort(const char* szMsg)
{
	//parse results from serial port and covert to details message then write to DB later.
	//
	CString data(szMsg);
    for(int i=0;i<HEADER_LEN;i++)
    {
	   AfxExtractSubString(strHeader[i], data, i, _T(','));
    }
    strHeader[3].Replace("DSC", ""); // remove DSC
    for(int j=0;j<BODY_LEN;j++)
    {
	   AfxExtractSubString(strBody[j], strHeader[3], j, _T(';'));
	   strBody[j].TrimLeft(_T("\t \r \n \r\n")); // remove tab/enter character.
	   strBody[j].TrimRight(_T("\t \r \n \r\n")); // remove tab/enter character.
	   if (strBody[j].Compare("=") == 0) {
		   dscData_length = j;
		   break;
	   }
    }
	this->covertToProtocolData();
}

void DSCProtocol::covertToProtocolData()
{
	for(int i=0;i<dscData_length;i++)
    {
		// remove addtional spaces.
		//
		char simplifyStr[256];
		removeSpaces(simplifyStr,(LPCTSTR)strBody[i]);
		CString cstr(simplifyStr);

		CString pair[2];  
		for (int j =0; j < 2; j++){
			AfxExtractSubString(pair[j], cstr, j, _T(' ')); // split data with 
		}

	   this->m_data[i].code = pair[0];
	   this->m_data[i].value = (float)atof(pair[1]); // if convert fail, set valid to 0, otherwise, set it to 1.
	   this->m_data[i].valid = 1;
    }
}

void DSCProtocol::removeSpaces(char * dst, const char * src)
{
    for (; *src; ++dst, ++src) {
        *dst = *src;
        if (isspace(*src))
            while (isspace(*(src + 1)))
                ++src;
    }
    *dst = '\0';
}
