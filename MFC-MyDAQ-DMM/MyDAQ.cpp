#include "stdafx.h"
#include "MyDAQ.h"
#include <NIDAQmx.h>
#include <string>
#include <list>

using namespace std;

MyDAQ::MyDAQ()
{
}


MyDAQ::~MyDAQ()
{
}


list<string> MyDAQ::getConnectedDevices()
{
	char namesBuffer[1000] = { '\0' };
	DAQmxGetSysDevNames(namesBuffer, 1000);
	string names(namesBuffer);
	list<string> devices = splitString(names, ',', 2);
	return devices;
}

list<string> MyDAQ::splitString(string value, char separator, int spacing)
{
	list<string> values;
	if (value.length() > 0) {
		size_t found = value.find_first_of(separator);
		while (found != string::npos)
		{
			values.push_back(value.substr(0, found));
			value = value.substr(found + spacing);
			found = value.find_first_of(separator);
		}
		values.push_back(value);
	}
	return values;
}


float MyDAQ::readDCVolt(string mydaq)
{
	TaskHandle  Task = 0;
	int32       read;
	float64 value = 0;

	mydaq += "/dmm";
	const char *c_str1 = mydaq.c_str();

	DAQmxCreateTask("", &Task);
	DAQmxCreateAIVoltageChan(Task, c_str1, "", DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL);
	DAQmxStartTask(Task);
	DAQmxReadAnalogF64(Task, 1, 10.0, DAQmx_Val_GroupByChannel, &value, 1, &read, NULL);
	DAQmxStopTask(Task);
	DAQmxClearTask(Task);

	values.push_back(value);
	return (float)value;
}


float MyDAQ::readResist(string mydaq)
{
	TaskHandle  Task = 0;
	int32       read;
	float64 value = NAN;

	mydaq += "/dmm";
	const char *c_str1 = mydaq.c_str();

	int upperRange[4] = { 1000, 10000, 100000, 1000000 };

	for (int i = 0; i < 4 && isnan(value); i++) {
		DAQmxCreateTask("", &Task);
		DAQmxCreateAIResistanceChan(Task, c_str1, "", 100, upperRange[i], DAQmx_Val_Ohms, DAQmx_Val_2Wire, DAQmx_Val_Internal, 0.001, NULL);
		DAQmxStartTask(Task);
		DAQmxReadAnalogF64(Task, 1, 10.0, DAQmx_Val_GroupByChannel, &value, 1, &read, NULL);
		DAQmxStopTask(Task);
		DAQmxClearTask(Task);
	}

	values.push_back(value);
	return (float)value;
}


float MyDAQ::readDCCur(string mydaq)
{
	TaskHandle  Task = 0;
	int32       read;
	float64 value = 0;

	mydaq += "/dmm";
	const char *c_str1 = mydaq.c_str();

	DAQmxCreateTask("", &Task);
	DAQmxCreateAICurrentChan(Task, c_str1, "", DAQmx_Val_Cfg_Default, 0.0, 1.0, DAQmx_Val_Amps, DAQmx_Val_Default, NULL, NULL);
	DAQmxStartTask(Task);
	DAQmxReadAnalogF64(Task, -1, 10.0, DAQmx_Val_GroupByChannel, &value, 1, &read, NULL);
	DAQmxStopTask(Task);
	DAQmxClearTask(Task);

	values.push_back(value);
	return (float)value;
}


void MyDAQ::clearValues()
{
	values.clear();
}
