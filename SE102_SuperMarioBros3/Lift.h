#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Animations.h"
#include "PlayScene.h"

#define LIFT_BBOX_WIDTH 48
#define LIFT_BBOX_HEIGHT 16

#define ID_ANI_LIFT 290001
#define LIFT_SPEED_X 0.04f
#define LIFT_GRAVITY 0.0001f
#define LIFT_MAX_Y	432
class CLift : public CGameObject
{
protected:
	bool isTouched;
	float spawnX;
	float ax, ay;
	float minX;
	float maxY;
	bool isActive = false;
public:
	CLift(float x, float y, float _spawnX)
		: CGameObject(x, y)
		, spawnX(_spawnX)
		, isActive(false)
	{
		this->ax = 0;
		this->ay = 0;
		isTouched = false;
		maxY = 420.0f;
	}

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 1; }
	int IsCollidable() { return 0; }
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void setIsTouched() { this->isTouched = isTouched; }
	void TriggerFall();
	float getY() { return y; }
	float getVy() { return vy; }
};

