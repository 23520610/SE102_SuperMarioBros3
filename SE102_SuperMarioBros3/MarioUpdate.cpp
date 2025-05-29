#include <algorithm>
#include "debug.h"
#include "Mario.h"
#include "Game.h"
#include "Mushroom.h"
#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "QuestionBrick.h"
#include "AssetIDs.h"
#include "PlantEnemy.h"
#include "FireBall.h"
#include "Collision.h"
#include "Button.h"
#include "PlayScene.h"
#include "Koopas.h"
#include "Leaf.h"
#include "ParaGoomba.h"
#include "ParaTroopa.h"
#include "Pipe.h"
#include "ItemCard.h"
#include "Lift.h"
#include "BoomerangBrother.h"
#include "RedParaTroopa.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isOnThePlatForm() && !isOnLiftNow()){
		ay = MARIO_GRAVITY;

	}
	else if (isGliding){
		ay = MARIO_GLIDING_SPEED;
	}
	if (vy > MARIO_JUMP_SPEED_Y && !isOnLiftNow())
	{
		vy = MARIO_JUMP_SPEED_Y;// giới tốc độ rơi
	}
	if (level == MARIO_LEVEL_RACCOON)
	{
		if (!tail) {
			CreateTail();
			DebugOut(L"Tail was created successfully!\n");
		}
	}
	//Danh rieng cho map 4
	if (world == 4 && haveTraveled) {
		if (x < 2060)
			x = 2060;
	}
	// TE VUC
	bool inSafeZone = (x > 1967 && x < 2478 && y > 540 && y < 724);

	if (y > MARIO_DEAD_Y && !inSafeZone && state != MARIO_STATE_TRAVELING)
	{
		SetState(MARIO_STATE_DIE);
		//return;
	}

	// RIA MAN HINH
	float leftLimit = 0.0f;
	float rightLimit = 2816.0f;
	if (this->GetWorld() == 4) rightLimit = 2575.0f;
	if (x < leftLimit)
		x = leftLimit;

	if (x > rightLimit - 16 && !CGame::GetInstance()->GetIsHasCard())
		x = rightLimit - 16;
	// HUONG MAT
	if (vx > 0)
	{
		facingDirection = 1;
	}
	else if (vx < 0)
	{
		facingDirection = -1;
	}
	//KICK KOOPAS
	if (isKicking && GetTickCount64() - kick_start > 200)
	{
		isKicking = false;
	}

	vy += ay * dt;
	vx += ax * dt;
	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// CAM KOOPAS
	if (isHolding && heldKoopas != nullptr)
	{
		//DebugOut(L"[KOOPAS] SetState HIT_MOVING, nx = %d\n", nx);
		float shellX = x + (nx > 0 ? 8 : -8);
		float shellY = y - 3;
		if (heldKoopas != nullptr)
			heldKoopas->SetPosition(shellX, shellY);
		if (!IsHoldingKeyPressed())
		{
			isHolding = false;
			heldKoopas->SetBeingHeld(false);
			heldKoopas->SetDirection(nx);
			heldKoopas->SetState(KOOPAS_STATE_HIT_MOVING);
			heldKoopas = nullptr;
		}
	}
	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (isTransforming)
	{
		CPlayScene* playScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		playScene->SetGamePaused(true);
		if (GetTickCount64() - transform_start >= MARIO_TRANSFORM_TIME)
		{
			isTransforming = false;
			playScene->SetGamePaused(false);
			if (level == MARIO_LEVEL_BIG)
			{
				level = MARIO_LEVEL_BIG;
				y += (MARIO_RACCOON_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT);

			}
			else if (level == MARIO_LEVEL_SMALL)
			{

				level = MARIO_LEVEL_SMALL;
				y += (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
			}
			else if (level == MARIO_LEVEL_RACCOON)
			{

				level = MARIO_LEVEL_RACCOON;
				y -= (MARIO_RACCOON_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT);
			}
			return;
		}
		return;
	}
	UpdatePower(dt);
	if (level == MARIO_LEVEL_RACCOON) {
		DWORD now = GetTickCount64();
		bool keySNow = CGame::GetInstance()->IsKeyDown(DIK_S);

		if (isFlying) {
			if (now - fly_start <= FLY_TIMEOUT) {
				if (keySNow && !keySPrev) {
					lastFlyInput = now;
				}
				if (now - lastFlyInput <= FLY_INPUT_WINDOW) {
					vy = -MARIO_FLYING_SPEED;
					ay = 0;
				}
				else {
					isGliding = true; 
					isFlying = false;
					ay = MARIO_GLIDING_SPEED;
				}
			}
			else {
				isGliding = true;
				isFlying = false;
				ay = MARIO_GLIDING_SPEED;
			}
		}
		else if (jump_with_max_power && keySNow) {
			isFlying = true;
			fly_start = now;
			lastFlyInput = now;
			vy = -MARIO_FLYING_SPEED;
			ay = 0;

			jump_with_max_power = false;
		}
		keySPrev = keySNow;
	}


	if (tail)
	{
		//DebugOut(L"[TAIL] Update position,\n");
		tail->UpdatePosition(x, y, nx);
		if (isAttacking && GetTickCount64() - attack_start >= MARIO_ATTACK_DURATION)
		{
			isAttacking = false;
			attack_start = 0;
			//DebugOut(L"[TAIL] Stop attacking 122\n");
		}
	}
	if (isTraveling)
	{
		y += vy * dt;
		ay = 0;
		if (isTravelup)
			vy = -MARIO_TRAVELING_SPEED;
		else if (isTraveldown)
			vy = MARIO_TRAVELING_SPEED;

		if (world == 1)
		{
			if (isTraveldown && GetTickCount64() - travel_start > 1000 && travel_phase == 0)
			{
				travel_phase = 1;
				ay = 0;
				isOnPlatform = false;
				DebugOut(L"[INFO] Travel o phia tren (Map 1)\n");
				SetPosition(2104, 555);
			}
			else if (isTravelup && GetTickCount64() - travel_start > 500 && travel_phase == 0)
			{
				travel_phase = 1;
				ay = 0;
				isOnPlatform = false;
				DebugOut(L"[INFO] Travel o phia duoi (Map 1)\n");
				SetPosition(2329, 384);
			}
		}
		else if (world == 4)
		{
			if (isTraveldown && GetTickCount64() - travel_start > 500 && travel_phase == 0)
			{
				travel_phase = 1;
				ay = 0;
				isOnPlatform = false;
				isTraveldown = false;
				isTravelup = true;
				DebugOut(L"[INFO] Travel len (Map 4)\n");
				SetPosition(2201, 386);
			}
		}

		if (travel_phase == 1 && GetTickCount64() - travel_start > 1500)
		{
			DebugOut(L"[INFO] Travel hoàn thành\n");
			vy = 0;
			ay = MARIO_GRAVITY;
			isTraveling = false;
			isTraveldown = false;
			isTravelup = false;
			isOnPlatform = true;
			haveTraveled = true;
			travel_phase = 0;
		}
	}
	//DebugOut(L"[Mario] state= %d",state);

	if (GetState() == MARIO_STATE_DIE && lives >= 0 && isTrueDied == false)
	{
		if (GetTickCount64() - die_start >= 2000)
		{
			OutputDebugString(L"Would reload scene here\n");
			CGame::GetInstance()->ReloadCurrentScene();

			return;
		}
	}
	if (state == MARIO_STATE_DIE && lives < 0 &&isTrueDied==true)
	{
		bool pressReStart = CGame::GetInstance()->IsKeyDown(DIK_W);
		if (pressReStart)
		{
			DebugOut(L"[INFO] Restarting game...\n");
			CGame::GetInstance()->RestartGame();
			isTrueDied = false;
			return;
		}
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
	checkOnLift();
}