#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxerr8.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"XInput.lib")        // Library containing necessary 360

#include <D3D9.h>
#include <D3DX9.h>
#include <XInput.h>

class cController
{
public:
	cController(void);
	virtual ~cController(void);

	bool Read();
	bool buttonDown(int controller, int button);
	float getLeftStickX(int controller);
	float getLeftStickY(int controller);

private:
	XINPUT_STATE state[4];     // Data type that stores all the current states of a controller.
};

#endif