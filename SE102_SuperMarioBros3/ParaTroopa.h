#pragma once
#include "Goomba.h"
#include "PlayScene.h"

#define PARA_TROOPAWING_BBOX_WIDTH 16
#define PARA_TROOPAWING_BBOX_HEIGHT 26

#define PARATROOPA_JUMP_SPEED 0.48f
#define PARATROOPA_JUMP_INTERVAL  1200

#define PARA_TROOPA_GRAVITY 0.002f 
#define ID_ANI_PARA_TROOPA_WALKLEFT 180000
#define WING_PARATROOPA 180999

class CParaTroopa : public CGameObject
{
protected:
    float ax;
    float ay;

    bool isActive = false;
    bool isOnGround = false;

    float spawnX;
    bool hasWings;
    ULONGLONG last_action_time = 0;

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    virtual void Render() override;

    virtual void OnCollisionWith(LPCOLLISIONEVENT e) override;
    virtual void OnNoCollision(DWORD dt) override;

public:
    CParaTroopa(float x, float y, float spawnX);
    virtual void SetState(int state) override;
    virtual int IsBlocking() { return 0; }
    virtual int IsCollidable() { return 1; }
    bool getHasWings() { return hasWings; }
    float GetX() { return x; }
	float GetY() { return y; }
    virtual void lostWings()
    {
        hasWings = false;
        CKoopas* koopas = new CKoopas(x, y, 0, 1);
        LPGAMEOBJECT self = this;
        CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
        scene->AddObject(koopas);
        this->Delete();
        vy = 0;
        return;
    }
	virtual void beHited()
	{
		hasWings = false;
		CKoopas* koopas = new CKoopas(x, y, 0, 1);
		LPGAMEOBJECT self = this;
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		scene->AddObject(koopas);
		this->Delete();
		vy = 0;
		koopas->SetState(KOOPAS_STATE_DIE);
		return;
	}
};