#include "Boomerang.h"
#include "Mario.h"
#include "Game.h"
#include "BoomerangBrother.h"

CBoomerang::CBoomerang(float x, float y, float vx, CBoomerangBrother* _owner)
    : CGameObject() 
{
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->start_x = x;
    flying_timer = GetTickCount64();
    isReturning = false;
    owner = _owner;
}

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGameObject::Update(dt, coObjects);

    if (GetTickCount64() - flying_timer > BOOMERANG_FLYING_TIME && !isReturning)
    {
        isReturning = true;
        vx = -vx;
    }

    x += vx * dt;

    if (x < 0 || x>RIGHT_LIMIT - 16)
    {
        Delete();
        owner->reduceboomerangCount();
    }

}

void CBoomerang::Render()
{
    CAnimations::GetInstance()->Get(ID_ANI_BOOMERANG)->Render(x, y);
    //RenderBoundingBox();
}

void CBoomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x;
    top = y;
    right = x + BOOMERANG_BBOX_WIDTH;
    bottom = y + BOOMERANG_BBOX_HEIGHT;
}