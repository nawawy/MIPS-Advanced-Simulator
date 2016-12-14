#include"WB.h"

WB::WB(int aluout, int pcplus1, int memdata, int dest, int controls, int*reg)
{

	this->memtoreg = controls >> 4 & 1;
	this->jalC = controls >> 2 & 1;
	this->memdata = memdata;
	this->aluout = aluout;
	this->dest = dest;
	this->pcplus1 = pcplus1;
	this->reg = reg;
	this->halt = controls & 1;
	this->regwrite = (controls >> 11) & 1;
	writedata();
}

WB::~WB()
{

}

void WB::writedata()
{
	if (memtoreg)
		wdata = memdata;
	else
		wdata = aluout;

	if (jalC)
	{
		wdata = pcplus1;
		dest = 31;
	}
	
	if (regwrite)
		reg[dest] = wdata;
}

int WB::get_data()
{
	return wdata;
}

bool WB::checkExit()
{
	return (halt ? true: false);
}
