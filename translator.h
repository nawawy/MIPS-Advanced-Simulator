#ifndef translator_h
#define translator_h
#include <iostream>
#include <string>
#include<vector>
using namespace std;

class translator
{
private:
	string instr;
	string key, label, rs_s, rt_s, rd_s;
	string imm_s;
	string address_s;
	int rs, rt, rd, shamt, funct, op;	
	int imm;		// immediate value in i-tpe instructions
	int address;	// address for j,jal which is the least 26 bits of instr
	bool syntax;	// check if syntax error is found
	unsigned int instr_bin;	// the instruction converted to binary 32 bits

	int pc;
	vector<unsigned int> instructions;
	string all_address[32];

	void Rtype(string,int);
	void addi(string);
	void lw_sw(string, int);
	void jump(string,int);
	void ble(string);

	void insert(unsigned int);
	void clear();

public:
	translator();
	translator(string);
	void convert(string);
	int getsize();
	unsigned int get_instr(int);

};
#endif
