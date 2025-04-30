#pragma once
#include "GameObject.h"

#define FIREBALL_GRAVITY 0.0000000001f
#define FIREBALL_SPEED 0.2f
#define FIREBALL_LIFETIME 10000 // tồn tại 2 giây

#define ID_ANI_FIREBALL 160000

#define FIREBALL_BBOX_WIDTH 8
#define FIREBALL_BBOX_HEIGHT 8

class CFireBall : public CGameObject
{
protected:
    float ax;
    float ay;
    ULONGLONG life_start; // thời điểm bắt đầu sống

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();
    virtual int IsCollidable() { return 0; };
    virtual int IsBlocking() { return 0; }
    virtual void OnNoCollision(DWORD dt);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
    CFireBall(float x, float y);
    void SetSpeed(float vx, float vy)
    {
        this->vx = vx;
        this->vy = vy;
    }

};
