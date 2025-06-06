﻿#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"
#include "Koopas.h"
#include "debug.h"
#include "Tail.h"
#define MARIO_WALKING_SPEED		0.1f //0.1f
#define MARIO_RUNNING_SPEED		0.2f //0.2f

#define MARIO_MAX_POWER		1.0f 
#define MARIO_POWER_UP_RATE 0.00083f
#define MARIO_POWER_DOWN_RATE 0.0005f
#define MARIO_POWER_BRAKE_RATE 0.0014f
#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f
#define MARIO_JUMP_SPEED_BOOST 0.0004f


#define MARIO_DECELERATION_X 0.0006f

#define MARIO_JUMP_SPEED_Y		0.55f
#define MARIO_JUMP_RUN_SPEED_Y	0.65f

#define MARIO_FLYING_SPEED		0.15f
#define MARIO_GLIDING_SPEED		0.0005f

#define MARIO_TRAVELING_SPEED	0.01f

#define MARIO_FLY_DURATION		2000
#define FLY_TIMEOUT  3000
#define FLY_INPUT_WINDOW 500

#define MARIO_ATTACK_DURATION 350

#define MARIO_GRAVITY			0.002f //0.002f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_HOLD			650

#define MARIO_STATE_KICK			700

#define MARIO_STATE_FLYING_RIGHT			800
#define MARIO_STATE_FLYING_LEFT			900
#define MARIO_STATE_GLIDING_RIGHT			1000
#define MARIO_STATE_GLIDING_LEFT			1100

#define MARIO_STATE_ATTACKING 1200

#define MARIO_STATE_TRAVELING 1300

#define MARIO_STATE_COLLECTED_ITEM 1400

#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_KICK_LEFT 1103
#define ID_ANI_MARIO_KICK_RIGHT 1104

#define ID_ANI_MARIO_HOLD_LEFT 10001
#define ID_ANI_MARIO_HOLD_RIGHT 10002


#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

#define ID_ANI_MARIO_SMALL_KICK_LEFT 1105
#define ID_ANI_MARIO_SMALL_KICK_RIGHT 1106

// RACCOON MARIO
#define ID_ANI_MARIO_RACCOON_IDLE_RIGHT 2100
#define ID_ANI_MARIO_RACCOON_IDLE_LEFT 2102

#define ID_ANI_MARIO_RACCOON_SIT_RIGHT 2103
#define ID_ANI_MARIO_RACCOON_SIT_LEFT 2104

#define ID_ANI_MARIO_RACCOON_KICK_LEFT 2106
#define ID_ANI_MARIO_RACCOON_KICK_RIGHT 2105

#define ID_ANI_MARIO_RACCOON_WALKING_RIGHT 2200
#define ID_ANI_MARIO_RACCOON_WALKING_LEFT 2201

#define ID_ANI_MARIO_RACCOON_RUNNING_RIGHT 2300
#define ID_ANI_MARIO_RACCOON_RUNNING_LEFT 2301

#define ID_ANI_MARIO_RACCOON_BRACE_RIGHT 2400
#define ID_ANI_MARIO_RACCOON_BRACE_LEFT 2401

#define ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT 2500
#define ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT 2501

#define ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT 2600
#define ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT 2601

#define ID_ANI_MARIO_RACCOON_ATTACKING_RIGHT 2700
#define ID_ANI_MARIO_RACCOON_ATTACKING_LEFT 2701

#define ID_ANI_MARIO_RACCOON_FLY_RIGHT 2800
#define ID_ANI_MARIO_RACCOON_FLY_LEFT 2801

#define ID_ANI_MARIO_RACCOON_GLIDE_RIGHT 2900
#define ID_ANI_MARIO_RACCOON_GLIDE_LEFT 2901

#define ID_ANI_MARIO_TRAVEL 3000
#define ID_ANI_MARIO_BIG_TRAVEL 3001
#define ID_ANI_MARIO_SMALL_TRAVEL 3002
#pragma endregion

#define GROUND_Y 160.0f

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACCOON	3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12

#define MARIO_RACCOON_BBOX_WIDTH  14
#define MARIO_RACCOON_BBOX_HEIGHT 24
#define MARIO_RACCOON_SITTING_BBOX_WIDTH  14
#define MARIO_RACCOON_SITTING_BBOX_HEIGHT 16

#define MARIO_DEAD_Y 440

#define LEFT_LIMIT 0.0f
#define RIGHT_LIMIT 2816.0f

#define MARIO_UNTOUCHABLE_TIME 2500

#define MARIO_TRANSFORM_TIME 600 

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	bool isJumping = false;
	bool isKicking;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 
	int lastNx = 1;

	int level;
	int world;
	int score = 0;
	int lives;
	int untouchable;
	ULONGLONG untouchable_start;
	bool isOnPlatform;
	int coin;
	vector<LPGAMEOBJECT>* currentCoObjects = nullptr;
	//List item
	vector<int> collectedItem;

	//EFFECTS BIGGER
	bool isTransforming = false;
	int facingDirection;
	ULONGLONG transform_start = 0;

	//kicking
	ULONGLONG kick_start = 0;

	//TRAVEL
	bool isTraveling = false;
	ULONGLONG travel_start = 0;
	bool isTraveldown = false;
	bool isTravelup = false;
	int travel_phase = 0;
	bool haveTraveled = false;

	//HOLDING KOOPAS
	bool isHolding = false;
	CKoopas* heldKoopas = nullptr;

	//RACCOON 
	bool isFlying = false;
	bool isGliding = false;
	float power = 0.0f;
	ULONGLONG fly_start = 0;
	ULONGLONG lastFlyInput = 0;
	bool keySPrev = false;
	ULONGLONG jump_start = 0;        
	bool jump_with_max_power = false; 


	//on the lift
	bool isOnLift = false;
	ULONGLONG attack_start; // Thời điểm bắt đầu tấn công
	bool isAttacking; // Đang tấn công hay không
	CTail* tail;

	ULONGLONG die_start = 0;
	bool isTrueDied = false;

	//running
	bool isRunning = false;

	//blockedByObject
	bool isBlockingRight = false;
	DWORD blockingRightStart = 0;
	bool isTouchingRight = false;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithPlant(LPCOLLISIONEVENT e);
	void OnCollisionWithFireBall(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
	void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithParaTroopa(LPCOLLISIONEVENT e);
	void OnCollisionWithButton(LPCOLLISIONEVENT e);
	void OnCollisionWithPipe(LPCOLLISIONEVENT e);
	void OnCollisionWithItemCard(LPCOLLISIONEVENT e);
	void OnCollisionWithLift(LPCOLLISIONEVENT e);
	void OnCollisionWithBoomerangBro(LPCOLLISIONEVENT e);
	void OnCollisionWithGoldBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithGoldBoomerang(LPCOLLISIONEVENT e);
	void checkOnLift();
	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRaccoon();
public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		isKicking = false;
		isJumping = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY;

		facingDirection = 1;
		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;
		lives = 4;
		world = 1;
		score = 0;
		attack_start = 0;
		isAttacking = false;
		tail = nullptr;
		bool canTravel = false;
		bool isTraveling = false;
		int travel_phase = 0;
		ULONGLONG travel_start = 0;
		bool isTraveldown = false;
		bool isTravelup = false;
		bool isOnLift = false;
		bool haveTraveled = false;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	int IsDied() { return (state == MARIO_STATE_DIE); }
	int IsCollidable()
	{
		return (state != MARIO_STATE_DIE && state != MARIO_STATE_TRAVELING);
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable == 0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	float getY() { return y; }
	float getX() { return x; }
	void SetLevel(int l);
	int GetLevel() { return level; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }
	bool IsHoldingKeyPressed();
	int GetFacingDirection() { return facingDirection; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	float GetPower() { return this->power; }
	void UpdatePower(DWORD dt);
	bool isOnThePlatForm() { return (bool)isOnPlatform; }
	bool isGlidingNow() { return isGliding; }
	bool isFlyingNow() { return isFlying; }
	void StartAttacking() { isAttacking = true; attack_start = GetTickCount64(); if (tail) tail->StartAttacking(); }
	bool IsAttacking() { return isAttacking; }
	void CreateTail();
	void RemoveTail();
	CTail* GetTail() { return tail; }

	void SetScore(int score) { this->score += score; }
	void SetCoin(int coin) { this->coin += coin; }
	void SetWorld(int world) { this->world = world; }
	void SetLives(int value) { lives = value; }

	int GetScore() { return score; }
	int GetCoin() { return coin; }
	int GetLives() { return lives; }
	int GetWorld() { return world; }


	int GetPowerLevel()
	{
		int level = (int)(power / (MARIO_MAX_POWER / 6.0f));
		if (level > 6) level = 6;
		return level;
	}
	bool isTravelingNow() { return isTraveling; }
	bool isTravelUp() { return isTravelup; }
	bool isTravelDown() { return isTraveldown; }
	void setPosition(float x, float y) { this->x = x; this->y = y; }
	void AddCollectedItem(int type) {
		collectedItem.push_back(type);
		if (collectedItem.size() > 3)
			collectedItem.erase(collectedItem.begin());
	};
	vector<int> GetCollectedItems() { return collectedItem; }
	void DecreaseLives() { lives--; }
	bool isOnLiftNow() { return isOnLift; }
	bool isUntouchable() {
		if (untouchable == 1) return 1;
		return 0;
	}
	void SetRunning(bool r) { isRunning = r; }
	bool GetIsRunning() { return isRunning; }
	void SetIsOnLift(bool value) { isOnLift = value; }
	float GetVx() { return vx; }
	void SetVx(float vx) { this->vx = vx; }
	int GetIsUntouchable() { return untouchable; }
	void SetX(float x) { this->x += x; }
	bool GetIsBlockingRight() { return isBlockingRight; }
	void SetIsBlockingRight(bool b) { isBlockingRight = b; }
	bool IsTouchingBlockingObjectRight(const vector<LPGAMEOBJECT>& coObjects);
	bool isJumpMaxSpeed() { return jump_with_max_power; }
	void ClearCollectedItems() { collectedItem.clear(); }
	void SetFaceDirection(int direction) { facingDirection = direction; }
};