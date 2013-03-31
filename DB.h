#include "stdafx.h"
#include "winsock.h"
#include "mysql.h"

#ifndef DB_H
#define DB_H
class CMySQLDB
{
public:
	CMySQLDB();
	virtual	~CMySQLDB();

	BOOL WriteToTable(CString string);


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
