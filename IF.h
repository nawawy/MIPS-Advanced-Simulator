#ifndef IF_H
#define IF_H


class IF
{
public:

	IF( int*, int,int,int,bool,bool,int**);		// Constructor
	~IF();						// Destructor

	int getinstruction();
	//int getpcplus();
	int getPcNext();
	int getStore();
	void checkBranch();

private:

	int* imem;
	int **branchaddr;
	int pc;
	bool stall1; bool stall2;
	int address;
	int pcsrc;
	int store;
};
#endif 

