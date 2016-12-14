#include"IF.h"
#include <iostream>
using namespace std;

// Constructor with argument, size is nelements, default is 11
IF::IF(int* i, int pc, int pcsrc, int address, bool stall1, bool stall2, int** branchaddr)
{
	this->imem = i;
	this->pc = pc;
	this->branchaddr = branchaddr;
	this->pcsrc = pcsrc;
	this->store = -1;
	this-> address = address;
	this->stall1 = stall1;
	this->stall2 = stall2;
	checkBranch();
}


// Destructor
IF::~IF()
{

}

void IF::checkBranch()
{
	for (int i = 0; i < 32; i++)
	if (pc == branchaddr[i][0])
		store = i;		
}

// Gets the instruction word
int IF::getinstruction()
{
	if (!this->stall1 && !this->stall2)
		return imem[pc];
	else
		return imem[pc - 1];
}

int IF::getStore()
{
	return store;
}

int IF::getPcNext()
{
	if (!this->stall1 && !this->stall2)
	{
		if (store != -1)						//branch
		{
			if (branchaddr[store][2] == 0 || branchaddr[store][2] == 1)
				return branchaddr[store][1];
			else return (this->pc + 1);
		}
		else
		{
			if (address == -1)
				return (this->pc + 1);
			else
				return  address;
		}
	}
	else
		return pc;
}