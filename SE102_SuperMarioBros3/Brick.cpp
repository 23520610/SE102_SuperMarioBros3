#include "Brick.h"
#include "debug.h"
void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (type!=-1)
	animations->Get(ID_ANI_BRICK)->Render(x, y);
	//RenderBoundingBox();
	if (isPointVisible)
	{
		CAnimations::GetInstance()->Get(ID_ANI_POINT_100)->Render(x, pointY);
	}
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = x + BRICK_BBOX_WIDTH / 2;
	b = y + BRICK_BBOX_HEIGHT / 2 + 0.2;
	if (this->hasButtonInside) t += 0.2;
	if (this->type == -1) t += 0.4;
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isPointVisible && GetTickCount64() - pointStartTime > 10)
	{
		if (pointY > y - 50)
			pointY -= 0.09f * dt;
		else
			pointY = y - 10;

		if (GetTickCount64() - pointStartTime > 600)
		{
			isPointVisible = false;
			//isDeleted = true;
		}

	}
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
	//DebugOut(L"[DEBUG] StopBounce: y = %.3f, startY = %.3f\n", y, startY);
	isBouncing = false;
}

void CBrick::SpawnPoint()
{
	isPointVisible = true;
	pointY = y - 8;
	pointStartTime = GetTickCount64();
}