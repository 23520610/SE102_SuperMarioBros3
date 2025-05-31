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

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed!

	if (this->state == MARIO_STATE_DIE)
	{
		return;
	}
	if (isTraveling && state != MARIO_STATE_TRAVELING)
	{
		return;
	}
	if (this->state == MARIO_STATE_COLLECTED_ITEM)
	{
		return;
	}
	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isFlying) break;
		if (isOnPlatform || isOnLift)
		{
			ay = MARIO_GRAVITY;
			ax += (facingDirection > 0 ? MARIO_JUMP_SPEED_BOOST : -MARIO_JUMP_SPEED_BOOST);
			if (abs(this->vx) == MARIO_RUNNING_SPEED) {
				vy = -MARIO_JUMP_RUN_SPEED_Y;
				if (level == MARIO_LEVEL_RACCOON && power >= MARIO_MAX_POWER)
				{
					jump_with_max_power = true;
				}
			}
			else
				vy = -MARIO_JUMP_SPEED_Y;
			
		}
		isJumping = true;
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		//vx = 0.0f;
		isAttacking = false;
		break;
	case MARIO_STATE_KICK:
		vx = 0;
		ax = 0;
		kick_start = GetTickCount64();
		//isKicking = false;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;

		--lives;
		CGame::GetInstance()->SetPlayerLives(this->lives);
		CGame::GetInstance()->SetPlayerScore(this->score);
		CGame::GetInstance()->SetPlayerCoin(this->coin);
		die_start = GetTickCount64();
		if (lives < 0) isTrueDied = true;

		break;

	case MARIO_STATE_FLYING_RIGHT:
	case MARIO_STATE_FLYING_LEFT:
		if (!isOnPlatform || power < MARIO_MAX_POWER) break;
		isFlying = true;
		isGliding = false;
		fly_start = GetTickCount64();
		lastFlyInput = fly_start;
		vy = -MARIO_FLYING_SPEED;
		ay = 0;
		nx = (state == MARIO_STATE_FLYING_RIGHT) ? 1 : -1;
		break;
	case MARIO_STATE_GLIDING_RIGHT:		
	case MARIO_STATE_GLIDING_LEFT:
		if (isOnPlatform) break;
		isGliding = true;
		isFlying = false;
		vy = 0; // MARIO_GLIDING_SPEED;
		DebugOut(L"[MARIO] GLIDING, vy =%d\n",vy);
		//ay = MARIO_GLIDING_SPEED;
		nx = (state == MARIO_STATE_GLIDING_RIGHT) ? 1 : -1;
		break;

	case MARIO_STATE_ATTACKING:
		if (isSitting || isFlying || isGliding) break;
		if (level == MARIO_LEVEL_RACCOON && tail)
		{
			StartAttacking();
			//DebugOut(L"[MARIO] ATTACKING cho 852\n");
		}
		break;
	case MARIO_STATE_TRAVELING:
		DebugOut(L"[INFO] MARIO TRAVELING \n");
		//vy = MARIO_TRAVELING_SPEED; 
		ay = 0;
		vx = 0;
		break;
	case MARIO_STATE_COLLECTED_ITEM:
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	}

	CGameObject::SetState(state);
}