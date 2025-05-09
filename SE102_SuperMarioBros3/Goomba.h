#pragma once
#include "GameObject.h"

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.05f


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_SUPER_DIE 250
#define GOOMBA_STATE_BOUNCE 300

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001
#define ID_ANI_GOOMBA_SUPER_DIE 5002

#define	ID_ANI_POINT_100	140000
#define	ID_ANI_POINT_200	140001

#define GOOMBA_SUPER_DIE_VX 1.5f
#define GOOMBA_SUPER_DIE_VY 0.5f
class CGoomba : public CGameObject
{
protected:
	float ax;				
	float ay; 

	bool isActive = false;
	float spawnX;

	float startY;
	const float BOUNCE_VELOCITY = -0.5f;
	bool isBouncing = false;

	int pointValue = 100;        
	bool isPointVisible = false;
	float pointY;
	ULONGLONG pointStartTime;

	ULONGLONG die_start;
	ULONGLONG super_die_start;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public: 	
	CGoomba(float x, float y, float spawnX);
	virtual void SetState(int state);
	void StartBouncing();
	void OnDefeated();
	float GetX() { return x; }
	float GetY() { return y; }
};