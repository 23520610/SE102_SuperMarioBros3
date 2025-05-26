#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_GOLDBRICK_STATE_BROKEN 77009
#define ID_ANI_BRICK 77000
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

class CBrick : public CGameObject {
	bool hasButtonInside = false;
	int type = -1; //0: binh thuong, 1: khong the pha, 2: chua button 3: chua qb 1up, 4: bounce vang; 5: chua qb 

	float startY;         
	float vy = 0;
	bool isBouncing; 

	const float BOUNCE_VELOCITY = -0.1f;
	const float GRAVITY = 0.0005f;
	const float MAX_BOUNCE_HEIGHT = 8.0f;

	int bounceCount = 0;
	const int MAX_BOUNCE = 5;

public:
	CBrick(float x, float y, int type) : CGameObject(x, y) 
	{
		this->x = x;
		this->y = y;
		this->startY = y;
		this->isBouncing = false;
		this->type = type;
		if (this->type == 2)
			hasButtonInside = true;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	float GetX() { return x; }
	float GetY() { return y; }
	int IsCollidable() override { return 1; }
	int IsBlocking() override { return 1; }
	bool GetButton() { return hasButtonInside; }
	int GetType() { return type; }
	void SetType(int t) { type = t; }

	void StartBounce();
	void StopBounce();

	bool getIsBouncing() { return isBouncing; }
	void setIsBouncing(bool isBouncing) { this->isBouncing = isBouncing; }

	int GetBounceCount() { return bounceCount; }
	//void SetBounceCount(int c){}
};