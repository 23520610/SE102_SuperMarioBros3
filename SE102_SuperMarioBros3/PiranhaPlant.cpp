#include "PiranhaPlant.h"
#include "Animations.h"

void CPiranhaPlant::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_PIRANHAPLANT)->Render(x, y);
	RenderBoundingBox();
}

void CPiranhaPlant::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PIRANHAPLANT_BBOX_WIDTH / 2;
	t = y - PIRANHAPLANT_BBOX_HEIGHT / 2;
	r = l + PIRANHAPLANT_BBOX_WIDTH;
	b = t + PIRANHAPLANT_BBOX_HEIGHT;
}
