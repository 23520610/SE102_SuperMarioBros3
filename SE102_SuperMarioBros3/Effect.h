#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
//#include "PlayScene.h"

#define ID_ANI_BREAK_EFFECT 190000
class CEffect : public CGameObject
{
    ULONGLONG time_start;
    DWORD duration;
    int ani_id;

    //break:
    float vx_initial;
    float vy_initial; 
    float vy_gravity;
public:
    CEffect(float x, float y, int ani_id, float vx_initial, float vy_initial, DWORD duration);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();
    bool IsExpired() { return GetTickCount64() - time_start > duration; }
    void GetBoundingBox(float& l, float& t, float& r, float& b);
};