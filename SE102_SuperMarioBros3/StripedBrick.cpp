#include "StripedBrick.h"

void CStripedBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_STRIPEDBRICK)->Render(x, y);
	//RenderBoundingBox();
}

void CStripedBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - STRIPEDBRICK_BBOX_WIDTH / 2;
	t = y - STRIPEDBRICK_BBOX_HEIGHT / 2;
	r = l + STRIPEDBRICK_BBOX_WIDTH;
	b = t + STRIPEDBRICK_BBOX_HEIGHT;
}