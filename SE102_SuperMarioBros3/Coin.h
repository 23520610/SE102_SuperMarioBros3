#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "PlayScene.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

class CCoin : public CGameObject
{
	bool isBouncing = false;
	float startY;
	float vy = 0.0f;
	const float BOUNCE_VELOCITY = -0.3f;
	const float GRAVITY = 0.001f;
public:
	CCoin(float x, float y) : CGameObject(x, y)
	{
		this->startY = y;
		//this->vy = BOUNCE_VELOCITY;
		//this->isBouncing = true;
	}
	void Render();
	void StartBouncing();
	//void Update(DWORD dt);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};