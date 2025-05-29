#pragma once
#include "GameObject.h"

#define BOOMERANGBROTHER_GRAVITY 0.002f
#define BOOMERANGBROTHER_WALKING_SPEED 0.05f
#define BOOMERANGBROTHER_JUMP_SPEED_Y 0.3f
#define BOOMERANGBROTHER_JUMP_INTERVAL 2500 
#define BOOMERANGBROTHER_THROW_INTERVAL 1500 

#define BOOMERANGBROTHER_BBOX_WIDTH 16
#define BOOMERANGBROTHER_BBOX_HEIGHT 22
#define BOOMERANGBROTHER_BBOX_HEIGHT_DIE 7
#define BOOMERANGBROTHER_BBOX_HEIGHT_THROW 28

#define BOOMERANGBROTHER_DIE_TIMEOUT 500

#define BOOMERANGBROTHER_STATE_WALKING 100
#define BOOMERANGBROTHER_STATE_DIE 200
#define BOOMERANGBROTHER_STATE_SUPER_DIE 250
#define BOOMERANGBROTHER_STATE_BOUNCE 300
#define BOOMERANGBROTHER_STATE_THROW 350
#define BOOMERANGBROTHER_STATE_JUMPING 400

#define ID_ANI_BOOMERANGBROTHER_WALKINGLEFT 301100
#define ID_ANI_BOOMERANGBROTHER_DIE 301101
#define ID_ANI_BOOMERANGBROTHER_SUPER_DIE 301102
#define ID_ANI_BOOMERANGBROTHER_THROWLEFT 301103
#define ID_ANI_BOOMERANGBROTHER_WALKINGRIGHT 301104
#define ID_ANI_BOOMERANGBROTHER_THROWRIGHT 301105
#define ID_ANI_BOOMERANGBROTHER_JUMPINGLEFT 301106
#define ID_ANI_BOOMERANGBROTHER_JUMPINGRIGHT 301107

#define ID_ANI_POINT_100 140000
#define ID_ANI_POINT_1000 140004

#define BOOMERANGBROTHER_SUPER_DIE_VX 1.5f
#define BOOMERANGBROTHER_SUPER_DIE_VY 0.5f

class CBoomerangBrother : public CGameObject
{
protected:
    float ax;
    float ay;
    bool isActive = false;
    float spawnX;
    float startX;
    float startY;
    const float BOUNCE_VELOCITY = -0.5f;
    bool isBouncing = false;
    float maxDistanceX = 20.0f;
    int pointValue = 1000;
    bool isPointVisible = false;
    float pointY;
    ULONGLONG pointStartTime;
    ULONGLONG die_start;
    ULONGLONG super_die_start;
    ULONGLONG jump_start;
    ULONGLONG throw_start;
    int boomerangCount; 
    bool isThrowing;
	bool hasThrownBoomerang = false;

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();
    virtual int IsCollidable() { return state != BOOMERANGBROTHER_STATE_DIE && state != BOOMERANGBROTHER_STATE_SUPER_DIE; }
    virtual int IsBlocking() { return 0; }
    virtual void OnNoCollision(DWORD dt);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);


public:
    CBoomerangBrother(float x, float y, float spawnX);
    virtual void SetState(int state);
    void StartBouncing();
    void OnDefeated(int score = 1000);
    void ThrowBoomerang();
    float GetX() { return x; }
    float GetY() { return y; }
    int checkMarioDirection();
    void reduceboomerangCount() { boomerangCount -= 1; }
};