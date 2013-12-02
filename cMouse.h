
#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "cInputLayer.h"

class cScene;

//Mouse buttons
#define LEFT		0
#define RIGHT		1

//Mouse pointers:
#define NORMAL		0
#define MOVE		1
#define ATTACK		2
#define SELECT		3
#define MN			4
#define MS			5
#define ME			6
#define MO			7
#define MNE			8
#define MSE			9
#define MNO			10
#define MSO			11

//Mouse delay animation
#define MDELAY		2

//Mouse selecting 
#define SELECT_NOTHING	0
#define SELECT_SCENE	1
#define SELECT_RADAR	2

class cMouse 
{
public:
	cMouse(LPDIRECTINPUT8 pDI, HWND hwnd, bool isExclusive = true);
	~cMouse();

	bool	ButtonDown(int button);
	bool	ButtonUp(int button);
	int		GetWheelMovement();
	void	GetMovement(int *dx, int *dy);

	bool Acquire();
	bool Unacquire();

	void SetPosition(int xo, int yo);
	void GetPosition(int *xpos, int *ypos);
	void GetCell(int *xcell,int *ycell);
	void SetPointer(int p);
	int  GetPointer();
	void SetSelection(int s);
	int  GetSelection();
	void SetSelectionPoint(int sx,int sy);
	void GetSelectionPoint(int *sx,int *sy);

	bool Read();

	void GetRect(RECT *rc,int *posx,int *posy);
	bool In(int xo,int yo,int xf,int yf);
	bool InCell(cScene *Scene,int cellx,int celly);
	void InitAnim();

private:
	LPDIRECTINPUTDEVICE8 m_pDIDev;
	DIMOUSESTATE	m_state;

	int x,y;
	int cx,cy;
	int pointer;
	int select;				//Selecting state
	int select_x,select_y;  //Select 1st point

	int seq,delay;
};

#endif 
