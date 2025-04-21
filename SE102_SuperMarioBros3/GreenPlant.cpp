#include "GreenPlant.h"
#include "Animations.h"

void CGreenPlant::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_GREENPLANT)->Render(x, y);
	//RenderBoundingBox();
}

void CGreenPlant::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - GREENPLANT_BBOX_WIDTH / 2;
	t = y - GREENPLANT_BBOX_HEIGHT / 2;
	r = l + GREENPLANT_BBOX_WIDTH;
	b = t + GREENPLANT_BBOX_HEIGHT;
}
