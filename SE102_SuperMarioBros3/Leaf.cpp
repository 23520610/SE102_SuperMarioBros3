#include "Leaf.h"
#include "debug.h"

void CLeaf::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
    if (!isEaten)
    {
        if (!moveRight)
            animations->Get(ID_ANI_LEAF)->Render(x, y);
        else
            animations->Get(ID_ANI_LEAF_LEFT)->Render(x, y);
    }
    if (isPointVisible)
    {
        animations->Get(ID_ANI_POINT_1000)->Render(pointX, pointY);
    }

	//RenderBoundingBox();
}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LEAF_BBOX_WIDTH / 2;
	t = y - LEAF_BBOX_HEIGHT / 2;
	r = l + LEAF_BBOX_WIDTH;
	b = t + LEAF_BBOX_HEIGHT;
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (isPointVisible)
    {
        //DebugOut(L"[POINT] Done bouncing! y = %.2f\n", y);
        if (pointY > y - 80)
            pointY -= 0.05f * dt;
        else
            pointY = y - 10;

        if (GetTickCount64() - pointStartTime > 800)
        {
            isPointVisible = false;
            isDeleted = true;
        }
    }
    if (isBouncing)
    {
        vy += GRAVITY  * dt ;

        if (GetTickCount64() - directionSwitchStart > LEAF_SWITCH_DIRECTION_TIME)
        {
            moveRight = !moveRight;
            vx = moveRight ? LEAF_FALL_SPEED_X : -LEAF_FALL_SPEED_X;
            directionSwitchStart = GetTickCount64();
        }

        x += vx * dt;
        y += vy * dt;

        if (y >= startY + LEAF_RISE_HEIGHT)
        {
            isBouncing = false;
            vx = 0;
            vy = 0;
        }
    }
}

void CLeaf::StartBouncing()
{
	if (!isBouncing)
	{
		vy = BOUNCE_VELOCITY;
		startY = y;
		isBouncing = true;
	}
}

void CLeaf::OnDefeated()
{
    isPointVisible = true;
    pointY = y;
    pointX = x;
    isEaten = true;
    pointStartTime = GetTickCount64();
}