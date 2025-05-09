#pragma once
#include "Goomba.h"
#include "FireBall.h"
#include "PlayScene.h"

#define PARA_GOOMBA_SMALL_HOP_SPEED  0.2f
#define PARA_GOOMBA_BIG_JUMP_SPEED   0.5f 
#define NUM_SMALL_HOPS 3
#define PARA_GOOMBA_HOP_INTERVAL 300 
#define PARA_GOOMBA_WAIT_TIME 400
#define PARA_GOOMBA_GRAVITY GOOMBA_GRAVITY

#define PARA_GOOMBAWING_BBOX_WIDTH 20
#define PARA_GOOMBAWING_BBOX_HEIGHT 18
#define PARA_GOOMBA_FLYING_BBOX_HEIGHT 23
#define PARA_GOOMBA_BBOX_WIDTH GOOMBA_BBOX_WIDTH
#define PARA_GOOMBA_BBOX_HEIGHT GOOMBA_BBOX_HEIGHT
#define PARA_GOOMBA_BBOX_HEIGHT_DIE GOOMBA_BBOX_HEIGHT_DIE

#define GOMBA_STATE_HOPPING 300
#define GOMBA_STATE_FLYING 400

#define ID_ANI_PARA_GOOMBA_WALK 170000
#define ID_ANI_PARA_GOOMBA_WALKING_WINGED 170001
#define ID_ANI_PARA_GOOMBA_HOPING 170002
#define ID_ANI_PARA_GOOMBA_JUMPING 170003
#define ID_ANI_PARA_GOOMBA_DIE 170004
#define ID_ANI_PARA_GOOMBA_SUPER_DIE 170005
class CParaGoomba : public CGoomba
{
protected:
    bool hasWings;
    bool isOnGround = false;
    int hopCount = 0; // 0: chờ, 1-3: nhảy nhỏ, 4: nhảy lớn, 5: chờ 400ms
    ULONGLONG last_action_time = 0;

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    virtual void Render() override;
    virtual void SetState(int state) override;

    virtual void OnCollisionWith(LPCOLLISIONEVENT e) override;
    virtual void OnNoCollision(DWORD dt) override;

public:
    CParaGoomba(float x, float y, float spawnX);
    virtual int IsBlocking() { return 0; }
    virtual int IsCollidable() { return state != GOOMBA_STATE_DIE; }
	bool getHasWings() { return hasWings; }
	void lostWings() 
    {
        hasWings = false;
        hopCount = 0;
        last_action_time = GetTickCount64();
        SetState(GOOMBA_STATE_WALKING);
        vy = 0; 
        return;
    }
};