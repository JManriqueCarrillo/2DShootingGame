
#ifndef __CRITTER_H__
#define __CRITTER_H__

#include <windows.h>
#include <math.h>
#include <stdio.h>
#include "cTrajectory.h"
#include "cLog.h"

class cScene;

class cCritter
{
public:
	cCritter(void);
	virtual ~cCritter(void);

	float cCritter::arctandeg(float valor);


	void GoToCell(int destcx,int destcy);
	void GoToEnemy(int destcx,int destcy);

	int RecursiveMoveX(float incrementoxMax, float incrementox, cScene *Scene);
	int RecursiveMoveY(float incrementoyMax, float incrementoy, cScene *Scene);

	void Move(int auxx, int auxy, int dir, cScene *Scene);
	void MoveKey(int dir, cScene *Scene);

	void MoveController(float controllerx, float controllery, cScene *Scene);

	void Fire();
	void GetRect(RECT *rc,int *posx,int *posy,cScene *Scene);
	void GetRectLife(RECT *rc,int *posx,int *posy,cScene *Scene);
	void GetRectShoot(RECT *rc,int *posx,int *posy,cScene *Scene);
	void GetRectRadar(RECT *rc,int *posx,int *posy);

	void SetPosition(int posx,int posy);
	void GetPosition(int *posx,int *posy);
	void SetCell(int cellx,int celly);
	void GetCell(int *cellx,int *celly);
	void SetSelected(bool sel);
	bool GetSelected();
	bool GetShooting();
	bool IsFiring();
	bool GetCollision(int *dir);


	

	int x,y;		//Position in total map
	int cx,cy;		//Cell position in total map
	bool selected;	//Selected for move or attack

	cTrajectory Trajectory;

	int seq;		//Sequence animation control
	int delay;		//Animation delay
	int CritterDir; //Criter facing direction
	bool isMoving;

	bool attack;	//Order to attack established (moving for attack)
	bool shoot;		//Begin attack (to shoot)
	int shoot_seq;	//Shooter sequence animation control
	int shoot_delay;//Shooter animation delay
};

#endif