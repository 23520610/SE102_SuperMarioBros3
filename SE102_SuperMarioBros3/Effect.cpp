#include "Effect.h"
#include "debug.h"

CEffect::CEffect(float x, float y, int ani_id, float vx_initial, float vy_initial, DWORD duration)
{
    this->x = x;
    this->y = y;
    this->ani_id = ani_id;
    this->vx = vx_initial;
    this->vy_initial = vy_initial;
    this->vy_gravity = 0.0025f;
    this->duration = duration;
    this->time_start = GetTickCount64();
}

void CEffect::Render()
{
    CAnimations::GetInstance()->Get(ani_id)->Render(x, y);
}

void CEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (ani_id == ID_ANI_BREAK_EFFECT)
    {
        vy_initial += vy_gravity * dt;
        y += vy_initial * dt;

        if (y > 600)
        {
            vy_initial = 0;
        }
    } 
}
void CEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x;
    t = y;
    r = x + 8;  
    b = y + 8;  

    // IF ELSE HERE
}