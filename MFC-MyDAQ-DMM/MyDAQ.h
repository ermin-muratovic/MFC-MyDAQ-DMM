#pragma once
#include <string>
#include <list>

using namespace std;

enum Mode {
	VDC = 1,
	VAC = 2,
	ADC = 3,
	AAC = 4,
	OHM = 5
};

class MyDAQ
{
public:
	MyDAQ();
	~MyDAQ();

	Mode mode = VDC;
	list<float> values;

	list<string> getConnectedDevices();
	list<string> splitString(std::string value, char separator, int spacing);
	float readDCVolt(string mydaq);
	float readResist(string mydaq);
	float readDCCur(string mydaq);
	void clearValues();
};

