
#include "cGame.h"
#include "cLog.h"
#include "cKeyboard.h"
#include <list>
#include <time.h>
#include "cKeyboard.h";
#include <list>

cGame::cGame() 
{
	int newx, newy;

	srand(time(NULL));
	
	EnemArray.numEnemies = 3;
	for (int i=0;i<EnemArray.numEnemies;i++)
	{
		newx = rand()%(SCENE_WIDTH-2)+1;
		newy = rand()%(SCENE_HEIGHT-3)+1;
		EnemArray.enemies[i].SetEnemy(i,newx,newy,100);
	}

}
cGame::~cGame(){}

bool cGame::Init(HWND hWnd,HINSTANCE hInst,bool exclusive)
{
	bool res;
	cLog *Log = cLog::Instance();

	res = Graphics.Init(hWnd);
	if(!res)
	{
		Log->Msg("Error initializing Graphics!");
		return false;
	}

	res = Input.Init(hInst,hWnd,exclusive,USE_MOUSE|USE_KEYBOARD);
	if(!res)
	{
		Log->Msg("Error initializing Input!");
		return false;
	}
	Input.SetMousePosition(SCREEN_RES_X >> 1,SCREEN_RES_Y >> 1);

	Graphics.LoadData();

	Scene.LoadMap("map.txt");

	return true;
}

void cGame::Finalize()
{
	Graphics.UnLoadData();
	Graphics.Finalize();
	Input.UnacquireAll();
	Input.Finalize();
}

bool cGame::Loop()
{
	bool res;

	//Input
	res = LoopInput();
	if(!res) return false;
	
	//Process
	res = LoopProcess();
	if(!res) return false;

	//Output
	res = LoopOutput();
	if(!res) return false;
	
	return true;
}

bool cGame::LoopInput()
{
	bool res;
	cLog *Log = cLog::Instance();

	res = Input.Read();
	if(!res)
	{
		Log->Msg("Error reading Input!");
		return false;
	}

	res = Controller.Read();
	return true;
}

bool cGame::LoopProcess()
{
	cMouse *Mouse;
	Mouse = Input.GetMouse();

	switch(state)
	{
		case STATE_MAIN:
						if(Mouse->ButtonDown(LEFT))
						{
							//Play button
							if(Mouse->In(334,236,420,278))
							{
								state = STATE_GAME;
							}
							//Exit button
							else if(Mouse->In(426,236,512,278))
							{
								return false;
							}
						}
						break;

		case STATE_GAME:
						ProcessOrder();
						for (int i=0;i<EnemArray.numEnemies;i++)
						{
							EnemArray.enemies[i].Move();
						}
						//Critter.Move();
						break;
	}
	return true;
}

bool cGame::LoopOutput()
{
	bool res;
	res = Render();
	return res;
}

bool cGame::Render()
{
	bool res;
	res = Graphics.Render(state, Input.GetMouse(), &Scene, &Critter, &EnemArray, &Bullet);
	return res;
}

void cGame::ProcessOrder()
{
	//cMouse *Mouse;
	cKeyboard *Keyboard;
	int mx,my,msx,msy,p,cx,cy,x,y;
	int s=5; //marge for directional pointers
	int xo,xf,yo,yf;
	//int b4pointer;
	bool KeyPushed = false;

	//Player position
	int px,py;

	//Mouse = Input.GetMouse();
	Keyboard = Input.GetKeyboard();
	//b4pointer = Mouse->GetPointer();
	//Mouse->GetPosition(&mx,&my);

	if(Scene.isMoving)
	{
		Scene.MoveMap(Scene.moveDir);
	}
	else
	{
		float xcont = Controller.getLeftStickX(0);
		float ycont = Controller.getLeftStickY(0);

		if	   (Keyboard->KeyDown(DIK_W)&&Keyboard->KeyDown(DIK_D)) Critter.MoveKey(NE,&Scene);
		else if(Keyboard->KeyDown(DIK_W)&&Keyboard->KeyDown(DIK_A)) Critter.MoveKey(NO,&Scene);
		else if(Keyboard->KeyDown(DIK_W)&&Keyboard->KeyDown(DIK_D)) Critter.MoveKey(NE,&Scene);
		else if(Keyboard->KeyDown(DIK_S)&&Keyboard->KeyDown(DIK_A)) Critter.MoveKey(SO,&Scene);
		else if(Keyboard->KeyDown(DIK_S)&&Keyboard->KeyDown(DIK_D)) Critter.MoveKey(SE,&Scene);
		else if(Keyboard->KeyDown(DIK_W)) Critter.MoveKey(N,&Scene);
		else if(Keyboard->KeyDown(DIK_S)) Critter.MoveKey(S,&Scene);
		else if(Keyboard->KeyDown(DIK_D)) Critter.MoveKey(E,&Scene);
		else if(Keyboard->KeyDown(DIK_A)) Critter.MoveKey(O,&Scene);
		else if(xcont!= 0 || ycont!=0) Critter.MoveController(xcont,-ycont,&Scene); // Controller movement
		else {Critter.MoveKey(STOP,&Scene);} 

		if(Scene.isDoorOpen)
		{
			switch(Scene.doorMap[int((Critter.x+16)/32)][int((Critter.y+16)/32)])
			{
				case Norte: Scene.MoveMap(Norte); break;
				case Sur: Scene.MoveMap(Sur); break;
				case Oeste: Scene.MoveMap(Oeste); break;
				case Este: Scene.MoveMap(Este); break;
			}
		}

		if((Keyboard->KeyDown(DIK_RCONTROL) || Controller.buttonDown(0, XINPUT_GAMEPAD_X)) && Bullet.isReadyToShoot())
		{
			int posx, posy;
			int dir, angulo=0;
			Critter.GetPosition(&posx,&posy);

			switch(Critter.CritterDir)
			{
				case N: angulo = 90; break;
				case S: angulo = 270; break;
				case O: angulo = 180; break;
				case E: angulo = 0; break;
				case NO: angulo = 135; break;
				case NE: angulo = 45; break;
				case SO: angulo = 225; break;
				case SE: angulo = 315; break;
		
			}
			Bullet.NewBullet(0,posx, posy, angulo, false, 0, 10);
		}

		if(Keyboard->KeyDown(DIK_LCONTROL))
			Scene.OpenNextDoor();

		bulletsCollision();
	
		Bullet.UpdateBullets(&Scene);

		/**
		* Update the doors animation
		**/
		Scene.UpdateDoors();

		/**
		* Enemy moving to the player
		**/
		Critter.GetCell(&px,&py);

		for (int i=0;i<EnemArray.numEnemies;i++)
		{
			EnemArray.enemies[i].GoToPlayer(Scene.Pathmap,px,py);
		}
	}
}

void cGame::bulletsCollision(){
	/*
	Modificará la lista de bullets i de los enemigos y Critter añadiendo un flag "colision" por si ha colisionado con ellos
	*/
	int headx, heady;

	//Por cada bullet
	std::list<BulletStruct>::iterator illista;
	illista = Bullet.listaBullets.begin();
	while( illista != Bullet.listaBullets.end() )
	{
		headx = illista->x + 32 + Bullet.cosdeg(illista->angulo) * illista->speed;
		heady = illista->y + 16 + Bullet.sindeg(illista->angulo) * illista->speed;
		
		//	Si no se ha salido de limites
		if(Scene.isWalkable(int(headx/32), int(heady/32)))
		{
			//Por cada enemigo
			for (int i=0;i<EnemArray.numEnemies;i++)
			{
				//Comprovar colision
				//Si colisionConEnemigo
				
				if((headx > EnemArray.enemies[i].x && headx < EnemArray.enemies[i].x+32) && (heady > EnemArray.enemies[i].y && heady < EnemArray.enemies[i].y+32))
				{
					EnemArray.enemies[i].Impactar(illista->poder);
					//quitarVida(EnemigoColisionado, Bullet.poder) //TODO
					illista->destroying = true;
				}

			}
		}
		else 
		{
			illista->destroying = true;
		}
		illista++;
	}
}