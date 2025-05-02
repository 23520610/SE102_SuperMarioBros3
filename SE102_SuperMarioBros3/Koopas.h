#pragma once
#include "GameObject.h"

#define KOOPAS_GRAVITY 0.002f
#define KOOPAS_WALKING_SPEED 0.025f


#define KOOPAS_BBOX_WIDTH_WALK 16
#define KOOPAS_BBOX_HEIGHT_WALK 27
#define KOOPAS_BBOX_HEIGHT_HIT 16
#define KOOPAS_BBOX_WIDTH_HIT 16
#define KOOPAS_BBOX_HEIGHT_REVIVE 16
#define KOOPAS_BBOX_WIDTH_REVIVE 16

#define KOOPAS_REVIVE_TIMEOUT 5000 
#define KOOPAS_DIE_TIMEOUT 500

#define KOOPAS_STATE_WALKING 6003
#define KOOPAS_STATE_HIT 6004
#define KOOPAS_STATE_REVIVE 6005
#define KOOPAS_STATE_DIE 6006
#define KOOPAS_STATE_HIT_MOVING 6007

#define ID_ANI_KOOPAS_WALKING_LEFT 6000
#define ID_ANI_KOOPAS_WALKING_RIGHT 6001
#define ID_ANI_KOOPAS_HIT 6002
#define ID_ANI_KOOPAS_REVIVE 6003
#define ID_ANI_KOOPAS_HIT_MOVING 6004

class CKoopas : public CGameObject
{
protected:
	float ax;
	float ay;

	bool isActive = false;
	float spawnX;

	ULONGLONG hit_start;
	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e);

public:
	bool IsNearEdge(vector<LPGAMEOBJECT>* coObjects);
	CKoopas(float x, float y, float spawnX);
	virtual void SetState(int state);
};