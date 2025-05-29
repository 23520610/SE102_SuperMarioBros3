#pragma once
#include "GameObject.h"

#define PLANT_STATE_IDLE 0
#define PLANT_STATE_RISE 1
#define PLANT_STATE_ATTACK 2
#define PLANT_STATE_HIDE 3

#define PLANT_SPEED_Y 0.05f
#define PLANT_WAIT_TIME 1500
#define PLANT_PLAYER_SAFE_DISTANCE_X 32.0f
class CPlantEnemy : public CGameObject
{
protected:
	float startY;
	float maxY;
	float riseHeight;
	bool hasFired=false;
	ULONGLONG state_timer;

public:
	CPlantEnemy(float x, float y, float riseHeight);
	virtual void SetState(int state);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {}

	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 0; }

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) = 0;
	virtual void Render() = 0;
	virtual void ShootFireball();
	float getX() { return x; }
	float getY() { return y; }

};
