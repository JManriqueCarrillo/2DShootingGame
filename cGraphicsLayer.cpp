
#include "cGraphicsLayer.h"
#include "cGame.h"
#include "cLog.h"
#include <stdio.h>


cGraphicsLayer::cGraphicsLayer()
{
	g_pD3D = NULL;
	g_pD3DDevice = NULL;
	g_pSprite = NULL;
}

cGraphicsLayer::~cGraphicsLayer(){}

bool cGraphicsLayer::Init(HWND hWnd)
{
	cLog *Log = cLog::Instance();
	HRESULT hr;

	D3DVIEWPORT9 viewPort = { 0, 0, SCREEN_RES_X, SCREEN_RES_Y, 0.0f, 1.0f };

	g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if(g_pD3D==NULL)
	{
		Log->Msg("Error creating Direct3D object");
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );

	d3dpp.Windowed               = FALSE;
	d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;	//Efficient page flipping
	d3dpp.BackBufferWidth        = SCREEN_RES_X;
    d3dpp.BackBufferHeight       = SCREEN_RES_Y;
    d3dpp.BackBufferFormat       = D3DFMT_X8R8G8B8;

	hr = g_pD3D->CreateDevice(	D3DADAPTER_DEFAULT, 
								D3DDEVTYPE_HAL, 
								hWnd,
								D3DCREATE_SOFTWARE_VERTEXPROCESSING,
								&d3dpp, 
								&g_pD3DDevice );
	if(FAILED(hr))
	{
		Log->Error(hr,"Creating Direct3D device");
		return false;
	}

	// Configure for 2d operations
    hr = g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    hr = g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	hr = g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	if(FAILED(hr))
	{
		Log->Error(hr,"Setting render state");
		return false;
	}

	hr = g_pD3DDevice->SetViewport(&viewPort);
	if(FAILED(hr))
	{
		Log->Error(hr,"Setting viewport");
		return false;
	}

	//Init font
	InitD3D();

	return true;
}

// [ DirectX9 Initialization ]
HRESULT cGraphicsLayer::InitD3D(void) {
	return D3DXCreateFont(g_pD3DDevice,     //D3D Device
                20,                                                             //Font height
                0,                                                              //Font width
                FW_NORMAL,                                              //Font Weight
                1,                                                              //MipLevels
                false,                                                  //Italic
                DEFAULT_CHARSET,                                //CharSet
                OUT_DEFAULT_PRECIS,                             //OutputPrecision
                DEFAULT_QUALITY,                                //Quality
                DEFAULT_PITCH|FF_DONTCARE,              //PitchAndFamily
                "Arial",                                                //pFacename,
                &g_font);                                               //ppFont
}

//Drawing Function:
void cGraphicsLayer::DrawString(int x, int y, COLORREF color, ID3DXFont *pFont, const char *fmt, ...)
{
        RECT FontPos = { x, y, x + 800, y + 16 };
        pFont->DrawText(NULL, fmt, -1, &FontPos, DT_TOP, color);
}

void cGraphicsLayer::Finalize()
{
	if(g_pD3DDevice)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}
	if(g_pD3D)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

void cGraphicsLayer::LoadData()
{
	D3DXCreateSprite( g_pD3DDevice, &g_pSprite ); 

	//Main menu
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"main.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								NULL,NULL,NULL,&texMain);
	//GUI game
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"game.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								NULL,NULL,NULL,&texGame);
	//Tiles
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"tiles.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0x00ff00ff,NULL,NULL,&texTiles);
	//Characters
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"characters.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0x00ff00ff,NULL,NULL,&texCharacters);
	//Bullets
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"Proyectil.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0x00ff00ff,NULL,NULL,&texBullet);
	//Mouse pointers
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"mouse.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0x00ff00ff,NULL,NULL,&texMouse);
	//Tileset
	D3DXCreateTextureFromFileEx(g_pD3DDevice,"tileset.png",0,0,1,0,D3DFMT_UNKNOWN,
								D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								0x00ff00ff,NULL,NULL,&texTileset);
}

void cGraphicsLayer::UnLoadData()
{
	if(texMain)
	{
		texMain->Release();
		texMain = NULL;
	}
	if(texGame)
	{
		texGame->Release();
		texGame = NULL;
	}
	if(texTiles)
	{
		texTiles->Release();
		texTiles = NULL;
	}
	if(texTileset)
	{
		texTileset->Release();
		texTileset = NULL;
	}
	if(texCharacters)
	{
		texCharacters->Release();
		texCharacters = NULL;
	}
	if(texBullet)
	{
		texBullet->Release();
		texBullet = NULL;
	}
	if(texMouse)
	{
		texMouse->Release();
		texMouse = NULL;
	}
	if(g_pSprite)
	{
		g_pSprite->Release();
		g_pSprite = NULL;
	}
}

bool cGraphicsLayer::Render(int state,cMouse *Mouse,cScene *Scene,cCritter *Critter,cSkeleton *Skeleton,cBullet *Bullet)
{
	g_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, 0xFF000000, 0, 0 );
	g_pD3DDevice->BeginScene();

		switch(state)
		{
			case STATE_MAIN:
				g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
					g_pSprite->Draw(texMain,NULL,NULL,&D3DXVECTOR3(0.0f,0.0f,0.0f),0xFFFFFFFF);
					DrawMouse(Mouse);
				g_pSprite->End();
				break;

			case STATE_GAME:
				g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
					DrawScene(Scene);
				g_pSprite->End();

				g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
					DrawSceneLayer2(Scene);
					DrawUnits(Scene,Critter,Skeleton);
					DrawBullets(Bullet);
				g_pSprite->End();
				break;
		}

		g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
			DrawString(5,5,D3DCOLOR_ARGB(255,255, 255, 0), g_font, Scene->logtext);
		g_pSprite->End();

	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present( NULL, NULL, NULL, NULL );

	return true;
}

void cGraphicsLayer::GetSceneRect(RECT *rc, int n)
{
	switch(n)
	{
		case 0: SetRect(rc,32*5,32*1,32*6,32*2); break;
		case 40:
		case 41:
		case 42:
		case 43:
		case 44:
		case 45:
		case 60:
		case 61:
		case 62:
		case 63:
		case 64:
		case 65:
		case 1: SetRect(rc,32*5,32*0,32*6,32*1); break;
		case 3: SetRect(rc,32*1,32*0,32*2,32*1); break;
		case 4: SetRect(rc,32*2,32*0,32*3,32*1); break;
		case 5: SetRect(rc,32*1,32*1,32*2,32*2); break;
		case 2: //Lo de abajo
		default: SetRect(rc,32*0,32*0,32*1,32*1); break;
	}
}

bool cGraphicsLayer::GetSceneSecondLayerRect(RECT *rc, int n)
{
	switch(n)
	{
		//Barriles
		case 50: SetRect(rc,32*7,32*1,32*8,32*2); break;
		case 51: SetRect(rc,32*7,32*3,32*8,32*4); break;

		// Puerta izquierda
		case 40: SetRect(rc,32*0,32*2,32*1,32*4); break;
		case 41: SetRect(rc,32*1,32*2,32*2,32*4); break;
		case 42: SetRect(rc,32*2,32*2,32*3,32*4); break;
		case 43: SetRect(rc,32*3,32*2,32*4,32*4); break;
		case 44: SetRect(rc,32*4,32*2,32*5,32*4); break;

		case 60: SetRect(rc,32*0,32*4,32*1,32*6); break;
		case 61: SetRect(rc,32*1,32*4,32*2,32*6); break;
		case 62: SetRect(rc,32*2,32*4,32*3,32*6); break;
		case 63: SetRect(rc,32*3,32*4,32*4,32*6); break;
		case 64: SetRect(rc,32*4,32*4,32*5,32*6); break;

		case 80: SetRect(rc,32*5,32*2,32*7,32*4); break;
		case 81: SetRect(rc,32*5,32*4,32*7,32*6); break;
		case 82: SetRect(rc,32*5,32*6,32*7,32*8); break;
		case 83: SetRect(rc,32*5,32*8,32*7,32*10); break;
		//case 84: SetRect(rc,32*5,32*10,32*7,32*12); break;

		case 20: SetRect(rc,32*0,32*6,32*2,32*8); break;
		case 21: SetRect(rc,32*0,32*8,32*2,32*10); break;
		case 22: SetRect(rc,32*0,32*10,32*2,32*12); break;
		case 23: SetRect(rc,32*0,32*12,32*2,32*14); break;
		//case 24: SetRect(rc,32*0,32*14,32*2,32*16); break;

		default: return false;
	}

	return true;
}

bool cGraphicsLayer::DrawScene(cScene *Scene)
{
	RECT rc, rc2;
	int x,y,n,n2,
		fx,fy,
		pantx,panty;
	bool paint;

	fx = SCENE_WIDTH;
	fy = SCENE_HEIGHT;
	for(y=0;y<fy;y++)
	{
		for(x=0;x<fx;x++)
		{
			n = Scene->map[x][y];
			GetSceneRect(&rc,n);

			if(Scene->isMoving)
			{
				int offsetx = 0, offsety = 0, offsetWIDTH = 0, offsetHEIGHT = 0;
				Scene->getMoveMapOffsets(&offsetx,&offsety,&offsetWIDTH,&offsetHEIGHT);

				g_pSprite->Draw(texTileset,&rc,NULL, 
								&D3DXVECTOR3(float(x*32 + offsetx*32 + offsetWIDTH*32),float(y*32 + offsety*32 + offsetHEIGHT*32),0.0f), 
								0xFFFFFFFF);

				n2 = Scene->auxMap[x][y];
				GetSceneRect(&rc,n2);

				g_pSprite->Draw(texTileset,&rc,NULL, 
								&D3DXVECTOR3(float(x*32 + offsetx*32),float(y*32 + offsety*32),0.0f), 
								0xFFFFFFFF);
			}
			else
			{
				g_pSprite->Draw(texTileset,&rc,NULL, 
								&D3DXVECTOR3(float(x*32),float(y*32),0.0f), 
								0xFFFFFFFF);
			}
		}
	}

	//Draw radar
	/*x=RADAR_Xo+(Scene->cx<<2);
	y=RADAR_Yo+(Scene->cy<<2);
	SetRect(&rc,0,32,80,100);
	g_pSprite->Draw(texTiles,&rc,NULL, 
					&D3DXVECTOR3(float(x),float(y),0.0f), 
					0xFFFFFFFF);*/
	return true;
}

bool cGraphicsLayer::DrawSceneLayer2(cScene *Scene)
{
	RECT rc, rc2;
	int x,y,n,n2,
		fx,fy,
		pantx,panty;
	bool paint;

	fx = SCENE_WIDTH;
	fy = SCENE_HEIGHT;
	for(y=0;y<fy;y++)
	{
		for(x=0;x<fx;x++)
		{
			n = Scene->map[x][y];
			paint = GetSceneSecondLayerRect(&rc2,n);

			if(paint)
			{
				if(Scene->isMoving)
				{
					int offsetx = 0, offsety = 0, offsetWIDTH = 0, offsetHEIGHT = 0;
					Scene->getMoveMapOffsets(&offsetx,&offsety,&offsetWIDTH,&offsetHEIGHT);

					g_pSprite->Draw(texTileset,&rc,NULL, 
									&D3DXVECTOR3(float(x*32 + offsetx*32 + offsetWIDTH*32),float(y*32 + offsety*32 + offsetHEIGHT*32),0.0f), 
									0xFFFFFFFF);

					n2 = Scene->auxMap[x][y];
					paint = GetSceneSecondLayerRect(&rc2,n2);

					g_pSprite->Draw(texTileset,&rc,NULL, 
									&D3DXVECTOR3(float(x*32 + offsetx*32),float(y*32 + offsety*32),0.0f), 
									0xFFFFFFFF);
				}
				else
				{
					g_pSprite->Draw(texTileset,&rc2,NULL, 
									&D3DXVECTOR3(float(x*32),float(y*32),0.0f), 
									0xFFFFFFFF);
				}
			}
		}
	}
	return true;
}

bool cGraphicsLayer::DrawUnits(cScene *Scene,cCritter *Critter,cSkeleton *Skeleton)
{
	int cx,cy,posx,posy;
	RECT rc;

	//Draw Critter
	Critter->GetCell(&cx,&cy);
	if(Scene->Visible(cx,cy))
	{
		float offsetx = 0, offsety = 0;
		if(Scene->isMoving)
		{
			Scene->getMovePlayerOffsets(&offsetx,&offsety);
			Critter->GetPosition(&posx,&posy);
			Critter->SetPosition(posx + (offsetx*32), posy + (offsety*32));
		}

		Critter->GetRect(&rc,&posx,&posy,Scene);
		g_pSprite->Draw(texCharacters,&rc,NULL, 
						&D3DXVECTOR3(float(posx),float(posy),0.0f), 
						0xFFFFFFFF);


		if(Critter->GetSelected())
		{
			Critter->GetRectLife(&rc,&posx,&posy,Scene);
			g_pSprite->Draw(texMouse,&rc,NULL, 
							&D3DXVECTOR3(float(posx),float(posy),0.0f), 
							0xFFFFFFFF);
		}
	}
	Critter->GetRectRadar(&rc,&posx,&posy);
	g_pSprite->Draw(texTiles,&rc,NULL, 
					&D3DXVECTOR3(float(posx),float(posy),0.0f), 
					0xFFFFFFFF);
	//Draw Skeleton
	Skeleton->GetCell(&cx,&cy);
	if(Scene->Visible(cx,cy))
	{
		Skeleton->GetRect(&rc,&posx,&posy,Scene);
		g_pSprite->Draw(texCharacters,&rc,NULL, 
						&D3DXVECTOR3(float(posx),float(posy),0.0f), 
						0xFFFFFFFF);
	}
		Skeleton->GetRectRadar(&rc,&posx,&posy);
		

		

		g_pSprite->Draw(texTiles,&rc,NULL, 
					&D3DXVECTOR3(float(posx),float(posy),0.0f)
					, 
					0xFFFFFFFF);
		

	//Draw Fire
	if(Critter->GetShooting())
	{
		if(Critter->IsFiring())
		{
			//Advance animation & draw
			Critter->GetRectShoot(&rc,&posx,&posy,Scene);
						
			g_pSprite->Draw(texCharacters,&rc,NULL, 
							&D3DXVECTOR3(float(posx),float(posy),0.0f), 
							0xFFFFFFFF);
					}
		else
		{
			//Advance animation
			Critter->GetRectShoot(&rc,&posx,&posy,Scene);
		}
	}
	return true;
}

bool cGraphicsLayer::DrawBullets(cBullet *Bullet)
{
	float angulo,x,y;
	int id;
	RECT rc;
	D3DXMATRIX matRotate;
	D3DXVECTOR2 vCenter(16.0f,16.0f);
	D3DXVECTOR2 vPosition(0, 0);

	std::list<BulletStruct>::iterator illista;
	illista = Bullet->listaBullets.begin();
	while( illista != Bullet->listaBullets.end() )
	{
		angulo = illista->angulo;
		x = illista->x;
		y = illista->y;
		id = illista->id;
		vPosition.x = x + Bullet->cosdeg(angulo) * 16;
		vPosition.y = y - Bullet->sindeg(angulo) * 16;

		D3DXMatrixTransformation2D(&matRotate, NULL, NULL, NULL, &vCenter,-PI/180*angulo , &vPosition);
		g_pSprite->SetTransform(&matRotate);

		SetRect(&rc,0,0,32,32);
		g_pSprite->Draw(texBullet,&rc,NULL, 
				NULL,//&D3DXVECTOR3(float(x),float(y),0.0f), 
				0xFFFFFFFF);

		D3DXMatrixRotationZ(&matRotate, 0);
		g_pSprite->SetTransform(&matRotate);

		illista++;
	}

	return true;
}

bool cGraphicsLayer::DrawMouse(cMouse *Mouse)
{
	RECT rc;
	int mx,my,posx,posy;

	//Mouse selection box
	Mouse->GetPosition(&mx,&my);

	if(Mouse->GetSelection()==SELECT_SCENE)
	{
		int sx,sy;
		Mouse->GetSelectionPoint(&sx,&sy);
		SetRect(&rc,sx,sy,mx,my);
		DrawRect(rc,0x0000ff00);
	}

	//Mouse
	g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		
		Mouse->GetRect(&rc,&posx,&posy);
		HRESULT hr = g_pSprite->Draw(texMouse,&rc,NULL,&D3DXVECTOR3(float(mx+posx),float(my+posy),0.0f),0xFFFFFFFF);
		if(FAILED(hr))
		{
			cLog *Log = cLog::Instance();
			Log->Error(hr,"mouse pointer");
			return false;
		}

	g_pSprite->End();

	return true;
}

bool cGraphicsLayer::DrawRect(RECT rc, D3DCOLOR color)
{
	RECT rect;
	int xo,yo,xf,yf;

	if((rc.left==rc.right)&&(rc.top==rc.bottom)) return false;

	if(rc.left < rc.right)
	{
		xo = rc.left;	xf = rc.right;
	}
	else
	{
		xo = rc.right;	xf = rc.left;
	}
	if(rc.top < rc.bottom)
	{
		yo = rc.top;	yf = rc.bottom;
	}
	else
	{
		yo = rc.bottom;	yf = rc.top;
	}

	//Top
	SetRect(&rect,xo,yo,xf+1,yo+1);
	g_pD3DDevice->Clear(1,(D3DRECT *)&rect,D3DCLEAR_TARGET,color,1.0f,0);
	//Bottom
	SetRect(&rect,xo,yf,xf,yf+1);
	g_pD3DDevice->Clear(1,(D3DRECT *)&rect,D3DCLEAR_TARGET,color,1.0f,0);
	//Left
	SetRect(&rect,xo,yo,xo+1,yf+1);
	g_pD3DDevice->Clear(1,(D3DRECT *)&rect,D3DCLEAR_TARGET,color,1.0f,0);
	//Right
	SetRect(&rect,xf,yo,xf+1,yf+1);
	g_pD3DDevice->Clear(1,(D3DRECT *)&rect,D3DCLEAR_TARGET,color,1.0f,0);
	
	return true;
}