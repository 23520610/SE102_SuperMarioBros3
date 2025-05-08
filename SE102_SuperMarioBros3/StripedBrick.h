#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_STRIPEDBRICK 78000
#define STRIPEDBRICK_WIDTH 16
#define STRIPEDBRICK_BBOX_WIDTH 16
#define STRIPEDBRICK_BBOX_HEIGHT 16

class CStripedBrick : public CGameObject {
public:
	CStripedBrick(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};