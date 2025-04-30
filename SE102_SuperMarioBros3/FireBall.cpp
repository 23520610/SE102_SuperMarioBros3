#include "FireBall.h"
#include "PlayScene.h"
#include "Mario.h"
CFireBall::CFireBall(float x, float y) : CGameObject(x, y)
{
    this->ax = 0;
    this->ay = FIREBALL_GRAVITY;
    life_start = GetTickCount64();  
}

void CFireBall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x - FIREBALL_BBOX_WIDTH / 2;
    top = y - FIREBALL_BBOX_HEIGHT / 2;
    right = left + FIREBALL_BBOX_WIDTH;
    bottom = top + FIREBALL_BBOX_HEIGHT;
}

void CFireBall::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void CFireBall::OnCollisionWith(LPCOLLISIONEVENT e)
{

}

void CFireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vy += ay * dt;
    vx += ax * dt;

    if (GetTickCount64() - life_start > FIREBALL_LIFETIME)
    {
        this->Delete();
        return;
    }

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
   
}

void CFireBall::Render()
{
    int aniId = ID_ANI_FIREBALL;
    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
    //RenderBoundingBox();
}
