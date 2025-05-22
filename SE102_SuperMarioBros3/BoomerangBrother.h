#pragma once
#include "GameObject.h"

#define BOOMERANGBROTHER_GRAVITY 0.002f
#define BOOMERANGBROTHER_WALKING_SPEED 0.05f


#define BOOMERANGBROTHER_BBOX_WIDTH 16
#define BOOMERANGBROTHER_BBOX_HEIGHT 22
#define BOOMERANGBROTHER_BBOX_HEIGHT_DIE 7

#define BOOMERANGBROTHER_DIE_TIMEOUT 500

#define BOOMERANGBROTHER_STATE_WALKING 100
#define BOOMERANGBROTHER_STATE_DIE 200
#define BOOMERANGBROTHER_STATE_SUPER_DIE 250
#define BOOMERANGBROTHER_STATE_BOUNCE 300
#define BOOMERANGBROTHER_STATE_THWOW 350

#define ID_ANI_BOOMERANGBROTHER_WALKING 301100
#define ID_ANI_BOOMERANGBROTHER_DIE 301101
#define ID_ANI_BOOMERANGBROTHER_SUPER_DIE 301102
#define ID_ANI_BOOMERANGBROTHER_THROW 301103

#define	ID_ANI_POINT_100	140000
#define	ID_ANI_POINT_200	140001

#define BOOMERANGBROTHER_SUPER_DIE_VX 1.5f
#define BOOMERANGBROTHER_SUPER_DIE_VY 0.5f
class CBoomerangBrother : public CGameObject
{
protected:
	float ax;
	float ay;

	bool isActive = false;
	float spawnX;
	float startX;
	float startY;
	const float BOUNCE_VELOCITY = -0.5f;
	bool isBouncing = false;

	float maxDistanceX = 20.0f;
	int pointValue = 100;
	bool isPointVisible = false;
	float pointY;
	ULONGLONG pointStartTime;

	ULONGLONG die_start;
	ULONGLONG super_die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() {
		if (state == BOOMERANGBROTHER_STATE_DIE)return 0;
		return 1;
	}
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CBoomerangBrother(float x, float y, float spawnX);
	virtual void SetState(int state);
	void StartBouncing();
	void OnDefeated();
	float GetX() { return x; }
	float GetY() { return y; }
};