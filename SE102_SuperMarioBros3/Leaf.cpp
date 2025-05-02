#include "Leaf.h"
#include "debug.h"

void CLeaf::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_LEAF)->Render(x, y);

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
    if (isBouncing)
    {
        vy += GRAVITY * dt;

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