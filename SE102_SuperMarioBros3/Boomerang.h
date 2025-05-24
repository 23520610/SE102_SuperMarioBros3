#pragma once
#include "GameObject.h"

class CBoomerangBrother;
#define BOOMERANG_SPEED 0.1f
#define BOOMERANG_BBOX_WIDTH 16
#define BOOMERANG_BBOX_HEIGHT 8
#define BOOMERANG_FLYING_TIME 1500

#define ID_ANI_BOOMERANG 311200

class CBoomerang : public CGameObject
{
protected:
    float start_x;
    ULONGLONG flying_timer;
    bool isReturning;
    CBoomerangBrother* owner;

public:
    CBoomerang(float x, float y, float vx, CBoomerangBrother* _owner); 
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    bool IsReturning() { return isReturning; }
    float getX() { return x; }
    float getY() { return y; }
};