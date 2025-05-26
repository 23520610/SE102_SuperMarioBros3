#include "Brick.h"
#include "debug.h"
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
	b = y + BRICK_BBOX_HEIGHT / 2 + 0.2;
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isBouncing)
	{
		vy += GRAVITY * dt;
		y += vy * dt;
	}
	if (y >= startY)
	{
		y = round(startY);
		vy = 0;
		StopBounce();
	}
	CGameObject::Update(dt, coObjects);
}


void CBrick::StartBounce()
{
	if (!isBouncing)
	{
		vy = BOUNCE_VELOCITY;
		isBouncing = true;

		if (type == 4)
			bounceCount++;
	}
}

void CBrick::StopBounce()
{
	DebugOut(L"[DEBUG] StopBounce: y = %.3f, startY = %.3f\n", y, startY);
	isBouncing = false;
}