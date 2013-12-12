#include "cController.h"

cController::cController()
{

}

cController::~cController()
{
	
}

bool cController::Read()
{
	DWORD dwResult;         // Used to store if a controller is connected
	int i;

	for(i=0;i<4;i++)
	{
		ZeroMemory( &(state[i]), sizeof(XINPUT_STATE) ); // Zeros out the states of the controller.
 
		// Get all the current states of controller 1
		dwResult = XInputGetState( i, &state[i] );
 
		if( dwResult == ERROR_SUCCESS )
		{
			// Controller is connected.
			// -----------INSERT BUTTON CHECKS HERE----------
		}
		else
		{
			// Controller is disconnected.
		}
	}

	return true;
}

/*
#define XINPUT_GAMEPAD_DPAD_UP          0x00000001
#define XINPUT_GAMEPAD_DPAD_DOWN        0x00000002
#define XINPUT_GAMEPAD_DPAD_LEFT        0x00000004
#define XINPUT_GAMEPAD_DPAD_RIGHT       0x00000008
#define XINPUT_GAMEPAD_START            0x00000010
#define XINPUT_GAMEPAD_BACK             0x00000020
#define XINPUT_GAMEPAD_LEFT_THUMB       0x00000040
#define XINPUT_GAMEPAD_RIGHT_THUMB      0x00000080
#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
#define XINPUT_GAMEPAD_A                0x1000
#define XINPUT_GAMEPAD_B                0x2000
#define XINPUT_GAMEPAD_X                0x4000
#define XINPUT_GAMEPAD_Y                0x8000
*/
bool cController::buttonDown(int controller, int button)
{
	return state[controller].Gamepad.wButtons & button;
}

float cController::getLeftStickX(int controller)
{
	if(state[controller].Gamepad.sThumbLX < 5000.0f && state[controller].Gamepad.sThumbLX > -5000.0f) return 0.0f;
	return state[controller].Gamepad.sThumbLX / 32767.0f;
}

float cController::getLeftStickY(int controller)
{
	if(state[controller].Gamepad.sThumbLY < 5000.0f && state[controller].Gamepad.sThumbLY > -5000.0f) return 0.0f;
	return state[controller].Gamepad.sThumbLY / 32767.0f;
}