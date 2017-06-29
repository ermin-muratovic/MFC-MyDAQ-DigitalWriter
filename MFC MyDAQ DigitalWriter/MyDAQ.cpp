#include "stdafx.h"
#include "MyDAQ.h"


MyDAQ::MyDAQ()
{
}


MyDAQ::~MyDAQ()
{
}

TaskHandle MyDAQ::createWriteTask(string name) {
	TaskHandle  Task = 0;

	name += "/port0/line4:7";
	const char *c_str1 = name.c_str();

	DAQmxCreateTask("", &Task);
	DAQmxCreateDOChan(Task, c_str1, "", DAQmx_Val_ChanForAllLines);

	return Task;
}

void MyDAQ::writeDO(TaskHandle Task, uInt32 input) {
	int32       written;
	DAQmxStartTask(Task);
	DAQmxWriteDigitalU32(Task, 1, 0, 10.0, DAQmx_Val_GroupByChannel, &input, &written, NULL);
	DAQmxStopTask(Task);
}