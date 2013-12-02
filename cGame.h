
#ifndef __GAME_H__
#define __GAME_H__

#include "cGraphicsLayer.h"
#include "cInputLayer.h"
#include "cScene.h"

#define STATE_MAIN	0
#define STATE_GAME	1

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
	cScene Scene;
	cCritter Critter;
	cSkeleton Skeleton;
	
	int state;

	void ProcessOrder();
	bool Render();
};

#endif
