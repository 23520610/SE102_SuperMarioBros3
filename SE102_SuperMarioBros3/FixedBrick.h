#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_STRIPEDBRICK 78000
#define ID_ANI_BLUEBRICK 78001

#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

class CFixedBrick : public CGameObject {

	int brickType = -1; //0: striped, 1: blue
public:
	CFixedBrick(float x, float y, int brickType) : CGameObject(x, y) 
	{
		this->brickType = brickType;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};