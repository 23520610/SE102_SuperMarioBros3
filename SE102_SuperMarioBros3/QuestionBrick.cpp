#include "QuestionBrick.h"
#include "Animations.h"

void CQuestionBrick::Render()
{
	//RenderBoundingBox();
	int aniId;
	if (this->GetState() == 90000)
		aniId = 30001;
	else
		aniId = 30000;

	if (isPointVisible)
	{
		CAnimations::GetInstance()->Get(ID_ANI_POINT_100)->Render(x, pointY);
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}


void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (itemType == -1) this->SetState(90000);
	if (isPointVisible && GetTickCount64() - pointStartTime>400)
	{
		if (pointY > y - 60)
			pointY -= 0.08f * dt;
		else
			pointY = y - 10;

		if (GetTickCount64() - pointStartTime > 800)
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
		y = startY;
		vy = 0;
		StopBounce();
	}
}

void CQuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QBRICK_BBOX_WIDTH / 2;
	t = y - QBRICK_BBOX_HEIGHT / 2;
	r = x + QBRICK_BBOX_WIDTH / 2;
	b = y + QBRICK_BBOX_HEIGHT / 2;
}

void CQuestionBrick::StartBounce()
{
	if (!isBouncing)
	{
		vy = BOUNCE_VELOCITY;
		isBouncing = true;
	}
}

void CQuestionBrick::StopBounce()
{
	isBouncing = false;
}

void CQuestionBrick::SpawnPoint()
{
	isPointVisible = true;
	pointY = y-8;
	pointStartTime = GetTickCount64();
}