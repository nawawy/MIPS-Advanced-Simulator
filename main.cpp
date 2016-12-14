#include<fstream>
// Our classes
#include"ID.h"
#include "IF.h"
#include "EXstage.h"
#include"MEM1.h"
#include"Mem2.h"
#include"WB.h"
#include<stack>
#include "translator.h"

using namespace std;

struct IF_ID
{
	int instr;
	int pcplus1;
};

struct ID_EX
{
	int pcplus1;
	int readRegA;
	int regwriteC;
	int readRegB;
	int memtoreg;
	int offset;
	int rt;
	int rd;
	int rs;
	int controls;
};

struct EX_MEM
{
	int pcplus1;
	int memtoreg;
	int writereg;
	int regwriteC;
	int zero;
	int aluResult;
	int writedata;
	int controls;
};

struct MEM_WB
{
	//int instr;
	int wb_out;
	int dataAlu;
	int regwriteC;
	int dataMemory;
	int pcplus1;
	int writereg;
	int controls;
};

struct MEM1_Mem2 // buffer between memory 1 and memory 2
{
	int pcplus1;
	int address;
	int writereg;
	int controls;
	int regwriteC;
	int data;
};

struct WB_END
{
	int writedata;
	int regwrite;
	int rd;
};

struct state
{
	int pc;
	IF_ID IFID;
	ID_EX IDEX;
	EX_MEM EXMEM;
	MEM1_Mem2 MEM1Mem2;
	MEM_WB MEMWB;
	WB_END WBEND;
	int cycles;
	int pcsrc;
	int store;
	int address;
	bool flush;
	bool stall2;
	bool stall1;
};

int cycles;
int *imem;
vector<string> aa;
int *dmem;
int n = 0;
int *regs;
int **branchAddr;
void simulate();
void branchSetup();

stack <int> S;

void initialize(state& c)
{
	c.IFID.pcplus1 = -1;
	c.IDEX.pcplus1 = -1;
	c.EXMEM.pcplus1 = -1;
	c.MEM1Mem2.pcplus1 = -1;
	c.MEMWB.pcplus1 = -1;
}

void output()
{
	cout << "THIS IS THE TABLE !!! \n";
	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cout << branchAddr[0][j] << endl;
		}
	}
}

int main()
{
	ifstream input;
	input.open("text.txt");

	imem = new int[32];  // instruction memory
	dmem = new int[16];  // data memory
	regs = new int[16];  // register memory
	branchAddr = new int*[32]; //branchadress table

	branchSetup();
	regs[0] = 0; regs[4] = 4; regs[5] = 5;
	dmem[5] = 23;

	translator translate;

	while (!input.eof())
	{
		string x;
		getline(input, x);
		aa.push_back(x);
		translate.convert(x);
	}


	int i = 0; int j = 0;
	for (i = 0; i < translate.getsize(); i++)
	{
		unsigned int y = translate.get_instr(i);
		imem[i] = y;
		n++;
		unsigned int opcod = y >> 26;
		int im = y & 0xFFFF;
		cout << "at pc of " << i << " we have instr " << y << endl;
		if (opcod == 7)
		{
			//cout << " <<< THIS IS BEQ INSTRUCTION \n";//st column of branch address table with the branch pcs
			branchAddr[j][0] = i;
			branchAddr[j][1] = i + im;
			branchAddr[j][2] = 0;
			//cout << branchAddr[0][0] << "  " << branchAddr[0][1] << "  " << branchAddr[0][2] << " >>>>>" << endl;
			j++;
		}
	}
	
	unsigned int halt = (63 << 26);  // Hlt instruction (111111000....00) == 4227858432(decimal)
	imem[i] = halt; //-67108864

	for (int i = 0; i < 5; i++)
		cout << "At PC: "<< i << " " << imem[i] << endl;
	simulate();
	output();
	for (int i = 0; i < 16; i++)
		cout << "Register " << i << "   : "  << regs[i] << endl;
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
	for (int i = 0; i < 16; i++)
		cout << "Memory Location " << i << "   : " << dmem[i] << endl;



	system("pause");
	return 0;
}


void simulate()
{
	state current, next;
	initialize(current); // set everything to -1 
	current.pc = 0;
	current.cycles = 0;
	current.address = -1;
	current.pcsrc = 0;
	current.flush = false;
	current.stall1 = false;
	current.stall2 = false;
	int count = 0;
	while (1)
	{
		next = current;
		count++;
		cout << "Current PC: " << current.pc << endl;
	
		IF IF_stage(imem, current.pc, current.pcsrc, current.address, current.stall1, current.stall2,branchAddr);
		next.pc = next.IFID.pcplus1 = IF_stage.getPcNext();
		if (!current.flush)
			next.IFID.instr = IF_stage.getinstruction();
		else next.IFID.instr = 3435973836;
		next.store = IF_stage.getStore();
		

		if (current.IFID.pcplus1 != -1)
		{
			ID ID_stage(current.IFID.instr, current.IFID.pcplus1,current.store, regs,branchAddr,S, 
			current.IDEX.memtoreg, current.IDEX.rt,current.EXMEM.memtoreg, current.EXMEM.writereg);
			next.pcsrc = ID_stage.getPCsrc();
			next.address = ID_stage.getAddress();
			next.flush = ID_stage.getFlush();
			if (!current.flush)
			{
				next.IDEX.readRegA = ID_stage.getRD1();
				next.IDEX.readRegB = ID_stage.getRD2();
				next.IDEX.offset = ID_stage.getSE();
				next.IDEX.controls = ID_stage.getControls();
				next.IDEX.rd = ID_stage.getRd();
				next.IDEX.rt = ID_stage.getRt();
				next.IDEX.rs = ID_stage.getRs();
				next.IDEX.regwriteC = ID_stage.get_regwrite();
				next.IDEX.memtoreg = ID_stage.getMTR();
				next.IDEX.pcplus1 = current.IFID.pcplus1;
				next.stall1 = ID_stage.getStall1();
				next.stall2 = ID_stage.getStall2();
			}
			else
			{
				next.IDEX.readRegA = -1;
				next.IDEX.readRegB = -1;
				next.IDEX.offset = -1;
				next.IDEX.controls = 0;
				next.IDEX.rd = -1;
				next.IDEX.rt = -1;
				next.IDEX.rs = -1;
				next.IDEX.pcplus1 = -1;
			}
		
			if (next.stall1 || next.stall2)
				next.IFID = current.IFID;
			
		}

		if (current.IDEX.pcplus1 != -1)
		{
			
			EXstage EX_stage(current.IDEX.offset, current.IDEX.pcplus1, current.IDEX.controls,
			current.IDEX.readRegA, current.IDEX.readRegB, current.IDEX.rt, current.IDEX.rd, current.IDEX.rs,
			current.EXMEM.aluResult, current.EXMEM.regwriteC, current.EXMEM.writereg, current.MEMWB.regwriteC,
			current.MEMWB.writereg, current.MEMWB.wb_out, current.WBEND.regwrite, current.WBEND.rd, current.WBEND.writedata,
			current.MEM1Mem2.regwriteC, current.MEM1Mem2.writereg, current.MEM1Mem2.address,
			current.IDEX.memtoreg, current.IDEX.rs, current.EXMEM.writereg, current.MEMWB.dataMemory, current.stall1);


			next.EXMEM.aluResult = EX_stage.getAluResult();
			next.EXMEM.zero = EX_stage.zeroFlag();
			next.EXMEM.writedata = EX_stage.getRD2();
			next.EXMEM.writereg = EX_stage.getWriteReg();
			next.EXMEM.regwriteC = current.IDEX.regwriteC;
			next.EXMEM.pcplus1 = current.IDEX.pcplus1;
			next.EXMEM.controls = EX_stage.getControls();
			next.EXMEM.memtoreg = current.IDEX.memtoreg;
			
		}

		if (current.EXMEM.pcplus1 != -1)
		{
			// the first block of memory which stores only (for now)
			MEM1 M1(current.EXMEM.pcplus1, current.EXMEM.aluResult, current.EXMEM.writedata, 
				current.EXMEM.writereg, current.EXMEM.controls);

			next.MEM1Mem2.address = M1.getAddress();
			next.MEM1Mem2.pcplus1 = current.EXMEM.pcplus1;
			next.MEM1Mem2.writereg = current.EXMEM.writereg;
			next.MEM1Mem2.controls = M1.getControls();
			next.MEM1Mem2.regwriteC = current.EXMEM.regwriteC;
			next.MEM1Mem2.data = M1.getData();
		}

		if (current.MEM1Mem2.pcplus1 != -1)
		{
			//second block of memory which loads only
			MEM2 M2(current.MEM1Mem2.data, current.MEM1Mem2.address, current.MEM1Mem2.pcplus1, 
				current.MEM1Mem2.writereg, current.MEM1Mem2.controls, dmem);
			next.MEMWB.dataAlu = M2.getAddress();
			next.MEMWB.dataMemory = M2.getLoadData();
			next.MEMWB.pcplus1 = M2.getPcplus1();
			next.MEMWB.writereg = current.MEM1Mem2.writereg;
			next.MEMWB.controls = M2.getControls();
			if (next.MEMWB.controls == 2072)
				next.MEMWB.wb_out = next.MEMWB.dataMemory;
			else
				next.MEMWB.wb_out = next.MEMWB.dataAlu;

			next.MEMWB.regwriteC = current.MEM1Mem2.regwriteC;
		}

		if (current.MEMWB.pcplus1 != -1)
			//here I need to put the last stage of WB
		{
			WB WB_stage(current.MEMWB.dataAlu, 
			current.MEMWB.pcplus1, current.MEMWB.dataMemory, current.MEMWB.writereg, current.MEMWB.controls, regs);
			next.WBEND.writedata = WB_stage.get_data();
			next.WBEND.regwrite = current.MEMWB.regwriteC;
			next.WBEND.rd = current.MEMWB.writereg;
			if (WB_stage.checkExit())
			{
				cout << "Simulation finished \n";
				break;
			}
		}


		current = next;
	//	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< " << aa[(current.IFID.pcplus1 )%n] << endl;
	}
	cout << "Number of cycles " << count << endl;
}

void branchSetup()
{

	for (int m = 0; m < 32; m++)
		branchAddr[m] = new int[3];

	for (int a = 0; a < 32; a++)
	{
		for (int b = 0; b < 3; b++)
			branchAddr[a][b] = -100;
	}
}



