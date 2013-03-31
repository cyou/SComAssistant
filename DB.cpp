#include "stdafx.h"
#include "DB.h"
#include "IniReader.h"


#define CFG ".\\config.ini"

CMySQLDB::CMySQLDB()
{
	
	// Read database configuration from ini file.
	CIniReader iniReader(CFG);
	szHost = iniReader.ReadString("DB", "host", "localhost");
	szUser = iniReader.ReadString("DB", "user", "root");
	szPwd  = iniReader.ReadString("DB", "password", "root");
	szDb   = iniReader.ReadString("DB", "database", "test");
	int  port    = iniReader.ReadInteger("DB", "port", 3306); 
	
	// Init database connection.
	p_mysql = mysql_init(NULL);
	if (!mysql_real_connect(p_mysql,NULL,szUser,szPwd,szDb,port,NULL,0))
	{
		AfxMessageBox("fail to connect to mysql DB, please check your configuration.");
	}
}

CMySQLDB::~CMySQLDB()
{
	// Close DB collection
	if (p_mysql)
	{
		mysql_close(p_mysql);
	}
	
	delete szHost;
	delete szUser;
	delete szPwd;
	delete szDb;
}

BOOL CMySQLDB::WriteToTable(CString data)
{
	// This is demo code that shows how to insert data into DB.
	for (int i = 0; i < 100; i++)
	{
		CString insert_sql;
		insert_sql.Format("INSERT INTO comm_t VALUES (%d, '%s', %f)", i, "testname", 22.7);
		mysql_query(p_mysql, insert_sql);
	}
	return true;
}
