#ifndef WB_H
#define WB_H

class WB
{
public:
	WB(int, int, int, int, int, int*);
	~WB();

	void writedata();
	int get_data();
	bool checkExit();

private:
	// controls
	int memtoreg;
	int jalC;
	int halt;

	// data
	int memdata;
	int wdata;
	int aluout;
	int dest;
	int pcplus1;
	int regwrite;
	int* reg;
};

#endif