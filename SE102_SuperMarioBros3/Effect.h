#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "PlayScene.h"

class CEffect : public CGameObject
{
    ULONGLONG time_start;
    DWORD duration;
public:
    CEffect(float x, float y, int ani_id, DWORD duration = 300);

    //void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    //void Render();
    bool IsExpired() { return GetTickCount64() - time_start > duration; }
};
