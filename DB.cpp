#include "stdafx.h"
#include "DB.h"
#include "IniReader.h"
#include <ctime>


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

void CMySQLDB::WriteProtocolData(int interval, DeviceData* data)
{

	time_t t = time(0);   // get time now
    struct tm * now = localtime(&t);

	int year = now->tm_year + 1900;
	int mon = now->tm_mon + 1;
	int day = now->tm_mday;

	int hour = now->tm_hour;
	int min = now->tm_min;
	int sec = now->tm_sec;

	CString timestamp;
	timestamp.Format("%d%02d%02d%02d%02d%02d", year, mon, day, hour, min, sec);

	CString insert_sql;
	insert_sql.Format("INSERT INTO acquisition VALUES ('%s', %d, %d, %d, %d, %d, %d, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d)", 
		timestamp, year, mon, day, hour, min, sec, interval/1000,
		data->WINDDIRECT, data->WINDDIRECTFLG, data->WINDSPEED, data->WINDSPEEDFLG, data->NOISE, data->NOISEFLG,
		data->PRESSURE, data->PRESSUREFLG, data->TEMPERATURE, data->TEMPERATUREFLG, data->HUMIDITY, data->HUMIDITYFLG,
		data->GRANDTEMPERATURE, data->GRANDTEMPERATUREFLG, data->GRANDHUMIDITY, data->GRANDHUMIDITYFLG, data->GRIPLEVEL, data->GRIPLEVELFLG,
		data->WATERPERCENT, data->WATERPERCENTFLG);
		//0.0 , 0, 0.0 , 0, 0.0 , 0, 0.0 , 0, 0.0 , 0, 0.0 , 0, 0.0 , 0, 0.0 , 0, 0.0 , 0, 0.0 , 0);
		//insert_sql.Format("INSERT INTO comm_t VALUES (%d, '%s', %f)", 1, "testname", 22.7);
	mysql_query(p_mysql, insert_sql);
}
