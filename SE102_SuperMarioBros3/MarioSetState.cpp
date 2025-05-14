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
#include "PlayScene.h"
#include "Koopas.h"
#include "Leaf.h"
#include "ParaGoomba.h"
void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

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
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		isGliding = false;
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
		vx = 0.0f;
		isAttacking = false;
		isFlying = false;
		isGliding = false;
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
		break;

		

	case MARIO_STATE_FLYING_RIGHT: 
		if (isSitting) break;
		
		ay = 0; 
		vy = initial_fly_vy; 
		nx = 1;
		isFlying = true;
		isGliding = false;
		canFlapTail = true; 
		if (fly_start == 0) { 
			fly_start = GetTickCount64(); 
		}
		DebugOut(L"[MARIO] SetState: FLYING_RIGHT. vy: %f\n", vy);
		break;

	case MARIO_STATE_FLYING_LEFT: 
		if (isSitting) break;

		ay = 0;
		vy = initial_fly_vy;
		nx = -1;
		isFlying = true;
		isGliding = false;
		canFlapTail = true;
		if (fly_start == 0) {
			fly_start = GetTickCount64();
		}
		DebugOut(L"[MARIO] SetState: FLYING_LEFT. vy: %f\n", vy);
		break;

	case MARIO_STATE_GLIDING_RIGHT: 
		if (isSitting || isOnPlatform) break;

		ay = MARIO_GLIDING_SPEED; 
		nx = 1;
		isGliding = true;
		isFlying = false;     
		canFlapTail = false;  
		DebugOut(L"[MARIO] SetState: GLIDING_RIGHT\n");
		break;

	case MARIO_STATE_GLIDING_LEFT:
		if (isSitting || isOnPlatform) break;

		ay = MARIO_GLIDING_SPEED;
		nx = -1;
		isGliding = true;
		isFlying = false;
		canFlapTail = false;
		DebugOut(L"[MARIO] SetState: GLIDING_LEFT\n");
		break;


	case MARIO_STATE_ATTACKING:
		if (isSitting || isFlying || isGliding || !isOnPlatform) break;
		if (level == MARIO_LEVEL_RACCOON && tail)
		{
			StartAttacking();
			DebugOut(L"[MARIO] ATTACKING cho 852\n");
		}
		break;

	}
	CGameObject::SetState(state);
}
