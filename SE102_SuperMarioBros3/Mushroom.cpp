#include "Mushroom.h"
#include "Animations.h"


//void CMushroom::Render()
//{
//	CAnimations* animations = CAnimations::GetInstance();
//	animations->Get(ID_ANI_MUSHROOM)->Render(x, y);
//}

//void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
//{
//	// Không cần cập nhật gì
//}

void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MUSHROOM_BBOX_WIDTH / 2;
	t = y - MUSHROOM_BBOX_HEIGHT / 2;
	r = x + MUSHROOM_BBOX_WIDTH / 2;
	b = y + MUSHROOM_BBOX_HEIGHT / 2;
}