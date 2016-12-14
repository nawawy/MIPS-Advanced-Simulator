#include "MEM1.h"

MEM1::MEM1(int pcplus1, int address, int data, int writereg, int controls)
{
	this->address = address;
	this->data = data;
	this->pcplus1 = pcplus1;
	this->writereg = writereg;
	this->controls = controls;
	//this->memwrite = controls >> 6 & 1;

	//setData();
}

//void MEM1::setData()
//{
//	if (memwrite)
//		mem[address] = data;
//}

int MEM1::getData()
{
	return data;
}
int MEM1::getAddress()
{
	return address;
}

int MEM1::getPcplus1()
{
	return pcplus1;
}

int MEM1::getWR()
{
	return writereg;
}
int MEM1::getControls()
{
	return controls;
}
/*int* MEM1::getArray()
{
return mem;
}*/

MEM1::~MEM1()
{

}