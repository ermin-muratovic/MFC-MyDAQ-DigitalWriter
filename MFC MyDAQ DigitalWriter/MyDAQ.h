

#include <NIDAQmx.h>
#include <string>

using namespace std;

#pragma once
class MyDAQ
{
public:
	MyDAQ();	
	~MyDAQ();

	TaskHandle createWriteTask(string);
	void writeDO(TaskHandle, uInt32);
};

