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
public:
	CBrick(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	float GetX() { return x; }
	float GetY() { return y; }
	int IsCollidable() override { return 1; }
	int IsBlocking() override { return 1; }
};