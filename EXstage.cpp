#include "EXstage.h"

EXstage::EXstage(int signimm, int pcplus4, int controls, int rd1, int rd2, int rt, int rd, int rs,
	int EXMEM_out, int rwc, int writereg, int MEMWB_regwrite, int MEMWB_rd, int memdata,
	int WBEND_regwrite, int WBEND_rd, int WBEND_out, int MEM12_regwrite, int MEM12_rd, int MEM12_out,
	int memtoreg, int IFID_rs, int IFID_rt, int MEMWB_datamemory, bool stall)
{
	this->pcplus4 = pcplus4;
	this->signimm = signimm;
	this->controls = controls;
	this->RD1 = rd1;
	this->RD2 = rd2;
	this->rt = rt; //rt
	this->rd = rd; //rd
	this->rs = rs;
	this->EXMEM_out = EXMEM_out;
	this->regdst = controls >> 12;
	this->Alusrc = controls >> 3 & 1;
	this->alucontrol = (controls >> 8) & 7;
	this->regwriteC = rwc;
	this->EXMEM_rd = writereg;
	this->MEMWB_rd = MEMWB_rd;
	this->MEMWB_regwrite = MEMWB_regwrite;
	this->memdata = memdata;
	this->WBEND_regwrite = WBEND_regwrite;
	this->WBEND_rd = WBEND_rd;
	this->WBEND_out = WBEND_out;
	this->MEM12_regwrite = MEM12_regwrite;
	this->MEM12_rd = MEM12_rd;
	this->MEM12_out = MEM12_out;
	this->IDEX_memtoreg = memtoreg;
	this->IFID_rs = IFID_rs;
	this->IFID_rt = IFID_rt;
	this->MEMWB_datamemory = MEMWB_datamemory;
	this->stall = stall;
}

int EXstage::getRD2()
{
	return RD2;
}

int EXstage::getAluResult()
{
	int srcb, srca;
	bool EXhazard1 = false, EXhazard2 = false, MEM1hazard1 = false, MEM1hazard2 = false, MEM2hazard1 = false, MEM2hazard2 = false;
	stall = false;
	srca = RD1;

	if (Alusrc)
		srcb = signimm;
	else
		srcb = RD2;

	if (regwriteC)             // forwarding
	{
		if (rs == EXMEM_rd)
		{
			EXhazard1 = true;
			srca = EXMEM_out;
		}

		if (controls == 72)
		if (EXMEM_rd == rt)
		{
			RD2 = EXMEM_out;
			EXhazard2 = true;
		}

		if (controls != 2056 && controls != 72)
			if (rt == EXMEM_rd)
			{
				EXhazard2 = true;
				srcb = EXMEM_out;
			}

		
	}

	if (MEM12_regwrite)
	{
		if (MEM12_rd == rs && !EXhazard1)
		{
			MEM1hazard1 = true;
			srca = MEM12_out;
		}

	

		if (controls != 2056 && controls != 72)
		if (MEM12_rd == rt && !EXhazard2)
			{
				srcb = MEM12_out;
				MEM1hazard2 = true;
			}


		

		if (controls == 72)
		if (MEM12_rd == rt && !EXhazard2)
		{
			RD2 = MEM12_out;
			MEM1hazard2 = true;

		}
	}

	if ((MEMWB_regwrite)   && !stall)
	{
		if (MEMWB_rd == rs && !EXhazard1 && !MEM1hazard1)
		{
			MEM2hazard1 = true;
			srca = memdata;
		}

		if (controls != 2056 && controls != 72)
		if (MEMWB_rd == rt && !EXhazard2 && !MEM1hazard2)
			{
				MEM2hazard2 = true;
				srcb = memdata;
			}

			if (controls == 72)
			if (MEMWB_rd == rt && !EXhazard2 && !MEM1hazard2)
			{
				RD2 = memdata;
				MEM2hazard2 = true;
			}
	}

	

	if (WBEND_regwrite )
	{
		

		if (WBEND_rd == rs &&!EXhazard1 && !MEM1hazard1  && !MEM2hazard1)
		{
			srca = WBEND_out;
		}


		if (controls != 2056 && controls != 72)
		if (WBEND_rd == rt &&!EXhazard2 && !MEM1hazard2  && !MEM2hazard2)
		{
			srcb = WBEND_out;
		}

		if (controls == 72)
		if (WBEND_rd == rt &&!EXhazard2 && !MEM1hazard2  && !MEM2hazard2)
			RD2 = WBEND_out;


		
	}

	

	int output;

	switch (alucontrol)
	{
	case(6) :
		output = srca;
		break;
	case(0) :
		output = srca + srcb;
		break;
	case(2) :
		output = srca - srcb;
		break;
	case(4) :
		output = srca ^ srcb;
		break;
	case(7) :
		if (srca < srcb)
			output = 1;
		else
			output = 0;

		break;
	}

	zero = (output == 0);
	return output;
}

bool EXstage::get_stall()
{
	return stall;
}


int EXstage::getWriteReg()
{
	int WR;

	
	if (regdst)
		WR = rd;   // RD
	else
		WR = rt;   // RT 

	if (WR != 0)   //since you can't write to reg 0, return trash or -1
		return WR;
	else
		return -1;
}

int EXstage::getRegwrite()
{
	return ((controls << 11) & 1);
}

int EXstage::getPcplus4()
{
	return pcplus4;
}
bool EXstage::zeroFlag()
{
	return zero;
}

int EXstage::getControls()
{
	return controls;
}
