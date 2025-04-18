#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define PIPE_TYPE_DEFAULT 0      // Pipe thường
#define PIPE_TYPE_DOWN 1         // Pipe có thể chui xuống
#define PIPE_TYPE_PIRANHA 2      // Pipe có cây piranha
#define PIPE_TYPE_VENUS_FIRE_TRAP 3 //Pipe có Venus


#define ID_ANI_PIPE_DEFAULT 12000
#define ID_ANI_PIPE_PIRANHA 12001

#define PIPE_WIDTH 32
#define PIPE_BBOX_WIDTH 32
#define PIPE_BBOX_HEIGHT 48
#define PIPE_TYPE_PIRANHA_BBOX_HEIGHT 32

class CPipe : public CGameObject {
	int pipeType;
public:
	CPipe(float x, float y, int type) : CGameObject(x, y)
	{
		this->pipeType = type;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsCollidable() override { return 1; }
	int IsBlocking() override { return 1; }
};
