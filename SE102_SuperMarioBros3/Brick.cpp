#include "Brick.h"

void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BRICK)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = x + BRICK_BBOX_WIDTH / 2;
	b = y + BRICK_BBOX_HEIGHT / 2;
}