#pragma once
#include "GameObject.h"
#include "Animations.h"
#include "Mushroom.h"
#define ID_ANI_QUESTION_BRICK 30000
#define QBRICK_BBOX_WIDTH 16
#define QBRICK_BBOX_HEIGHT 16

#define ID_ANI_POINT_100 140000

class CQuestionBrick : public CGameObject
{
private :
	int itemType; //0:coin, 1: mushroom/leaf
	bool isEmpty = 0; //da lay vat pham chua
    float startY;         // vi tri ban dau
    float vy = 0;         
    bool isBouncing; //dang nay
    CMushroom* mr = NULL;

    int pointValue = 100;        
    bool isPointVisible = false;
    float pointY;
    ULONGLONG pointStartTime;

    const float BOUNCE_VELOCITY = -0.1f;
    const float GRAVITY = 0.0005f;
    const float MAX_BOUNCE_HEIGHT = 8.0f;


public:
    CQuestionBrick(float x, float y, int itemType) : CGameObject(x, y)
    {
        this->x = x;
        this->y = y;
		this->itemType = itemType;
		this->startY = y;
		this->isBouncing = false;
    }
    void Render();
	float getStartY() { return startY; }
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void GetBoundingBox(float& l, float& t, float& r, float& b);
	int getType() { return itemType; }

	bool getIsEmpty() { return isEmpty; }
	void SetIsEmpty(int isEmpty) { this->isEmpty = isEmpty; }

    void StartBounce();
	void StopBounce();

	bool getIsBouncing() { return isBouncing; }
	void setIsBouncing(bool isBouncing) { this->isBouncing = isBouncing; }

	float getX() { return x; }
	float getY() { return y; }

    int IsCollidable() override { return 1; }
    int IsBlocking() override { return 1; }

    void SpawnPoint();
	
};