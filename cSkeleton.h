
#ifndef __SKELETON_H__
#define __SKELETON_H__

#include <windows.h>

class cScene;

class cSkeleton
{
public:
	cSkeleton(void);
	virtual ~cSkeleton(void);

	void GetRect(RECT *rc,int *posx,int *posy,cScene *Scene);
	void GetRectRadar(RECT *rc,int *posx,int *posy);

	void SetPosition(int posx,int posy);
	void GetPosition(int *posx,int *posy);
	void SetCell(int cellx,int celly);
	void GetCell(int *cellx,int *celly);

private:
	int x,y;		//Position in total map
	int cx,cy;		//Cell position in total map
};

#endif