#include "MEM2.h"

MEM2::MEM2(int data, int address, int pcplus1, int writereg, int controls, int *t)
{
	this->address = address;
	this->pcplus1 = pcplus1;
	this->writereg = writereg;
	this->controls = controls;
	this->writeMEM = controls >> 6 & 1;
	this->data = data;
	this->mem = t;
	storeWord();
}

void MEM2::storeWord()
{
	if (writeMEM)
		mem[address] = data;

	
} 

int MEM2::getPcplus1()
{
	return pcplus1;
}

int MEM2::getWR()
{
	return writereg;
}

int MEM2::getLoadData()
{
	if (address < 16 && address > 0)
		return mem[address];

	return -100000;
}

int MEM2::getAddress()
{
	return address;
}

int MEM2::getControls()
{
	return controls;
}

MEM2::~MEM2()
{

}

/*void MEM2::setMEMData(int data)
{
mem[address] = data;
}*/

/*int* MEM2::getArray()
{
return mem;
}*/
