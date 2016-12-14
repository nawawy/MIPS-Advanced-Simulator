#include "translator.h"

using namespace std;


translator::translator()
{
	instr = "";
	label = "";
	key = ""; 
	rd_s = "";
	rs_s = "";
	rt_s = "";

	pc = 0;
	rs = 0;
	rt = 0;
	rd = 0;
	op = 0;
	imm = 0;
	address = 0;
	syntax = true;
	instr_bin = 0;
}

translator::translator(string insr)
{
	this->instr = instr;
}

void translator::convert(string instr)
{
	int x = instr.find(":");
	
	if (x == -1)
		 key = instr.substr(0, instr.find(" "));
	else
	{
		label = instr.substr(0, x);
		string t = instr.substr(instr.find(" ") + 1);
		for (int i = 0; i < pc; ++i)
		{
			if (all_address[i] == label)
			{
				instructions[i] = instructions[i] | pc;
				break;
			}
		}
		key = t.substr(0, t.find(" "));
	}

	if (key == "add" || key == "ADD")
		Rtype(instr,1);
	else
	if (key == "addi" || key == "ADDI")
		addi(instr);
	else
	if (key == "xor" || key == "XORI")
		Rtype(instr, 2);
	else
	if (key == "lw" || key == "LW")
		lw_sw(instr, 1);
	else
	if (key == "sw" || key == "SW")
		lw_sw(instr, 2);
	else
	if (key == "ble" || key == "BLE")
		ble(instr);
	else
	if (key == "jal" || key == "JAL")
		jump(instr,1);
	else
	if (key == "jr" || key == "JR")
		Rtype(instr,4);
	else
	if (key == "j" || key == "J")
		jump(instr, 2);
	else
	if (key == "slt" || key == "SLT")
		Rtype(instr, 3);
	else
	if (key == "jump" || key == "JUMP")
	{
		string temp = instr.substr(instr.find(" ") + 1);
		imm = atoi(temp.c_str());
		op = 14;


		instr_bin = (op << 26) | (imm);;   // opcode 1110 - 14
	}
	else
	if (key == "return" || key == "RETURN")
	{
		imm = 0;
		op = 15;


		instr_bin = (op << 26) | (imm);;   // opcode 1110 - 14
	}
	else
		syntax = false;

	if (syntax)
	{
		insert(instr_bin);
		pc++;
	}

	
}

void translator::Rtype(string instr, int f)
{
	string temp;
	temp = instr.substr(instr.find(" ") + 1);
	op = 0;
	shamt = 0;


	switch (f)
	{
	case 1:
		funct = 32;
		break;
	case 2:
		funct = 38;
		break;
	case 3:
		funct = 42;
		break;
	case 4:
		funct = 8;
		rs_s = temp.substr(temp.find("$") + 1, temp.find(" "));
		rs = atoi(rs_s.c_str());
		rt = 0; rd = 0;
		instr_bin = (op << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (shamt << 6) | (funct);
		return;
	case 5:
		funct = 42;
		rd = 1;
		instr_bin = (op << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (shamt << 6) | (funct);
		return;
	}

	rd_s = temp.substr(temp.find("$") + 1, temp.find(" "));

	temp = temp.substr(temp.find(" ") + 1);

	rs_s = temp.substr(temp.find("$") + 1, temp.find(" "));

	temp = temp.substr(temp.find(" ") + 1);

	rt_s = temp.substr(temp.find("$") + 1);


	rs = atoi(rs_s.c_str());
	rt = atoi(rt_s.c_str());
	rd = atoi(rd_s.c_str());


	instr_bin = (op << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (shamt << 6) | (funct);

}

void translator::clear()
{
	instr = "";
	key = "";
	rd_s = "";
	rs_s = "";
	rt_s = "";
	imm_s = "";
	address_s = "";

	rs = 0;
	rt = 0;
	rd = 0;
	op = 0;
	shamt = 0;
	funct = 0;
	imm = 0;
	address = 0;

	syntax = true;
	instr_bin = 0;
}

void translator::addi(string instr)
{
	string temp;
	temp = instr.substr(instr.find(" ") + 1);

	op = 8;

	rt_s = temp.substr(temp.find('$') + 1 , temp.find(" "));
	
	temp = temp.substr(temp.find(" ") + 1);

	rs_s = temp.substr(temp.find('$') + 1, temp.find(" "));

	imm_s = temp.substr(temp.find(" ") + 1);

	rs = atoi(rs_s.c_str());
	rt = atoi(rt_s.c_str());
	imm = atoi(imm_s.c_str());

	instr_bin = (op << 26) | (rs << 21) | (rt << 16) | (imm);
	
}

unsigned int translator::get_instr(int i)
{
	return instructions[i];
}

void translator::insert(unsigned int in)
{
	instructions.push_back(in);
}



void translator::lw_sw(string instr, int f)
{
	string temp;
	temp = instr.substr(instr.find(" ") + 1);

	switch (f)
	{
	case 1:
		op = 35;
		break;
	case 2:
		op = 43;
		break;
	}

	rt_s = temp.substr(temp.find('$') + 1, temp.find(" "));

	temp = temp.substr(temp.find(" ") + 1);

	imm_s = temp.substr(0, temp.find('('));

	rs_s = temp.substr(temp.find('$') + 1, temp.find(')'));


	rs = atoi(rs_s.c_str());
	rt = atoi(rt_s.c_str());
	imm = atoi(imm_s.c_str());

	instr_bin = (op << 26) | (rs << 21) | (rt << 16) | (imm);

}



void translator::ble(string instr)
{
	string temp;
	temp = instr.substr(instr.find(" ") + 1);

	

	rs_s = temp.substr(temp.find('$') + 1, temp.find(" "));

	temp = temp.substr(temp.find(" ") + 1);

	rt_s = temp.substr(temp.find('$') + 1, temp.find(" "));

	imm_s = temp.substr(temp.find(" ") + 1);

	rs = atoi(rs_s.c_str());
	rt = atoi(rt_s.c_str());
	imm = atoi(imm_s.c_str());

	op = 7;
	
	
	instr_bin = (op << 26) | (rs << 21) | (rt << 16) | (imm);

}

void translator::jump(string instr, int f)
{
	switch (f)
	{
	case 1:
		op = 3;
		break;
	case 2:
		op = 2;
		break;
	}

	string temp = instr.substr(instr.find(" ") + 1);

	if (temp.find_first_not_of("0123456789") == std::string::npos)
		address = atoi(instr.substr(instr.find(" ") + 1).c_str());
	else
	{

		address_s = instr.substr(instr.find(" ") + 1);
		all_address[pc] = address_s;
	}

	instr_bin = (op << 26) | (address);



}

int translator::getsize()
{
	return pc;
}