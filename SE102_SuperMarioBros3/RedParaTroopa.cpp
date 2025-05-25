#include "RedParaTroopa.h"

CRedParaTroopa::CRedParaTroopa(float x, float y, float spawnX)
    : CParaTroopa(x, y, spawnX)
{
    vx = 0;
    ay = 0;        
    vy = -RED_PARATROOPA_SPEED_Y; 
    fly_timer = GetTickCount64();
}

void CRedParaTroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

    if (GetTickCount64() - fly_timer >= RED_PARATROOPA_INTERVAL)
    {
        vy = -vy;
        fly_timer = GetTickCount64();
    }

    CGameObject::Update(dt, coObjects);
    y += vy * dt;

}
void CRedParaTroopa::Render()
{
    if (!isActive)
        return;

    int aniId = ID_ANI_RED_PARA_TROOPA_FLY;
    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}
