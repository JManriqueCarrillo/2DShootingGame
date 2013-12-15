
#ifndef __SKELETON_H__
#define __SKELETON_H__

#include <windows.h>
#include "cPath.h"
#include <list>

class cScene;

class cSkeleton
{
public:
	cSkeleton(void);
	virtual ~cSkeleton(void);

	void cSkeleton::SetEnemy ( int id, int newx, int newy, int newhealth);

	//Enemy Move
	void GoToCell(int *map,int destpx,int destpy);
	void GoToPlayer(int *map,int destpx,int destpy);
	void Move();

	void GetRect(RECT *rc,int *posx,int *posy,cScene *Scene);
	void GetRectRadar(RECT *rc,int *posx,int *posy);

	void SetPosition(int posx,int posy);
	void GetPosition(int *posx,int *posy);
	void SetCell(int cellx,int celly);
	void GetCell(int *cellx,int *celly);

	void Impactar(int damage);

	int x,y;		//Position in total map
	int cx,cy;		//Cell position in total map
	int id_enemy;
	bool dead;
	bool dying;

private:

	int health;
	bool damaged;

	int seq_die;


	//AI of the enemy for the path
	cPath Trajectory;

	int delay;		//Animation delay
};

#endif