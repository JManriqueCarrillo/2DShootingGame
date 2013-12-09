
#ifndef __BULLET_H__
#define __BULLET_H__

#include <windows.h>
#include <list>

class cScene;

typedef struct
{
	int id;
	float x;
	float y;
	float angulo;
	bool rebota;
	int numRebotes;
}BulletStruct;

class cBullet
{
public:
	cBullet(void);
	virtual ~cBullet(void);
	
	void NewBullet ( int id, float x, float y, float fAngulo, bool rebota, int numRebotes );
	void UpdateBullets(cScene *Scene);
	bool isReadyToShoot();

	std::list<BulletStruct> listaBullets;

	float sindeg(float angle);
	float cosdeg(float angle);

private:
	int delay;
};

#endif