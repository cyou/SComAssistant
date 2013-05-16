#include "stdafx.h"

#include "DSCProtocol.h"

DSCProtocol::DSCProtocol(CString name):Protocol(name)

{
	m_name = name;
	dscData_length = 0;
	this->m_send_cmd = m_cmdReader->ReadString("Device", "dsc", "@1 MES 16");

	int cmd_length = strlen(this->m_send_cmd);
	this->m_send_cmd[cmd_length] = '\r';
	this->m_send_cmd[cmd_length+1] = '\n';
}

DSCProtocol::~DSCProtocol()
{
}

CString DSCProtocol::GetName()
{
	return m_name;
}


char* DSCProtocol::ParseDataToSerialPort()
{
	//convert messages from DSC potocal here, then you can send out it by serial port.
	// 
	return this->m_send_cmd;
}

void DSCProtocol::ParseDataFromSerialPort(const char* szMsg)
{
	//parse results from serial port and covert to details message then write to DB later.
	//
	//CString data(szMsg);
	CString data(this->m_buffer); // get data from buffer.
	char dsc_buffer[256]={0};

	if (1 == sscanf(data, "%*[^D]DSC %[0-9a-z.;/- ]", dsc_buffer))
	{
		CString dsc_str(dsc_buffer);
		for(int j=0;j<BODY_LEN;j++)
		{
		   AfxExtractSubString(strBody[j], dsc_buffer, j, _T(';'));
		   strBody[j].TrimLeft(_T("\t \r \n \r\n")); // remove tab/enter character.
		   strBody[j].TrimRight(_T("\t \r \n \r\n")); // remove tab/enter character.
		   dscData_length = j;
		   if (strBody[j].Compare("") == 0) {
			   break;
		   }
		}
	}
	else
	{
		dscData_length = 0;
	}
	// reset protocol buffer.
	this->ResetBuffer();//ResetBuffer
	this->convertToProtocolData();
}

void DSCProtocol::convertToProtocolData()
{
	float tmp;
	int i;
	for(i=0;i<dscData_length;i++)
    {
		// remove addtional spaces.
		//
		char simplifyStr[256];
		removeSpaces(simplifyStr,(LPCTSTR)strBody[i]);
		CString cstr(simplifyStr);

		CString pair[2];  
		for (int j =0; j < 2; j++){
			AfxExtractSubString(pair[j], cstr, j, _T(' ')); // split data with space
		}

	   this->m_data[i].code = pair[0];
	   this->m_data[i].valid = 0;
#if 1
		int code;
		float value;
		int result;
		result = sscanf(cstr,"%d %f",&code, &value);

		if(result == 2)
		{
			this->m_data[i].value = value;
			this->m_data[i].valid = 1;
		}
		else if(result == 1)
		{
			this->m_data[i].value = 0;
		}
		else if(result <= 0)
			break;
#else
	  // if convert fail, set valid to 0, otherwise, set it to 1.
	   if (1 == sscanf(pair[1], "%f", &tmp))
	   {
			this->m_data[i].value = tmp;
			this->m_data[i].valid = 1;
	   }
	   else
	   {
			this->m_data[i].value = 0;
			this->m_data[i].valid = 0;
			// break;
	   }
#endif
    }

    if (BODY_LEN == dscData_length){
		this->m_data[i-1].code = "!last";
	}else {
		this->m_data[i].code = "!last";
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

BOOL DSCProtocol::checkResponseValid()
{
	//return this->m_isResponseReady;
	for (int i = 0; i < m_buffer_index; i++){
		if (m_buffer[i] == 10 || m_buffer[i] == 13){
			return true;
		}
	}
	return false;
}
