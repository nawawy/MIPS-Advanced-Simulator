#ifndef EXstage_H
#define EXstage_H
#include<string>
using namespace std;

class EXstage
{
public:
	/*void readBoth(int instruction, int&r1, int &r2);
	void se(int instruction, int&see);
	void writeregister(int instruction, int regDst, int writedata);*/

	EXstage(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int,bool);
	//int getBranchAddress();
	bool zeroFlag();
	int getRD2();
	int getAluResult();
	//int getWriteData();
	int getWriteReg();
	int getPcplus4();
	int getControls();
	int getRegwrite();
	bool get_stall();

private:
	int IDEX_memtoreg;
	int IFID_rt;
	int IFID_rs;
	int signimm;
	int pcplus4;
	int WBEND_regwrite;
	int WBEND_rd;
	int WBEND_out;
	int MEM12_regwrite;
	int MEM12_rd;
	int MEM12_out;
	int alucontrol;
	int controls;
	int MEMWB_datamemory;
	int Alusrc;
	int EXMEM_out;
	int regwriteC;
	bool zero;
	int EXMEM_rd;
	bool stall;
	int MEMWB_regwrite;
	int MEMWB_rd;
	int RD1;
	int RD2;
	int memdata;
	int ALUoutput;
	int rt, rd, rs;
	int regdst;
};
#endif
