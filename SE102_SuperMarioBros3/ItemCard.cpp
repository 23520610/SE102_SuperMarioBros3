#include "ItemCard.h"


void CItemCard::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (state == ITEMCARD_STATE_BE_COLLECTED && y < 200.0f)
	{
		SetState(ITEMCARD_STATE_ENDINGSCENE);
		return;
	}

	if (state == ITEMCARD_STATE_ENDINGSCENE)
	{
		if (endingSpriteStart == 0)
			endingSpriteStart = GetTickCount64();
		return;
	}

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
		if (isMushroom) ani = ID_ANI_ITEMCARD_MUSHROOM;
		else if (isStar) ani = ID_ANI_ITEMCARD_STAR;
		else if (isFlower) ani = ID_ANI_ITEMCARD_FLOWER;
		break;
	case ITEMCARD_STATE_ENDINGSCENE:
		if (isMushroom) ani = ID_ANI_ITEMCARD_MUSHROOM;
		else if (isStar) ani = ID_ANI_ITEMCARD_STAR;
		else if (isFlower) ani = ID_ANI_ITEMCARD_FLOWER;
		break;
	default:
		break;
	}
	if (state == ITEMCARD_STATE_ENDINGSCENE && GetTickCount64() - endingSpriteStart < 3000&&endingSpriteStart>0)
	{
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		CMario* mario = (CMario*)scene->GetPlayer();
		float px, py;
		mario->GetPosition(px, py);
		float hudX, hudY;
		CHud* hud = scene->GetHud();
		hud->GetPosition(hudX, hudY);
		CSprites::GetInstance()->Get(ID_SPRITE_ENDING)->Draw(hudX, hudY - 180);
		if (mario->GetWorld() == 1) {
			if(px>2815)
			mario->SetPosition(2815, py);
		}
		else {
			if (px > 2560)
				mario->SetPosition(2560, py);
		}
	}
	if (ani != -1)
		CAnimations::GetInstance()->Get(ani)->Render(x, y);
}
void CItemCard::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == ITEMCARD_STATE_BE_COLLECTED || state == ITEMCARD_STATE_ENDINGSCENE)
	{
		l = t = r = b = 0;
		return;
	}
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

	case ITEMCARD_STATE_ENDINGSCENE:
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		float hudX, hudY;
		CHud* hud = scene->GetHud();
		hud->GetPosition(hudX, hudY);
		x =hudX + 62;
		y = hudY - 164;
		vy = 0;
		break;
	}

	CGameObject::SetState(state);
}

void CItemCard::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}
