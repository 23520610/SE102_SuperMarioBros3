#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "PlayScene.h"

#define ID_ANI_LEAF 7000

#define	LEAF_WIDTH 10
#define LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 14

#define LEAF_FALL_SPEED_X 0.05f
#define LEAF_SWITCH_DIRECTION_TIME 600
#define LEAF_RISE_HEIGHT 260 

class CLeaf : public CGameObject
{
	bool isBouncing = false;
	float startY;
	float vy = 0.0f;
	float vx;
	DWORD directionSwitchStart;
	bool moveRight;
	const float BOUNCE_VELOCITY = -0.1f;
	const float GRAVITY = 0.00006f;

	int pointValue = 100;      
	bool isPointVisible = false;
	float pointY;
	ULONGLONG pointStartTime;
public:
	CLeaf(float x, float y) : CGameObject(x, y)
	{
		this->startY = y;
		vy = -BOUNCE_VELOCITY;        
		vx = LEAF_FALL_SPEED_X;      
		moveRight = true;
		directionSwitchStart = GetTickCount64();
	}
	void Render();
	void StartBouncing();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};