
#ifndef __TRAJECTORY_H__
#define __TRAJECTORY_H__

//Direction
#define N		1
#define S		2
#define E		3
#define O		4
#define NE		5
#define NO		6
#define SE		7
#define SO		8

//Direction (no trivial case)
#define C1	   11
#define C2	   12
#define C3	   13
#define C4	   14
#define C5	   15
#define C6	   16
#define C7	   17
#define C8	   18

//Going
#define STOP		0
#define CONTINUE	1
#define ARRIVE		2

//Step length
#define STEP_LENGTH	2	//Posible values = {1,2,4,8,16,32}

class cTrajectory  
{
public:
	cTrajectory();
	virtual ~cTrajectory();

	void Make(int cx,int cy,int cxdest,int cydest);	//Make new trajectory
	void ReMake(int cxdest,int cydest);				//Make new trajectory overlapping previous
	int  NextStep(int *px,int *py,int *cx,int *cy);	//Calculate next step position
	int  NextCell();								//Calculate next cell (subdir)

	int  Faced();
	bool IsDone();
	void Done();

private:

	int x,y;	//actual cell
	int xf,yf;	//objective cell
	int nxf,nyf;//new objective cell (superposed case)
	int dir;	//direction
	int subdir;	//subdirection (cell to next cell)

	//Midpoint algorithm (Bresenham's) variables
	//+info: http://gamedev.cs.colorado.edu/tutorials/Bresenham.pdf
	int p;		//next error
	int k1,k2;	//constants
};

#endif
