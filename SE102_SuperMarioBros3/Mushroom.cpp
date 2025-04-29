#include "Mushroom.h"
#include "Animations.h"
#include "GameObject.h"
#include "debug.h"
#include "PlayScene.h"
#include "Collision.h"
#include "Utils.h"
#include "AssetIDs.h"

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    
    if (state == MUSHROOM_STATE_EMERGE)
    {
        y += vy * dt; 

        if (startY - y >= MUSHROOM_BBOX_HEIGHT / 2)
        {
            state = MUSHROOM_STATE_WALK;
            y = startY - MUSHROOM_BBOX_HEIGHT;
            vy = 0;
            vx = MUSHROOM_WALK_SPEED_X;
        }
    }
    else if (state == MUSHROOM_STATE_WALK)
    {
        vy += MUSHROOM_GRAVITY * dt;
    }
    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroom::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void CMushroom::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e)
{
    if (e->ny != 0 && e->obj->IsBlocking())
    {
        vy = 0;
    }
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!e->obj->IsBlocking()) return; 
    if (e->ny != 0 && e->obj->IsBlocking())
    {

        vy = 0;
    }
    else
    if (e->nx != 0 && e->obj->IsBlocking())
    {
        vx = -vx;
    }
    if (dynamic_cast<CQuestionBrick*>(e->obj))
        OnCollisionWithQuestionBrick(e);
}

void CMushroom::Render() {
    //RenderBoundingBox();
    CAnimations* animations = CAnimations::GetInstance();
    animations->Get(ID_ANI_MUSHROOM)->Render(x, y);
}

void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b) {
    l = x - MUSHROOM_BBOX_WIDTH / 2;
    t = y - MUSHROOM_BBOX_HEIGHT / 2;
    r = x + MUSHROOM_BBOX_WIDTH / 2;
    b = y + MUSHROOM_BBOX_HEIGHT / 2;
}