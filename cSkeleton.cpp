
#include "cSkeleton.h"
#include "cScene.h"
#include "cLog.h"
#include <stdio.h>
#include <time.h>


cSkeleton::cSkeleton()
{

}
cSkeleton::~cSkeleton()
{

}

void cSkeleton::SetEnemy ( int id, int newx, int newy, int newhealth)
{
	id_enemy = id;
	cx = newx;
	cy = newy;
	x = int(cx*32);
	y = int(cy*32);
	health = 100;
	damaged = false;
	dying = false;
	seq_die = 0;
	dead = false;
}

void cSkeleton::GetRect(RECT *rc,int *posx,int *posy,cScene *Scene)
{
	*posx = SCENE_Xo + x - (Scene->cx<<5);
	*posy = SCENE_Yo + y - (Scene->cy<<5);

	SetRect(rc,128,32,160,64);
	
}

void cSkeleton::GetRectRadar(RECT *rc,int *posx,int *posy)
{
	*posx = RADAR_Xo + ( cx << 2 );
	*posy = RADAR_Yo + ( cy << 2 );

	SetRect(rc,84,32,88,36);
}

//enemy movement
void cSkeleton::GoToCell(int *map,int destpx,int destpy)
{
	// "leave all we're doing"
	//attack=false;
	//shoot=false;

	// Go
	if(Trajectory.IsDone())	Trajectory.Make(map,cx,cy,destpx,destpy);
	else					Trajectory.ReMake(map,destpx,destpy);
}

void cSkeleton::GoToPlayer(int *map,int destpx,int destpy)
{
	//(Only implemented attack right to left)
	GoToCell(map,destpx,destpy);

	//attack=true;
	//shoot=false;
}
void cSkeleton::Move()
{
	int mov;

	if(!Trajectory.IsDone())
	{
		mov=Trajectory.NextStep(&x,&y,&cx,&cy);

		if(mov==ARRIVE)
		{
			Trajectory.Done();
			//seq=0;
		}
		else if(mov==CONTINUE)
		{
		}
	}
	else
	{
		//Moved for attack?
		/*if(attack)
		{
			shoot=true;
			shoot_seq=0;
			shoot_delay=0;
			attack=false;
		}*/
	}
}


void cSkeleton::SetPosition(int posx,int posy)
{
	x = posx;
	y = posy;
}
void cSkeleton::GetPosition(int *posx,int *posy)
{
	*posx = x;
	*posy = y;
}
void cSkeleton::SetCell(int cellx,int celly)
{
	cx = cellx;
	cy = celly;
}
void cSkeleton::GetCell(int *cellx,int *celly)
{
	*cellx = cx;
	*celly = cy;
}

void cSkeleton::Impactar(int damage)
{
	if (health > 0)
	{
		damaged = true;
		health -= damage;
		if (health <= 0)	dying = true;
	}

}