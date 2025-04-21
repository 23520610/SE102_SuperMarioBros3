#pragma once
#include "GameObject.h"

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16
#define ID_ANI_MUSHROOM 50000
class CMushroom : public CGameObject
{
public:
    CMushroom(float x, float y) : CGameObject(x, y)
    {
        this->x = x;
        this->y = y;
    }
    //void Render();
    void Update(DWORD dt) {}
    void GetBoundingBox(float& l, float& t, float& r, float& b);
};

