
#ifndef __ASTAR_H__
#define __ASTAR_H__

#define mapWidth		25
#define mapHeight		19
#define numberPeople	3

class cAStar  
{
private:
	//Declare constants
	int tileSize;
	int onClosedList;
	int notfinished, notStarted;// path-related constants
	int found, nonexistent; 
	int walkable, unwalkable;// walkability array constants

	//Create needed arrays
	char walkability [mapWidth][mapHeight];
	int openList[mapWidth*mapHeight+2]; //1 dimensional array holding ID# of open list items
	int whichList[mapWidth+1][mapHeight+1];  //2 dimensional array used to record 
	//whether a cell is on the open list or on the closed list.
	int openX[mapWidth*mapHeight+2]; //1d array stores the x location of an item on the open list
	int openY[mapWidth*mapHeight+2]; //1d array stores the y location of an item on the open list
	int parentX[mapWidth+1][mapHeight+1]; //2d array to store parent of each cell (x)
	int parentY[mapWidth+1][mapHeight+1]; //2d array to store parent of each cell (y)
	int Fcost[mapWidth*mapHeight+2];	//1d array to store F cost of a cell on the open list
	int Gcost[mapWidth+1][mapHeight+1]; 	//2d array to store G cost for each cell.
	int Hcost[mapWidth*mapHeight+2];	//1d array to store H cost of a cell on the open list
	int pathLength[numberPeople+1];     //stores length of the found path for critter
	int pathLocation[numberPeople+1];   //stores current position along the chosen path for critter		
	int *pathBank [numberPeople+1];

	//Path reading variables
	int pathStatus[numberPeople+1];
	int xPath[numberPeople+1];
	int yPath[numberPeople+1];

public:
	cAStar();
	virtual ~cAStar();

	void InitializePathfinder(void);
	void EndPathfinder(void);
	int  FindPath (int pathfinderID,int startingX, int startingY, int targetX, int targetY);
	void ReadPath(int pathfinderID,int currentX,int currentY, int pixelsPerFrame);
	int  ReadPathX(int pathfinderID,int pathLocation);
	int  ReadPathY(int pathfinderID,int pathLocation);

/////////////////////////////////
	void LoadMap(int *map);
	void PrintPath();
	void NextCell(int *cx,int *cy);
	
private:
	int idxStep;
/////////////////////////////////
};

#endif
