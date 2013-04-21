#include "stdafx.h"

#ifndef PROTOCOL_H
#define PROTOCOL_H


#define BODY_LEN  128

typedef struct 
{
	CString code;
	float value;
	int valid;
}ProtocolData;


class Protocol
{
public:
	Protocol(CString name);
	virtual	~Protocol();

	virtual CString GetName() = 0;
	virtual char* ParseDataToSerialPort(char* szMsg) = 0; // convert data from protocol sub class to serial port.
	virtual void ParseDataFromSerialPort(const char* szMsg) = 0; // convert data from serial to upper protocol class.
	void AddDataToBuffer(char data);
	ProtocolData* GetProtocolData();

protected:
	CString m_name;

	char* m_buffer;
	int m_buffer_length;
	int m_buffer_index;

	ProtocolData m_data[128];
};
#endif

typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	无符号8位整型变量  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		有符号8位整型变量  */
typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
typedef float          fp32;                    /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         fp64;                    /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */