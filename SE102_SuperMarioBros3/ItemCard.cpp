#include "ItemCard.h"
void CItemCard::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == ITEMCARD_STATE_BE_COLLECTED&&y<200.0f) this->Delete();
	if (start_change == 0)
	{
		start_change = GetTickCount64();
	}
	else
	{
		if (GetTickCount64() - start_change > ITEMCARD_TIME_CHANGE)
		{
			if (state == ITEMCARD_STATE_MUSHROOM)
			{
				SetState(ITEMCARD_STATE_STAR);
			}
			else if (state == ITEMCARD_STATE_STAR)
			{
				SetState(ITEMCARD_STATE_FLOWER);
			}
			else if (state == ITEMCARD_STATE_FLOWER)
			{
				SetState(ITEMCARD_STATE_MUSHROOM);
			}
			start_change = GetTickCount64();
		}
	}
	y += vy * dt;	
	CGameObject::Update(dt);
}
void CItemCard::Render()
{
	int ani = -1;
	switch (state)
	{
	case ITEMCARD_STATE_MUSHROOM:
		ani = ID_ANI_ITEMCARD_MUSHROOM;
		break;
	case ITEMCARD_STATE_STAR:
		ani = ID_ANI_ITEMCARD_STAR;
		break;
	case ITEMCARD_STATE_FLOWER:
		ani = ID_ANI_ITEMCARD_FLOWER;
		break;
	case ITEMCARD_STATE_BE_COLLECTED:
		if (isMushroom)
			ani = ID_ANI_ITEMCARD_MUSHROOM;
		else if (isStar)
			ani = ID_ANI_ITEMCARD_STAR;
		else if (isFlower)
			ani = ID_ANI_ITEMCARD_FLOWER;
		break;
	default:
		break;
	}
	CAnimations::GetInstance()->Get(ani)->Render(x, y);
	//RenderBoundingBox();
}
void CItemCard::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - ITEMCARD_BBOX_WIDTH / 2;
	t = y - ITEMCARD_BBOX_HEIGHT / 2;
	r = l + ITEMCARD_BBOX_WIDTH;
	b = t + ITEMCARD_BBOX_HEIGHT;
}
void CItemCard::SetState(int state)
{
	switch (state)
	{
	case ITEMCARD_STATE_MUSHROOM:
		start_change = GetTickCount64();
		isMushroom = 1;
		isStar = 0;
		isFlower = 0;
		break;

	case ITEMCARD_STATE_STAR:
		start_change = GetTickCount64();
		isMushroom = 0;
		isStar = 1;
		isFlower = 0;
		break;

	case ITEMCARD_STATE_FLOWER:
		start_change = GetTickCount64();
		isMushroom = 0;
		isStar = 0;
		isFlower = 1;
		break;

	case ITEMCARD_STATE_BE_COLLECTED:
		if (isMushroom) type = 0;
		else if (isStar) type = 1;
		else if (isFlower) type = 2;
		vy = -ITEMCARD_BOUNCE_SPEED;
		break;
	}

	CGameObject::SetState(state);
}

void CItemCard::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}
