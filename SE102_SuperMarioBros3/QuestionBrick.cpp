#include "QuestionBrick.h"
#include "Animations.h"

void CQuestionBrick::Render()
{
	int aniId;

	if (this->GetState() == 90000)
		aniId = 30001;
	else
		aniId = 30000;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isBouncing)
	{
		vy += GRAVITY * dt;
		y += vy * dt;

		if (y >= startY)
		{
			y = startY;
			vy = 0;
			isBouncing = false;
		}
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
