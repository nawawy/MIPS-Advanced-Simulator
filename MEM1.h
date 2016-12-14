#ifndef MEM1_H
#define MEM1_H

class MEM1
{
public:
	MEM1(int, int, int, int, int);
	int getAddress();
	//void setData();
	//int* getArray();
	int getPcplus1();
	int getControls();
	int getData();
	int getWR(); // get write reg
	~MEM1();

private:
	int address;
	int pcplus1;
	int writereg;
	int controls;
	int memwrite;
	int data;
};

#endif