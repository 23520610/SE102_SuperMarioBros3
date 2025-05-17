#include "ParaTroopa.h"
#include "debug.h"
#include "FireBall.h"
#include "Goomba.h"
#include "Coin.h"

CParaTroopa::CParaTroopa(float x, float y, float spawnX) : CGameObject(x, y)
{
    isActive = false;
    hasWings = true;
    this->spawnX = spawnX;
    ay = PARA_TROOPA_GRAVITY;
    SetState(WING_PARATROOPA);
    last_action_time = GetTickCount64();
}

void CParaTroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (!isActive)
    {
        float camX, camY;
        CGame::GetInstance()->GetCamPos(camX, camY);

        if (camX + CGame::GetInstance()->GetBackBufferWidth() / 2 >= this->spawnX)
            this->isActive = true;
        else
            return;
    }

    vy += ay * dt;

    if (hasWings && isOnGround && GetTickCount64() - last_action_time > PARATROOPA_JUMP_INTERVAL)
    {
        vy = -PARATROOPA_JUMP_SPEED;
        last_action_time = GetTickCount64();
        isOnGround = false; 
    }


    isOnGround = false;
    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CParaTroopa::Render()
{
    if (!isActive) {
        //DebugOut(L"[PARATROOPA] Render blocked due to inactive\n");
        return;
    }
    int aniId = -1;

    if (this->GetState() == WING_PARATROOPA)
    {
        aniId = ID_ANI_PARA_TROOPA_WALKLEFT;
    }

    if (aniId != -1) {
        CAnimations::GetInstance()->Get(aniId)->Render(x, y);
    }
    //RenderBoundingBox();
}

void CParaTroopa::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state)
    {
    case WING_PARATROOPA:
        vx = -0.03f; 
        break;
    }
}

void CParaTroopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
     left = x - PARA_TROOPAWING_BBOX_WIDTH / 2;
     top = y - PARA_TROOPAWING_BBOX_HEIGHT / 2;
     right = left + PARA_TROOPAWING_BBOX_WIDTH;
     bottom = top + PARA_TROOPAWING_BBOX_HEIGHT;

}

void CParaTroopa::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
};


void CParaTroopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (dynamic_cast<CParaTroopa*>(e->obj)) return;
    if (dynamic_cast<CFireBall*>(e->obj)) return;
    if (dynamic_cast<CCoin*>(e->obj)) return;
    //if (!e->obj->IsBlocking()) return;
    if (dynamic_cast<CKoopas*>(e->obj)) return;

    if (e->ny != 0)
    {
        vy = 0;
        isOnGround = true;
    }
}