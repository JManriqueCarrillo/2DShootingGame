
#include "cGame.h"
#include "cLog.h"
#include "cKeyboard.h";
#include <list>

cGame::cGame() {}
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
						Skeleton.Move();
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
	res = Graphics.Render(state,Input.GetMouse(),&Scene,&Critter,&Skeleton, &Bullet);
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
	else if(xcont!= 0 && ycont!=0) Critter.MoveController(xcont,-ycont,&Scene); // Controller movement
	else {Critter.MoveKey(STOP,&Scene);} 

	if(Scene.isMoving)
	{
		Scene.MoveMap(Scene.moveDir);
	}
	else
	{
		switch(Scene.map[int((Critter.x+16)/32)][int((Critter.y+16)/32)])
		{
			case 4: Scene.MoveMap(Norte); break;
			case 5: Scene.MoveMap(Sur); break;
			case 6: Scene.MoveMap(Oeste); break;
			case 7: Scene.MoveMap(Este); break;
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
		Bullet.NewBullet(0,posx, posy, angulo, false, 0,10);
	}

	bulletsCollision();

	Bullet.UpdateBullets(&Scene);

	/**
	* Enemy moving to the player
	**/
	Critter.GetCell(&px,&py);

	Skeleton.GoToPlayer(Scene.Pathmap,px,py);

	// Critter.SetPosition(x,y);
	/*
	if(Mouse->ButtonDown(LEFT))
	{
		Mouse->SetPointer(NORMAL);
		
		if(Mouse->In(SCENE_Xo,SCENE_Yo,SCENE_Xf,SCENE_Yf))
		{
			if(Mouse->GetSelection()!=SELECT_SCENE)
			{
				//Select movement/attack
				if(Critter.GetSelected())
				{
					//Attack
					Skeleton.GetCell(&cx,&cy);
					if(Mouse->InCell(&Scene,cx,cy))
					{
						if(!Critter.GetShooting())
							Critter.GoToEnemy(Scene.cx+cx,Scene.cx+cy);
					}
					//Movement
					else
					{
						Mouse->GetCell(&cx,&cy);
						Critter.GoToCell(Scene.cx+cx,Scene.cy+cy);
					}
				}
				//Begin selection
				else
				{
					Mouse->SetSelection(SELECT_SCENE);
					Mouse->SetSelectionPoint(mx,my);
				}
			}
		}
		else if(Mouse->In(RADAR_Xo,RADAR_Yo,RADAR_Xf,RADAR_Yf))
		{
			if(Critter.GetSelected())
			{
				int radar_cell_x = (mx-RADAR_Xo) >> 2, //[672..799]/4=[0..31]
					radar_cell_y = (my-RADAR_Yo) >> 2; //[ 60..187]/4=[0..31]

				Critter.GoToCell(radar_cell_x,radar_cell_y);
			}
			else
			{
				Mouse->SetSelection(SELECT_RADAR);
				Scene.MoveByRadar(mx,my);
			}
		}
	}
	else if(Mouse->ButtonUp(LEFT))
	{
		if(Mouse->GetSelection()==SELECT_SCENE)
		{
			Mouse->GetSelectionPoint(&msx,&msy);

			xo = min(msx,mx)+(Scene.cx<<5)-SCENE_Xo;
			xf = max(msx,mx)+(Scene.cx<<5)-SCENE_Xo;
			yo = min(msy,my)+(Scene.cy<<5)-SCENE_Yo,
			yf = max(msy,my)+(Scene.cy<<5)-SCENE_Yo;
			
			Critter.GetPosition(&x,&y);
			if((xo<(x+32))&&(xf>=x)&&(yo<(y+32))&&(yf>=y))
				Critter.SetSelected(true);
		}
		Mouse->SetSelection(SELECT_NOTHING);

		//Mouse over Critter
		Critter.GetCell(&cx,&cy);
		if(Mouse->InCell(&Scene,cx,cy))
		{
			Mouse->SetPointer(SELECT);
			return;
		}
		//Mouse over Enemy
		Skeleton.GetCell(&cx,&cy);
		if(Mouse->InCell(&Scene,cx,cy))
		{
			Mouse->SetPointer(ATTACK);
		}
		else if(Mouse->In(s,SCENE_Yo,SCENE_Xf,SCENE_Yf-s))
		{
			//Critter selected pointing, where to move
			if(Critter.GetSelected())	Mouse->SetPointer(MOVE);
			//Critter selected but mouse out map
			else						Mouse->SetPointer(NORMAL);
		}
		else if(Mouse->In(RADAR_Xo,RADAR_Yo,RADAR_Xf,RADAR_Yf))
		{
			//Critter selected pointing, where to move trough radar
			if(Critter.GetSelected())	Mouse->SetPointer(MOVE);
		}
		else
		{	
			//Arrow mouse pointers to move through scene
			if	   (Mouse->In(             s,             s,SCREEN_RES_X-s,SCREEN_RES_Y-s)) Mouse->SetPointer(NORMAL);
			else if(Mouse->In(             s,             0,SCREEN_RES_X-s,             s)) Mouse->SetPointer(MN);
			else if(Mouse->In(             s,SCREEN_RES_Y-s,SCREEN_RES_X-s,  SCREEN_RES_Y)) Mouse->SetPointer(MS);
			else if(Mouse->In(SCREEN_RES_X-s,             s,  SCREEN_RES_X,SCREEN_RES_Y-s)) Mouse->SetPointer(ME);
			else if(Mouse->In(             0,             s,             s,SCREEN_RES_Y-s)) Mouse->SetPointer(MO);
			else if(Mouse->In(             0,             0,             s,             s)) Mouse->SetPointer(MNO);
			else if(Mouse->In(             0,SCREEN_RES_Y-s,             s,  SCREEN_RES_Y)) Mouse->SetPointer(MSO);
			else if(Mouse->In(SCREEN_RES_X-s,             0,  SCREEN_RES_X,             s)) Mouse->SetPointer(MNE);
			else if(Mouse->In(SCREEN_RES_X-s,SCREEN_RES_Y-s,  SCREEN_RES_X,  SCREEN_RES_Y)) Mouse->SetPointer(MSE);
			else																			Mouse->SetPointer(NORMAL);

			p = Mouse->GetPointer();
			if((p>=MN)&&(p<=MSO))	Scene.Move(p);
		}
	}
	if(Mouse->ButtonDown(RIGHT))
	{
		if(Critter.GetSelected())
		{
			Critter.SetSelected(false);
			Mouse->SetPointer(NORMAL);
		}
	}

	if(b4pointer!=Mouse->GetPointer()) Mouse->InitAnim();
	*/
}

void cGame::bulletsCollision(){
	/*
	Modificará la lista de bullets i de los enemigos y Critter añadiendo un flag "colision" por si ha colisionado con ellos
	*/
	int headx, heady;

	std::list<EnemyStruct>::iterator enemlist;

	
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
			enemlist = listaEnemigos.begin();
			while( enemlist != listaEnemigos.end() )
			{
				//Comprovar colision
				//Si colisionConEnemigo
				if((headx > enemlist->x && headx < enemlist->x+32) && (heady > enemlist->y && heady < enemlist->y+32))
				{
					enemlist->impactado = true;
					//quitarVida(EnemigoColisionado, Bullet.poder) //TODO
					illista->destroying = true;
				}
				enemlist++;
			}
		} 
		else 
		{
			illista->destroying = true;;
		}
		illista++;
	}
	
}