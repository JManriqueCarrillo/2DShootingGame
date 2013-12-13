
#ifndef __GAME_H__
#define __GAME_H__

#include "cGraphicsLayer.h"
#include "cInputLayer.h"
#include "cController.h"
#include "cScene.h"
#include <list>

#define STATE_MAIN	0
#define STATE_GAME	1

typedef struct
{
	int id;
	int x;
	int y;
	int vida;
	bool impactado;
}EnemyStruct;


class cGame
{
public:
	cGame();
	virtual ~cGame();

	bool Init(HWND hWnd,HINSTANCE hInst,bool exclusive);
	bool Loop(); 
	void Finalize();

private:
	bool LoopInput();
	bool LoopProcess();
	bool LoopOutput();

	cGraphicsLayer Graphics;
	cInputLayer Input;
	cController Controller;
	cScene Scene;
	cCritter Critter;
	cSkeleton Skeleton;
	cBullet Bullet;
	
	int state;

	std::list<EnemyStruct> listaEnemigos;

	void ProcessOrder();
	bool Render();
	void Shoot();

	void bulletsCollision();
};

#endif
