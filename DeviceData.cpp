#include "stdafx.h"
#include "DeviceData.h"

DeviceData::DeviceData()
{
	WINDDIRECT = 0.0; //'����(��)'
	WINDDIRECTFLG = 0;

	WINDSPEED = 0.0; //'����(��/��)',
	WINDSPEEDFLG = 0;

	NOISE = 0.0; // '����(�ֱ�)'
	NOISEFLG = 0;

	PRESSURE = 0.0; //'����ѹ(��)'
	PRESSUREFLG = 0;

	TEMPERATURE = 0.0; //'�¶�(���϶�)
	TEMPERATUREFLG = 0;

	HUMIDITY = 0.0; // 'ʪ��(�ٷֱ�)',
	HUMIDITYFLG = 0;

	GRANDTEMPERATURE = 0.0; //'�����¶�(���϶�)
	GRANDTEMPERATUREFLG = 0;

	GRANDHUMIDITY = 0.0; // ����ʪ��(�ٷֱ�)
	GRANDHUMIDITYFLG = 0;

	GRIPLEVEL = 0.0; //'���滬��ϵ��(�ٷֱ�)
	GRIPLEVELFLG = 0;

	WATERPERCENT = 0.0; //'���溬ˮ��(����)',
	WATERPERCENTFLG = 0;
	
}

void DeviceData::ResetDeviceData()
{
	WINDDIRECT = 0.0; //'����(��)'
	WINDDIRECTFLG = 0;

	WINDSPEED = 0.0; //'����(��/��)',
	WINDSPEEDFLG = 0;

	NOISE = 0.0; // '����(�ֱ�)'
	NOISEFLG = 0;

	PRESSURE = 0.0; //'����ѹ(��)'
	PRESSUREFLG = 0;

	TEMPERATURE = 0.0; //'�¶�(���϶�)
	TEMPERATUREFLG = 0;

	HUMIDITY = 0.0; // 'ʪ��(�ٷֱ�)',
	HUMIDITYFLG = 0;

	GRANDTEMPERATURE = 0.0; //'�����¶�(���϶�)
	GRANDTEMPERATUREFLG = 0;

	GRANDHUMIDITY = 0.0; // ����ʪ��(�ٷֱ�)
	GRANDHUMIDITYFLG = 0;

	GRIPLEVEL = 0.0; //'���滬��ϵ��(�ٷֱ�)
	GRIPLEVELFLG = 0;

	WATERPERCENT = 0.0; //'���溬ˮ��(����)',
	WATERPERCENTFLG = 0;
}