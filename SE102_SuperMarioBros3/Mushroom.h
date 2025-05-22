#pragma once
#include "GameObject.h"

#define MUSHROOM_BBOX_WIDTH  16
#define MUSHROOM_BBOX_HEIGHT 16

#define MUSHROOM_STATE_EMERGE   50000
#define MUSHROOM_STATE_WALK     50001

#define MUSHROOM_EMERGE_SPEED_Y 0.005f
#define MUSHROOM_WALK_SPEED_X   0.04f
#define MUSHROOM_GRAVITY        0.002f
#define ID_ANI_MUSHROOM         50000

#define ID_ANI_POINT_1000       140004

class CMushroom : public CGameObject {
protected:
    float startY; 
    bool isEaten = false;
    int pointValue = 1000;        
    bool isPointVisible = false;
    float pointX;
    float pointY;
    ULONGLONG pointStartTime;
    int IsCollidable() override { return 1; }
    int IsBlocking() override { return 0; }
public:
    CMushroom(float x, float y) : CGameObject(x, y) {
        this->x = x;
        this->y = y;
        startY = y;
        vx = 0;
        vy = -MUSHROOM_EMERGE_SPEED_Y;
        state = MUSHROOM_STATE_EMERGE;

		pointY = y;
		pointStartTime = 0;
		isPointVisible = false;
    }
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();

    void GetBoundingBox(float& l, float& t, float& r, float& b);
    void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnNoCollision(DWORD dt);
    void OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e);
    void OnDefeated();
};
