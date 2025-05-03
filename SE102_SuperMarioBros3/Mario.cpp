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

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// TE VUC
	if (y > MARIO_DEAD_Y)
	{
		SetState(MARIO_STATE_DIE);
	}
	// RIA MAN HINH
	if (x < LEFT_LIMIT)
		x = LEFT_LIMIT;
	if (x > RIGHT_LIMIT - 16)
		x = RIGHT_LIMIT - 16;
	// HUONG MAT
	if (vx > 0)
		facingDirection = 1;
	else if (vx < 0)
		facingDirection = -1;

	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;
	// CAM KOOPAS
	if (isHolding && heldKoopas != nullptr)
	{
		float shellX = x + (nx > 0 ? 10 : -10);
		float shellY = y - 5;
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
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (isTransforming)
	{
		if (GetTickCount64() - transform_start >= MARIO_TRANSFORM_TIME)
		{
			isTransforming = false;
			level = MARIO_LEVEL_BIG;
			y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
		}
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{ 
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CQuestionBrick*>(e->obj))
		OnCollisionWithQuestionBrick(e);
	else if (dynamic_cast<CPlantEnemy*>(e->obj))
		OnCollisionWithPlant(e);
	else if (dynamic_cast<CFireBall*>(e->obj))
		OnCollisionWithFireBall(e);
	else if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CKoopas*>(e->obj))
		OnCollisionWithKoopas(e);
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithLeaf(e);
}

bool CMario::IsHoldingKeyPressed()
{
	return CGame::GetInstance()->IsKeyDown(DIK_A);
}


void CMario::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);

	if (e->ny < 0) // Mario nhảy lên mai
	{
		if (koopas->GetState() == KOOPAS_STATE_WALKING)
		{
			koopas->SetState(KOOPAS_STATE_HIT);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (koopas->GetState() == KOOPAS_STATE_HIT)
		{
			if (koopas->GetState() != KOOPAS_STATE_DIE)
			{
				koopas->SetState(KOOPAS_STATE_HIT_MOVING);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
		}
		else if (koopas->GetState() == KOOPAS_STATE_REVIVE)
		{
			if (koopas->GetState() != KOOPAS_STATE_DIE)
			{
				koopas->SetState(KOOPAS_STATE_HIT);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
		}
		else if (koopas->GetState() == KOOPAS_STATE_HIT_MOVING)
		{
			if (koopas->GetState() != KOOPAS_STATE_DIE)
			{
				koopas->SetState(KOOPAS_STATE_HIT);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
		}
	}
	else 
	{
		if (untouchable == 0)
		{
			if (koopas->GetState() == KOOPAS_STATE_HIT)
			{
				if (abs(vx) >= MARIO_RUNNING_SPEED && IsHoldingKeyPressed())
				{
					isHolding = true;
					heldKoopas = koopas;
					koopas->SetBeingHeld(true);
				}
				else
				{
					int direction = (this->x < koopas->GetX()) ? 1 : -1;
					this->SetState(MARIO_STATE_KICK);
					koopas->SetDirection(direction);
					koopas->SetState(KOOPAS_STATE_HIT_MOVING);
				}
			}
			else
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}


void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	if (this->level == MARIO_STATE_DIE) return;
	if (level == MARIO_LEVEL_SMALL)
	{
		this->SetLevel(MARIO_LEVEL_BIG);
		isTransforming = true;
		transform_start = GetTickCount64();
		StartUntouchable();
	}
}
void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	CParaGoomba* paraGoomba = dynamic_cast<CParaGoomba*>(e->obj);

	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			if (paraGoomba && paraGoomba->getHasWings())
			{
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				paraGoomba->lostWings();
			}
			else
			{
				goomba->SetState(GOOMBA_STATE_DIE);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
		}
	}
	else
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}
void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
	if (leaf != nullptr)
	{
		if (this->level == MARIO_LEVEL_BIG)
		{
			//this->SetLevel(MARIO_LEVEL_RACCOON);
			isTransforming = true;
			transform_start = GetTickCount64();
			StartUntouchable();
		}
		e->obj->Delete();
	}
}

void CMario::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e)
{
	CQuestionBrick* qb = dynamic_cast<CQuestionBrick*>(e->obj);

	if (qb != nullptr && e->ny > 0 && qb->GetState() != 90000)
	{
		qb->SetState(90000);

		if (qb->getIsEmpty() == 0)
		{
			qb->SetIsEmpty(1);
			qb->StartBounce();

			if (qb->getType() == 0)
			{
				float coinX = qb->getX();
				float coinY = qb->getY() - QBRICK_BBOX_HEIGHT / 2 - COIN_BBOX_HEIGHT / 2;

				CCoin* coin = new CCoin(coinX, coinY);
				coin->StartBouncing();

				((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddObject(coin);
			}
			else if (qb->getType() == 1 && qb->getY() == qb->getStartY() && this->level == MARIO_LEVEL_SMALL)
			{
				float mushroomX = qb->getX();
				float mushroomY = qb->getY() - QBRICK_BBOX_HEIGHT / 2;

				CMushroom* mushroom = new CMushroom(mushroomX, mushroomY);
				((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddObject(mushroom);
				((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddObject(qb);
			}
			else if (qb->getType() == 1  && this->level == MARIO_LEVEL_BIG)
			{
				float leafX = qb->getX();
				float leafY = qb->getY() - QBRICK_BBOX_HEIGHT / 2 - LEAF_BBOX_HEIGHT / 2;
				CLeaf* leaf = new CLeaf(leafX, leafY);
				leaf->StartBouncing();
				((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddObject(leaf);
			}
		}
	}
}


void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}
void CMario::OnCollisionWithPlant(LPCOLLISIONEVENT e)
{
	CPlantEnemy* plant = dynamic_cast<CPlantEnemy*>(e->obj);
	if (plant->GetState()==0) return;
	if (untouchable == 0)
	{
		if (level > MARIO_LEVEL_SMALL)
		{
			level = MARIO_LEVEL_SMALL;
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}
	
}
void CMario::OnCollisionWithFireBall(LPCOLLISIONEVENT e)
{
	CFireBall* fireball = dynamic_cast<CFireBall*>(e->obj);
	if (fireball->GetState() == 0) return;
	if (untouchable == 0)
	{
		fireball->Delete();
		if (level > MARIO_LEVEL_SMALL)
		{
			level = MARIO_LEVEL_SMALL;
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}

}
//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	if (isTransforming)
	{
		ULONGLONG t = GetTickCount64() - transform_start;
		if (((t / 100) % 2 == 0))
		{
			if (facingDirection > 0)
				animations->Get(ID_ANI_MARIO_IDLE_RIGHT)->Render(x, y);
			else
				animations->Get(ID_ANI_MARIO_IDLE_LEFT)->Render(x, y);
		}
		else
		{
			if (facingDirection > 0)
				animations->Get(ID_ANI_MARIO_SMALL_IDLE_RIGHT)->Render(x, y);
			else
				animations->Get(ID_ANI_MARIO_SMALL_IDLE_LEFT)->Render(x, y);
		}
		return;
	}

	if (facingDirection > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
	else aniId = ID_ANI_MARIO_IDLE_LEFT;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (state == MARIO_STATE_KICK)
	{
		if (level == MARIO_LEVEL_BIG)
			aniId = (nx > 0) ? ID_ANI_MARIO_KICK_RIGHT : ID_ANI_MARIO_KICK_LEFT;
		else if (level == MARIO_LEVEL_SMALL)
			aniId = (nx > 0) ? ID_ANI_MARIO_SMALL_KICK_RIGHT : ID_ANI_MARIO_SMALL_KICK_LEFT;
	}
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();


	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
	
	DebugOutTitle(L"Coins: %d", coin);
}

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
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
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
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

