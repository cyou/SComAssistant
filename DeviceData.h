#include "stdafx.h"

#ifndef DEVICEDATA_H
#define DEVICEDATA_H

class DeviceData
{
public:
	DeviceData();

	float WINDDIRECT; //'����(��)'
	int WINDDIRECTFLG;

	float WINDSPEED; //'����(��/��)',
	int WINDSPEEDFLG;

	float NOISE; // '����(�ֱ�)'
	int NOISEFLG;

	float PRESSURE; //'����ѹ(��)'
	int PRESSUREFLG;

	float TEMPERATURE; //'�¶�(���϶�)
	int TEMPERATUREFLG;

	float HUMIDITY; // 'ʪ��(�ٷֱ�)',
	int HUMIDITYFLG;

	float GRANDTEMPERATURE; //'�����¶�(���϶�)
	int GRANDTEMPERATUREFLG;

	float GRANDHUMIDITY; // ����ʪ��(�ٷֱ�)
	int GRANDHUMIDITYFLG;

	float GRIPLEVEL; //'���滬��ϵ��(�ٷֱ�)
	int GRIPLEVELFLG;

	float WATERPERCENT; //'���溬ˮ��(����)',
	int WATERPERCENTFLG;


};
#endif
