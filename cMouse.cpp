
#include "cMouse.h"
#include "cLog.h"
#include "cScene.h"
#include <dxerr9.h>

/************************************************************************
cMouse::Constructor()

Initialize DI device
*************************************************************************/
cMouse::cMouse(LPDIRECTINPUT8 pDI, HWND hwnd, bool isExclusive)
{
	cLog *Log = cLog::Instance();
	HRESULT hr;

	hr = pDI->CreateDevice(GUID_SysMouse, &m_pDIDev, NULL);
	if(FAILED(hr))
	{
		Log->Error(hr,"Creating mouse device");
	}
	hr = m_pDIDev->SetDataFormat(&c_dfDIMouse);
	if(FAILED(hr))
	{
		Log->Error(hr,"Setting mouse data format");
	}
	//DIMOUSESTATE Structure - http://msdn.microsoft.com/library/default.asp?url=/library/en-us/directx9_c/directx/input/ref/structs/dimousestate.asp
	
	DWORD flags;
	if (isExclusive)
	{
		flags = DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY;
	}
	else
	{
		flags = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;
		//ShowCursor(false);
	}

	hr = m_pDIDev->SetCooperativeLevel(hwnd, flags);
	if (FAILED(hr))
	{
		Log->Error(hr,"Setting mouse cooperative level");
	}

	hr = m_pDIDev->Acquire();
	if(FAILED(hr))
	{
		Log->Error(hr,"Acquiring mouse");
	}

	hr = m_pDIDev->GetDeviceState(sizeof(DIMOUSESTATE), &m_state);
	if(FAILED(hr))
	{
		Log->Error(hr,"Getting mouse device state");
	}

	SetPointer(NORMAL);
	SetSelection(SELECT_NOTHING);
	InitAnim();
}


/***************************************************************************
cMouse::Destructor()

Release DI device
***************************************************************************/
cMouse::~cMouse()
{
	if(m_pDIDev)
	{
		m_pDIDev->Unacquire();
		m_pDIDev->Release();
		m_pDIDev = NULL;
	}
}

/******************************************************************************
cMouse::Read()

Queuries the current state of the mouse and stores it in the member variables
********************************************************************************/
bool cMouse::Read()
{
	cLog *Log = cLog::Instance();
	HRESULT hr;

	hr = m_pDIDev->GetDeviceState(sizeof(DIMOUSESTATE), &m_state);
	if(FAILED(hr))
	{
		Log->Error(hr,"Getting mouse device state");

		hr = m_pDIDev->Acquire();
		if(FAILED(hr))
		{
			Log->Error(hr,"Acquiring mouse");
			return false;
		}
		hr = m_pDIDev->GetDeviceState(sizeof(DIMOUSESTATE), &m_state);
		if(FAILED(hr))
		{
			Log->Error(hr,"Getting mouse device state");
			return false;
		}
	}

	x+=m_state.lX;
	y+=m_state.lY;
	
	switch(select)
	{
		case SELECT_NOTHING:	if(x<0) x=0;
								else if(x>=SCREEN_RES_X) x=SCREEN_RES_X-1;
								if(y<0) y=0;
								else if(y>=SCREEN_RES_Y) y=SCREEN_RES_Y-1;
								break;		

		case SELECT_SCENE:		if(x<SCENE_Xo) x=SCENE_Xo;
								else if(x>=SCENE_Xf) x=SCENE_Xf-1;
								if(y<SCENE_Yo) y=SCENE_Yo;
								else if(y>=SCENE_Yf) y=SCENE_Yf-1;
								break;

		case SELECT_RADAR:		if(x<RADAR_Xo) x=RADAR_Xo;
								else if(x>=RADAR_Xf) x=RADAR_Xf-1;
								if(y<RADAR_Yo) y=RADAR_Yo;
								else if(y>=RADAR_Yf) y=RADAR_Yf-1;
								break;
	}
	if((x<SCENE_Xf)&&(y>=SCENE_Yo))
	{
		cx=(x-SCENE_Xo)>>5;
		cy=(y-SCENE_Yo)>>5;
	}
	else
	{
		cx=-1;
		cy=-1;
	}
	return true;
}

/******************************************************************************
cMouse::Acquire()

Acquires mouse
********************************************************************************/
bool cMouse::Acquire()
{
	cLog *Log = cLog::Instance();
	HRESULT hr;

	hr = m_pDIDev->Acquire();
	if(FAILED(hr))
	{
		Log->Error(hr,"Acquiring mouse");
		return false;
	}
	return true;
}


/******************************************************************************
cMouse::Unacquire()

Unacquires mouse
********************************************************************************/
bool cMouse::Unacquire()
{
	cLog *Log = cLog::Instance();
	HRESULT hr;

	hr = m_pDIDev->Unacquire();
	if(FAILED(hr))
	{
		Log->Error(hr,"Unacquiring mouse");
		return false;
	}
	return true;
}

/******************************************************************************
Mouse queries
********************************************************************************/
bool cMouse::ButtonDown(int button)
{
	return (m_state.rgbButtons[button] & 0x80) ? true : false;
}
bool cMouse::ButtonUp(int button)
{
	return (m_state.rgbButtons[button] & 0x80) ? false : true;
}
int  cMouse::GetWheelMovement()
{
	return m_state.lZ;
}
void cMouse::GetMovement(int *dx, int *dy)
{
	*dx = m_state.lX;
	*dy = m_state.lY;
}
void cMouse::SetPosition(int xo, int yo)
{
	x = xo;
	y = yo;
}
void cMouse::GetPosition(int *xpos, int *ypos)
{
	*xpos = x;
	*ypos = y;
}
void cMouse::GetCell(int *xcell,int *ycell)
{
	*xcell = cx;
	*ycell = cy;
}
void cMouse::SetPointer(int p)
{
	pointer = p;
}
int  cMouse::GetPointer()
{
	return pointer;
}
void cMouse::SetSelection(int s)
{
	select = s;
}
int  cMouse::GetSelection()
{
	return select;
}
void cMouse::SetSelectionPoint(int sx,int sy)
{
	select_x = sx;
	select_y = sy;
}
void cMouse::GetSelectionPoint(int *sx,int *sy)
{
	*sx = select_x;
	*sy = select_y;
}

/******************************************************************************
cMouse::GetRect()

Get sprite pointer rectangle
********************************************************************************/
void cMouse::GetRect(RECT *rc,int *posx,int *posy)
{
	switch(pointer)
	{
		case NORMAL:	SetRect(rc,0,0,32,32);					*posx=  0; *posy=  0;	break;
		case MOVE:		SetRect(rc,seq<<5, 96,(seq+1)<<5,128);	*posx=-16; *posy=-16;	break;
		case ATTACK:	SetRect(rc,seq<<5,128,(seq+1)<<5,160);	*posx=-16; *posy=-16;	break;
		case SELECT:	SetRect(rc,seq<<5, 64,(seq+1)<<5, 96);	*posx=-16; *posy=-16;	break;
		case MN:		SetRect(rc,seq<<5,160,(seq+1)<<5,192);	*posx=-16; *posy=  0;	break;
		case MS:		SetRect(rc,seq<<5,192,(seq+1)<<5,224);	*posx=-16; *posy=-31;	break;
		case ME:		SetRect(rc,seq<<5,224,(seq+1)<<5,256);	*posx=-31; *posy=-16;	break;
		case MO:		SetRect(rc,seq<<5,256,(seq+1)<<5,288);	*posx=  0; *posy=-16;	break;
		case MNE:		SetRect(rc,seq<<5,288,(seq+1)<<5,320);	*posx=-31; *posy=  0;	break;
		case MSE:		SetRect(rc,seq<<5,320,(seq+1)<<5,352);	*posx=-31; *posy=-31;	break;
		case MNO:		SetRect(rc,seq<<5,352,(seq+1)<<5,384);	*posx=  0; *posy=  0;	break;
		case MSO:		SetRect(rc,seq<<5,384,(seq+1)<<5,416);	*posx=  0; *posy=-31;	break;
	}
	delay++;
	if(delay>MDELAY)
	{
		seq++;
		if(pointer>=MN)
		{
			if(seq==16) seq=0;
		}
		else
		{
			if(seq==8) seq=0;
		}
		delay=0;
	}
}
/******************************************************************************
cMouse::In()

Mouse position in rectangle
********************************************************************************/
bool cMouse::In(int xo,int yo,int xf,int yf)
{
	return ((x>=xo)&&(x<xf)&&(y>=yo)&&(y<yf)) ? true : false;
}
/******************************************************************************
cMouse::InCell()

Mouse position in same cell
********************************************************************************/
bool cMouse::InCell(cScene *Scene,int cellx,int celly)
{
	return (((Scene->cx+cx)==cellx)&&((Scene->cy+cy)==celly)) ? 1 : 0;
}
/******************************************************************************
cMouse::InitAnim()

Init animation control variables
********************************************************************************/
void cMouse::InitAnim()
{
	seq=0;
	delay=0;
}