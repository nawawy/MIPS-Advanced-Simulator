#ifndef ID_H
#define ID_H
#include "regsblock.h"
#include <stack>
#include <bitset>
#include <string>
class ID
{
public:
	ID(int, int, int, int*, int**, stack <int>&, int, int, int, int);
	void setRt();
	int getRt();
	int getRd();
	int getRs();
	int getSE();
	int getControls();
	int getRD1();
	int getRD2();
	int getPCsrc();
	int getAddress();
	int get_regwrite();
	int getMTR();
	bool getFlush();
	bool getStall1();
	bool getStall2();
	void checkBranch();
	void update();
	void checkJump();
	void setRd();
	void setRs();
	void setPCplus1(int);
	void readBoth();
	void se();
	void checkStall();
	void checkPrediction();
	//void writeregister(int regDst, int writedata);
	//int checkType(int&);
	void setControls();


private:
	int rd1, rd2;
	int rs, rd, rt;
	int signextension;
	int controls;
	int pcplus1;
	int pcsrc;
	int branch;
	int address;
	int store;
	int IDEXmemread;
	int IDEXrt;
	int EXMEMmemread;
	int EXMEMrt;
	int regwrite;
	bool jump;
	//int storeback;
	bool flush;
	bool stall1;
	bool stall2;
	bool Nequal; // to check of RD1 != RD2
	unsigned int instruction;
	int* regs;
	int **branchAddr;
	stack <int> S;
};
#endif