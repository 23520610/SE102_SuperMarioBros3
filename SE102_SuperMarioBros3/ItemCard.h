#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Animations.h"
#include "Mario.h"
#include "PlayScene.h"


#define ITEMCARD_STATE_MUSHROOM 0
#define ITEMCARD_STATE_STAR 1
#define ITEMCARD_STATE_FLOWER 2
#define ITEMCARD_STATE_BE_COLLECTED 3
#define ITEMCARD_TIME_CHANGE 150
#define ITEMCARD_BBOX_WIDTH 16
#define ITEMCARD_BBOX_HEIGHT 16
#define ITEMCARD_BOUNCE_SPEED 0.2f

#define ID_ANI_ITEMCARD_MUSHROOM 210003
#define ID_ANI_ITEMCARD_STAR 210005
#define ID_ANI_ITEMCARD_FLOWER 210004

#define ID_SPRITE_MUSHROOM 410000
#define ID_SPRITE_STAR 410002
#define ID_SPRITE_FLOWER 410001
class CItemCard : public CGameObject
{
protected :
	ULONGLONG start_change;
	int type;
	bool isMushroom = 0;
	bool isStar = 0;
	bool isFlower = 0;
	bool isCollected = false;

public:
	CItemCard(float x, float y) :CGameObject(x, y)
	{
		type = -1;
		start_change = 0;
		bool isMushroom = 0;
		bool isStar = 0;
		bool isFlower = 0;
		vy = 0;
		state = ITEMCARD_STATE_MUSHROOM;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 1; }
	int IsCollidable() { return state != ITEMCARD_STATE_BE_COLLECTED; }
	int GetType()
	{
		return type;
	}
	void OnNoCollision(DWORD dt);
	virtual void SetState(int state) override;
	bool IsCollected() { return isCollected; }
	void SetIsCollected(bool c) { isCollected = c; }
};

