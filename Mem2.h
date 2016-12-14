#ifndef MEM2_H
#define MEM2_H

class MEM2
{
public:
	MEM2(int, int, int, int, int, int*);
	int getWR(); //here we get write register 
	int getLoadData(); // here we get the load data of lw function
	int getPcplus1();
	int getAddress();
	int getControls();
	void storeWord();
	~MEM2();

	//void setMEMData(int data);
	//int getData();
	//int* getArray();
private:
	int address;
	int data; int controls;
	int pcplus1;
	int writereg;
	int* mem;
	int writeMEM;
};

#endif