
#include "cCritter.h"
#include "cTrajectory.h"
#include "cScene.h"

cCritter::cCritter()
{
	SetPosition(384,96);
	SetCell(12,3);
	SetSelected(false);

	seq=0;
	delay=0;

	attack=false;
	shoot=false;
	shoot_seq=0;
	shoot_delay=0;

	CritterDir = S;
	isMoving = false;
}
cCritter::~cCritter()
{
	
}

void cCritter::GetRect(RECT *rc,int *posx,int *posy,cScene *Scene)
{
	*posx = SCENE_Xo + x - (Scene->cx<<5);
	*posy = SCENE_Yo + y - (Scene->cy<<5);

	switch(CritterDir)//(Trajectory.Faced())
	{
	case STOP:	SetRect(rc,256,0,288,32);	break;

	case N:		SetRect(rc,((12+seq)<<5),0,((12+seq+1)<<5),32);	break;
	case S:		SetRect(rc,(( 8+seq)<<5),0,(( 8+seq+1)<<5),32);	break;
	case SE:
	case NE:
	case E:		SetRect(rc,(( 7-seq)<<5),0,(( 7-seq+1)<<5),32);	break;
	case SO:
	case NO:
	case O:		SetRect(rc,     (seq<<5),0,   ((seq+1)<<5),32);	break;
	}
	//if(!Trajectory.IsDone())
	//{
	if(isMoving)
	{
		delay++;
		if(delay>=4)
		{
			seq++;
			if(seq>3) seq=0;
			delay=0;
		}
	}
	else
	{
		delay = 0;
		seq = 0;
	}
	//}
}

void cCritter::GetRectLife(RECT *rc,int *posx,int *posy,cScene *Scene)
{
	*posx = SCENE_Xo + x - (Scene->cx<<5);
	*posy = SCENE_Yo + y - (Scene->cy<<5);

	//Life dependency not implemented
	SetRect(rc,0,32,32,64);
}

void cCritter::GetRectShoot(RECT *rc,int *posx,int *posy,cScene *Scene)
{
	*posx = SCENE_Xo + x - (Scene->cx<<5);
	*posy = SCENE_Yo + y - (Scene->cy<<5);

	SetRect(rc,shoot_seq<<5,64,(shoot_seq+1)<<5,96);
	switch(shoot_seq)
	{
	case 0:	*posx+=5;	break;
	case 1:	*posx-=2;	break;
	case 2: *posx-=8;	break;
	case 3: *posx-=16;	break;
	case 4: *posx-=24;	break;
	case 5:	*posx-=32;	break;
	case 6: *posx-=32;	break;
	case 7: *posx-=32;	break;
	}

	shoot_delay++;
	if(shoot_delay==4)
	{
		shoot_seq++;
		if(shoot_seq==16) shoot_seq=0;
		shoot_delay=0;
	}
}

void cCritter::GetRectRadar(RECT *rc,int *posx,int *posy)
{
	*posx = RADAR_Xo + ( cx << 2 );
	*posy = RADAR_Yo + ( cy << 2 );

	SetRect(rc,80,32,84,36);
}

void cCritter::GoToCell(int destcx,int destcy)
{
	// "leave all we're doing"
	attack=false;
	shoot=false;

	// Go
	if(Trajectory.IsDone())	Trajectory.Make(cx,cy,destcx,destcy);
	else					Trajectory.ReMake(destcx,destcy);
}

void cCritter::GoToEnemy(int destcx,int destcy)
{
	//(Only implemented attack right to left)
	GoToCell(destcx+1,destcy);

	attack=true;
	shoot=false;
}
void cCritter::Move()
{
	int mov;

	if(!Trajectory.IsDone())
	{
		mov=Trajectory.NextStep(&x,&y,&cx,&cy);

		if(mov==ARRIVE)
		{
			Trajectory.Done();
			seq=0;
		}
		else if(mov==CONTINUE)
		{
		}
	}
	else
	{
		//Moved for attack?
		if(attack)
		{
			shoot=true;
			shoot_seq=0;
			shoot_delay=0;
			attack=false;
		}
	}
}

void cCritter::MoveKey(int dir, cScene *Scene)
{
	int auxx, auxy;

	auxx = x;
	auxy = y;

	int incremento = 5;
	int incremento2 = 4;

	switch (dir)
	{
		case N: auxy-=incremento; break;
		case E: auxx+=incremento; break;
		case S: auxy+=incremento; break;
		case O: auxx-=incremento; break;
		case NE: auxy-=incremento2; auxx+=incremento2; break;
		case NO: auxy-=incremento2; auxx-=incremento2; break;
		case SE: auxy+=incremento2; auxx+=incremento2; break;
		case SO: auxy+=incremento2; auxx-=incremento2; break;
		case STOP: break; //Does fucking nothing!!!
	}

	if(	Scene->isWalkable(int(auxx/32), int(y/32)) && Scene->isWalkable(int(auxx/32)+1, int(y/32)) && Scene->isWalkable(int(auxx/32), int(y/32)+1) && Scene->isWalkable(int(auxx/32)+1, int(y/32)+1) ) 	x = auxx;
	if(	Scene->isWalkable(int(x/32), int(auxy/32)) && Scene->isWalkable(int(x/32)+1, int(auxy/32)) && Scene->isWalkable(int(x/32), int(auxy/32)+1) && Scene->isWalkable(int(x/32)+1, int(auxy/32)+1) )	y = auxy;
	
	isMoving = true;
	if(dir == STOP) isMoving = false;
	else			CritterDir = dir;

	cx = int(x/32);
	cy = int(y/32);

	/*if (!GetCollision(&dir))
	if(!isCollidingMap(
	{
		cx = (x+16)/((SCENE_Xf-SCENE_Xo)/20);
		cy = (y+24)/((SCENE_Yf-SCENE_Yo)/17);
	}
	*/
}

void cCritter::SetPosition(int posx,int posy)
{
	x = posx;
	y = posy;
}
void cCritter::GetPosition(int *posx,int *posy)
{
	*posx = x;
	*posy = y;
}
void cCritter::SetCell(int cellx,int celly)
{
	cx = cellx;
	cy = celly;
}
void cCritter::GetCell(int *cellx,int *celly)
{
	*cellx = cx;
	*celly = cy;
}
void cCritter::SetSelected(bool sel)
{
	selected = sel;
}
bool cCritter::GetSelected()
{
	return selected;
}
bool cCritter::GetShooting()
{
	return shoot;
}
bool cCritter::IsFiring()
{
	return (shoot_seq<8);
}

bool cCritter::GetCollision(int *dir)
{
	//Dir can be changed

	//cLog Log;
	//char str[50];

	//itoa(SCENE_Yo,str,10);
	//Log.Msg(str);

	int I_subdir;

	switch (*dir)
	{
	case N: if ((y-3<=0)) return true; break;
	case E: if ((x+3>=(SCENE_Xf-32))) return true; break;
	case S: if ((y+3>=SCENE_Yf-64)) return true; break;
	case O: if ((x-3<=SCENE_Xo)) return true; break;
	case NE: if((y-3<=0)||(x+3>=(SCENE_Xf-32))) {
		I_subdir = N;
		if(!GetCollision(&I_subdir)){
			*dir = N;
			return false;
		}
		else{
			I_subdir = E;
			if(!GetCollision(&I_subdir)){
				*dir = E;
				return false;
			}
			else{
				return true; 
			}
		}
	}

			 break;
	case NO: if((y-3<=0)||(x-3<=(SCENE_Xo))) {
		I_subdir = N;
		if(!GetCollision(&I_subdir)){
			*dir = N;
			return false;
		}
		else{
			I_subdir = O;
			if(!GetCollision(&I_subdir)){
				*dir = O;
				return false;
			}
			else{
				return true; 
			}
		}
			 } break;
	case SE: if((y+3>=SCENE_Yf-88)||(x+3>=(SCENE_Xf-32))) {
					I_subdir = S;
					if(!GetCollision(&I_subdir)){
						*dir = S;
						return false;
					}
					else{
						I_subdir = E;
						if(!GetCollision(&I_subdir)){
							*dir = E;
							return false;
						}
						else{
							return true; 
						}
					}
				 } break;
	case SO: if((y+3>=SCENE_Yf-88)||(x-3<=SCENE_Xo)){
					I_subdir = S;
					if(!GetCollision(&I_subdir)){
						*dir = S;
						return false;
					}
					else{
						I_subdir = O;
						if(!GetCollision(&I_subdir)){
							*dir = O;
							return false;
						}
						else{
							return true; 
						}
					}
				 }break;
	}

	//itoa(y,str,10);
	//Log.Msg(str);

	return false;
}

void cCritter::Fire()
{
	shoot = true;
}