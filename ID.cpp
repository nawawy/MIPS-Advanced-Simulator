#include "ID.h"
#include <iostream>

ID::ID(int instruction, int pcplus1, int store, int* regs, int** branchAddr, stack<int>& S,
	int IDEXmemread, int IDEX_rt, int EXMEMmemread,int EXMEMrt)
{
	this->instruction = instruction;
	this->regs = regs;
	this->pcplus1 = pcplus1;
	this->store = store;
	this->branchAddr = branchAddr;
	this->S = S;
	this->IDEXmemread = IDEXmemread;
	this->IDEXrt = IDEX_rt;
	this->EXMEMmemread = EXMEMmemread;
	this->EXMEMrt = EXMEMrt;
	flush = false;
	stall1 = false;
	stall2 = false;
	setRt();
	setRd();
	setRs();
	readBoth();
	se();
	setControls();
	checkBranch(); // change pcsrc
	checkPrediction(); //check prediction and update
	checkJump();
	checkStall();
	//cout << "This is now in ID : " << this->instruction << endl;
	S = this->S;
}

void ID::checkStall()
{
	if (IDEXmemread)
	{
		if (controls != 2056)
		{
			if (IDEXrt == rt || IDEXrt == rs)
			{
				stall2 = true;
				controls = 0;
			}
		}
		else
		if (IDEXrt == rs)
		{
			stall2 = true;
			controls = 0;
		}
	}

	if (EXMEMmemread)
	{
		if (controls != 2056)
		{
			if (EXMEMrt == rt || EXMEMrt == rs)
			{
				stall1 = true;
				controls = 0;
			}
		}
		else
		if (EXMEMrt == rs)
		{
			stall1 = true;
			controls = 0;
		}
	}

}

bool ID::getStall1()
{
	return stall1;
}

bool ID::getStall2()
{
	return stall2;
}

void ID::checkPrediction()
{
	if (store != -1) // branch instruction
	{
		int state = branchAddr[store][2];

		if (pcsrc && (state == 0 || state == 1)) //true is taken and predicted is taken
		{
			address = -1; flush = false;
		}
		else if (!pcsrc && (state != 0 && state != 1)) //true is non taken and predicted is non taken
		{
			address = -1; flush = false;
		}
		else if (pcsrc && (state != 0 && state != 1)) // true is taken and predicted is non taken
		{
			address = branchAddr[store][1]; flush = true;
		}
		else if (!pcsrc && (state == 0 || state == 1)) //true is non taken and predicted is taken
		{
			address = branchAddr[store][0] + 1; flush = true;
		}
	}
	else
		address = -1;

	update();
}

bool ID::getFlush()
{
	return flush;
}
void ID::update()
{
	if (store != -1)
	{
		int move = (pcsrc == 1);
		int state = branchAddr[store][2];
		switch (state) //FSM
		{
		case 0: {if (move) state = state; else state = state + 1; break; }
		case 1: {if (move) state = state - 1; else state = state + 1; break; }
		case 2: {if (move) state = state - 1; else state = state + 1; break; }
		case 3: {if (move) state = state - 1; else state = state; break; }
		}
		branchAddr[store][2] = state;
	}
	
}
int ID::getAddress()
{
	return address;
}

void ID::checkBranch()
{
	int branch = controls >> 7 & 1;
	pcsrc = Nequal & branch;
}
void ID::setRt()
{
	int x = instruction >> 16;
	rt = x & 31; //instr[20:16]
}

void ID::setRd()
{
	int x = instruction >> 11;
	rd = x & 31; //instr[15:11]
}

void ID::setRs()
{
	int x = instruction >> 21;
	rs = x & 31; //instr[15:11]
}

int ID::getSE()
{
	return signextension;
}

int ID::get_regwrite()
{
	return regwrite;
}
/*void ID::setPCplus1(int pcplus1)
{
this->pcplus1 = pcplus1;
}*/

void ID::readBoth()
{
	int RD1 = regs[rs];
	int RD2 = regs[rt];
	if (RD1 <= RD2)
		Nequal = true;
	else
		Nequal = false;
	rd1 = RD1;
	rd2 = RD2;
}

void ID::se()
{
	int imm = instruction & 0xFFFF;
	int sign = imm >> 14;
	
	
	if (sign != 0)
	{
		int y = 0xFFFF0000;
		signextension = y | imm;
	}
	else
	{
		int y = 0x00000000;
		signextension = y | imm;
	}

}


void ID::setControls()
{
	// given array of 11 bits to control
	// regDst, regwrite, aluctl 3 bits, branch, memwrite, jump, memtoreg, alusrc, jalC, jrC , halt
	int func = instruction & 0x3F;
	unsigned int opcode = instruction >> 26;

	if (opcode == 0)
	{
		switch (func)
		{
		case 32: controls = 6144; //add
			break;
		case 38: controls = 7168; //xor
			break;
		case 42: controls = 7936; //slt
			break;
		case 8: controls = 1538; //jr
		}
	}
	else
	if (opcode == 2) controls = 32; //jr
	else if (opcode == 3) controls = 2084; //jal
	else if (opcode == 8) controls = 2056; //addi 
	else if (opcode == 35) controls = 2072; //lw
	else if (opcode == 43) controls = 72; //sw
	else if (opcode == 7) controls = 640; //bne
	else if (opcode == 63) controls = 1; //HALT (stop simulation)

	regwrite = (controls >> 11) & 1;

}

int ID::getRt()
{
	return rt;
}

int ID::getRd()
{
	return rd;
}

int ID::getRs()
{
	return rs;
}

int ID::getControls()
{
	return controls;
}

int ID::getRD1()
{
	return rd1;
}

int ID::getRD2()
{
	return rd2;
}

int ID::getMTR() // get memtoreg
{
	return ((controls >> 4) & 1);
}

int ID::getPCsrc()
{
	return pcsrc;
}

void ID::checkJump()
{
	unsigned int opc = instruction >> 26;
	unsigned int func = instruction & 63;
	//unsigned int jC = instruction >> 5 & 1;
	if (opc == 2) //j
	{
		jump = true;
		flush = true;
		address = (instruction & 67108863) - 1;
	}
	else
	{
		if (opc == 14) // jump procedure
		{
			jump = true;
			flush = true;
			int x = (instruction & 67108863);
			address = x -1;
			
			S.push(pcplus1);
		}
		else
		{
			if (opc == 15) // return procedure
			{
				//jump = true;
				
				
				if (S.size() > 0)
				{
					address = S.top();
					S.pop();
					flush = true;
				}
			}
		}
		if (opc == 3)
		{
			int dest;
			
			
			dest = 31;
			int x = (instruction & 67108863);
			address = x - 1;
			regs[dest] = pcplus1;
			flush = true;
		}
		else
		if (opc == 0 && (func == 8))
		{
			address = regs[rs];
			flush = true;
		}
	}
}