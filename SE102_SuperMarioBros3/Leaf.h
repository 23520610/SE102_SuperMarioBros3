#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "PlayScene.h"

#define ID_ANI_LEAF 7000
#define ID_ANI_LEAF_LEFT 7001

#define	LEAF_WIDTH 10
#define LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 14

#define LEAF_FALL_SPEED_X 0.03f
#define LEAF_SWITCH_DIRECTION_TIME 900
#define LEAF_RISE_HEIGHT 250 

class CLeaf : public CGameObject
{
protected:
	bool isBouncing = false;
	float startY;
	float vy = 0.0f;
	float vx;
	DWORD directionSwitchStart;
	bool moveRight;
	const float BOUNCE_VELOCITY = -0.08f;
	const float GRAVITY = 0.00006f;
	bool isEaten = false;
	int pointValue = 100;      
	bool isPointVisible = false;
	float pointY;
	float pointX;
	ULONGLONG pointStartTime;
	int IsBlocking() { return 0; }
	int IsCollidable() { return 1; }
public:
	CLeaf(float x, float y) : CGameObject(x, y)
	{
		this->startY = y;
		pointY = y;
		pointX = x;
		vy = -BOUNCE_VELOCITY;        
		vx = LEAF_FALL_SPEED_X;      
		moveRight = true;
		directionSwitchStart = GetTickCount64();
	}
	bool GetIsEaten() { return isEaten; }
	void Render();
	void StartBouncing();
	void OnDefeated();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};