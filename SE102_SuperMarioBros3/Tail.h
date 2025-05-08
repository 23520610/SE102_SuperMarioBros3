#pragma once
#include "GameObject.h"

#define TAIL_BBOX_WIDTH 20
#define TAIL_BBOX_HEIGHT 10
#define TAIL_DURATION 350 

class CTail : public CGameObject
{
protected:
    ULONGLONG attack_start; 
    bool isAttacking; 

public:
    CTail(float x, float y, int nx);
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override { RenderBoundingBox(); } 
    void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
    void StartAttacking() { isAttacking = true; attack_start = GetTickCount64(); }
    bool IsAttacking() { return isAttacking; }
    void UpdatePosition(float mario_x, float mario_y, int mario_nx);
    void OnCollisionWith(LPCOLLISIONEVENT e); 
	int IsCollidable() override { return isAttacking ? 1 : 0; }//chỉ va chạm khi đang tấn công
    int IsBlocking() override { return 0; }
    void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
    void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
    //void OnCollisionWithPLant(LPCOLLISIONEVENT e);
    void OnCollisionWithGoldBrick(LPCOLLISIONEVENT e);
};