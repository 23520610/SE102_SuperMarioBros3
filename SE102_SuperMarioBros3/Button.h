#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BUTTON 171000
#define ID_ANI_BUTTON_BROKEN 171001

#define BUTTON_WIDTH 16
#define BUTTON_BBOX_WIDTH 16
#define BUTTON_BBOX_HEIGHT 16
#define BUTTON_BROKEN_BBOX_HEIGHT 7

class CButton : public CGameObject {
protected:
	bool isBroken = false;
	float targetY;
	bool isSpawning = false;

public:
	CButton(float x, float y) : CGameObject(x, y)
	{
		targetY = y -10;
		this->isBroken = false;
		vy = -0.05f;
		isSpawning = true;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	float GetX() { return x; }
	float GetY() { return y; }

	void SetBroken(bool broken) { this->isBroken = broken; }
	bool IsBroken() const { return isBroken; }

	static void SpawnButton(float x, float y);

	bool GetIsSpawn() { return isSpawning; }

	int IsCollidable() override { return 1; }
	int IsBlocking() override { return 0; }
};