#include "stdafx.h"

#ifndef DEVICEDATA_H
#define DEVICEDATA_H

class DeviceData
{
public:
	DeviceData();

	float WINDDIRECT; //'风向(度)'
	int WINDDIRECTFLG;

	float WINDSPEED; //'风速(米/秒)',
	int WINDSPEEDFLG;

	float NOISE; // '噪音(分贝)'
	int NOISEFLG;

	float PRESSURE; //'大气压(帕)'
	int PRESSUREFLG;

	float TEMPERATURE; //'温度(摄氏度)
	int TEMPERATUREFLG;

	float HUMIDITY; // '湿度(百分比)',
	int HUMIDITYFLG;

	float GRANDTEMPERATURE; //'地面温度(摄氏度)
	int GRANDTEMPERATUREFLG;

	float GRANDHUMIDITY; // 地面湿度(百分比)
	int GRANDHUMIDITYFLG;

	float GRIPLEVEL; //'地面滑溜系列(百分比)
	int GRIPLEVELFLG;

	float WATERPERCENT; //'地面含水量(毫米)',
	int WATERPERCENTFLG;


};
#endif
