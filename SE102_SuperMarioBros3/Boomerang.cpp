#include "Boomerang.h"
#include "Mario.h"
#include "Game.h"
#include "BoomerangBrother.h"

CBoomerang::CBoomerang(float x, float y, float vx,float vy, CBoomerangBrother* _owner)
    : CGameObject() 
{
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->start_x = x;
    this->vy = vy; 
    this->ay = 0;
    flying_timer = GetTickCount64();
    isReturning = false;
    owner = _owner;
}

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGameObject::Update(dt, coObjects);

    ULONGLONG now = GetTickCount64();

    if (!isReturning && now - flying_timer > BOOMERANG_FLYING_TIME - 300 && !isFallingDown)
    {
        isFallingDown = true;
        fall_distance = y + 40; 
    }

    if (now - flying_timer > BOOMERANG_FLYING_TIME && !isReturning)
    {
        isReturning = true;
        vx = -vx;
        vy = 0; 
    }

    if (isFallingDown)
    {
        if (abs(y - fall_distance) > 0.5f)
        {
            float direction = (y < fall_distance) ? 1.0f : -1.0f;
            y += direction * 0.04f * dt;
        }
        else
        {
            y = fall_distance;
        }
    }
    //vy += ay * dt;
    x += vx * dt;
	y += vy * dt;
    if (x < 0 || x>RIGHT_LIMIT - 16)
    {
        Delete();
        owner->reduceboomerangCount();
    }

}

void CBoomerang::Render()
{
    CAnimations::GetInstance()->Get(ID_ANI_BOOMERANG)->Render(x, y);
    RenderBoundingBox();
}

void CBoomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x;
    top = y;
    right = x + BOOMERANG_BBOX_WIDTH;
    bottom = y + BOOMERANG_BBOX_HEIGHT;
}