#include "RedPlant.h"
#include "Animations.h"

void CRedPlant::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_REDPLANT)->Render(x, y);
	//RenderBoundingBox();
}

void CRedPlant::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - REDPLANT_BBOX_WIDTH / 2;
	t = y - REDPLANT_BBOX_HEIGHT / 2;
	r = l + REDPLANT_BBOX_WIDTH;
	b = t + REDPLANT_BBOX_HEIGHT;
}
