#include "stdafx.h"
#include "winsock.h"
#include "mysql.h"
#include "DeviceData.h"

#ifndef DB_H
#define DB_H
/* Create by CYOU, would modify if need */
class CMySQLDB
{
public:
	CMySQLDB();
	virtual	~CMySQLDB();

	BOOL WriteToTable(CString string);
	void WriteProtocolData(int interval, DeviceData* data);


private:
	MYSQL* p_mysql;

	// DB connection info.
	char *szHost;
	char *szUser; 
	char *szPwd; 
	char *szDb;   
	int  port;   
};
#endif
