#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "PlayScene.h"

#define ID_ANI_
class CEffect : public CGameObject
{
    ULONGLONG time_start;
    DWORD duration;
    int ani_id;
public:
    CEffect(float x, float y, int ani_id, DWORD duration = 300);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override {}
    void Render();
    bool IsExpired() { return GetTickCount64() - time_start > duration; }
};

//PREPARE FOR RACOON MARIO