#include "stdafx.h"
#include "DeviceData.h"

DeviceData::DeviceData()
{
	WINDDIRECT = 0.0; //'风向(度)'
	WINDDIRECTFLG = 0;

	WINDSPEED = 0.0; //'风速(米/秒)',
	WINDSPEEDFLG = 0;

	NOISE = 0.0; // '噪音(分贝)'
	NOISEFLG = 0;

	PRESSURE = 0.0; //'大气压(帕)'
	PRESSUREFLG = 0;

	TEMPERATURE = 0.0; //'温度(摄氏度)
	TEMPERATUREFLG = 0;

	HUMIDITY = 0.0; // '湿度(百分比)',
	HUMIDITYFLG = 0;

	GRANDTEMPERATURE = 0.0; //'地面温度(摄氏度)
	GRANDTEMPERATUREFLG = 0;

	GRANDHUMIDITY = 0.0; // 地面湿度(百分比)
	GRANDHUMIDITYFLG = 0;

	GRIPLEVEL = 0.0; //'地面滑溜系列(百分比)
	GRIPLEVELFLG = 0;

	WATERPERCENT = 0.0; //'地面含水量(毫米)',
	WATERPERCENTFLG = 0;
	
}

void DeviceData::ResetDeviceData()
{
	WINDDIRECT = 0.0; //'风向(度)'
	WINDDIRECTFLG = 0;

	WINDSPEED = 0.0; //'风速(米/秒)',
	WINDSPEEDFLG = 0;

	NOISE = 0.0; // '噪音(分贝)'
	NOISEFLG = 0;

	PRESSURE = 0.0; //'大气压(帕)'
	PRESSUREFLG = 0;

	TEMPERATURE = 0.0; //'温度(摄氏度)
	TEMPERATUREFLG = 0;

	HUMIDITY = 0.0; // '湿度(百分比)',
	HUMIDITYFLG = 0;

	GRANDTEMPERATURE = 0.0; //'地面温度(摄氏度)
	GRANDTEMPERATUREFLG = 0;

	GRANDHUMIDITY = 0.0; // 地面湿度(百分比)
	GRANDHUMIDITYFLG = 0;

	GRIPLEVEL = 0.0; //'地面滑溜系列(百分比)
	GRIPLEVELFLG = 0;

	WATERPERCENT = 0.0; //'地面含水量(毫米)',
	WATERPERCENTFLG = 0;
}