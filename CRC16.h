#include "stdafx.h"
#include "Protocol.h"
#include "type.h"

#ifndef CRC16_H
#define CRC16_H

class CRC16 
{
private:
	// 低字节CRC值表
	static uint8  auchCRCLo[];  
	// 高字节CRC值表
	static uint8 auchCRCHi[];
	uint8 m_crc_hi;
	uint8 m_crc_lo;

	
//	CRC16();
//	CRC16(CRC16 const&);
//	~CRC16();
	CRC16& operator=(CRC16 const&);

public:
	static CRC16& instance()
	{
		static CRC16 crc;
		return crc;
	}
	void Modbus_CRC16(uint8 *Buff_addr,uint16 len);
	uint8 CRC16_Hi();
	uint8 CRC16_Lo();
};
#endif
