#include "FixedBrick.h"

void CFixedBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (this->brickType == 0)
		animations->Get(ID_ANI_STRIPEDBRICK)->Render(x, y);
	if (this->brickType == 1)
		animations->Get(ID_ANI_BLUEBRICK)->Render(x, y);
	//RenderBoundingBox();
}

void CFixedBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}