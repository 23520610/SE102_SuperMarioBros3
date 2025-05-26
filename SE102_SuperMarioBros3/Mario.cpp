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
		if (e->ny < 0) {
			isOnPlatform = true;
			isFlying = false;
			isGliding = false;
			isTraveling = false;
			isOnLift = false;
			ay = MARIO_GRAVITY; 
			if (state == MARIO_STATE_FLYING_RIGHT || state == MARIO_STATE_FLYING_LEFT)
			{
				SetState(MARIO_STATE_IDLE); 
			}
		}
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
	else if (dynamic_cast<CPipe*>(e->obj))
		OnCollisionWithPipe(e);
	else if (dynamic_cast<CItemCard*>(e->obj))
		OnCollisionWithItemCard(e);
	else if (dynamic_cast<CParaTroopa*>(e->obj))
		OnCollisionWithParaTroopa(e);
	else if (dynamic_cast<CButton*>(e->obj))
		OnCollisionWithButton(e);
	else if (dynamic_cast<CLift*>(e->obj))
		OnCollisionWithLift(e);
	else if (dynamic_cast<CBoomerangBrother*>(e->obj))
		OnCollisionWithBoomerangBro(e);
	else if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithGoldBrick(e);
}

bool CMario::IsHoldingKeyPressed()
{
	return CGame::GetInstance()->IsKeyDown(DIK_A);
}

void CMario::OnCollisionWithGoldBrick(LPCOLLISIONEVENT e)
{
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);
	//DebugOut(L"[DEBUG] Brick type = %d, has button = %d\n", brick->GetType(), brick->GetButton());
	if (brick != nullptr && e->ny > 0 && (brick->GetType()==0 || brick->GetType() == 3 || brick->GetType() == 2 || brick->GetType() == 5)) 
	{
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		CEffect* effect = new CEffect(brick->GetX(), brick->GetY(), ID_ANI_RACCOON_HIT_EFFECT, 0, 0, 50);
		scene->AddEffect(effect);
		brick->Delete();
		float vx_initial[] = { -1.5f, -1.5f, 1.5f, 1.5f };
		float vy_initial[] = { -0.5f, -0.4f, -0.4f, -0.5f };
		for (int i = 0; i < 4; i++)
		{
			CEffect* effect = new CEffect(brick->GetX(), brick->GetY(), ID_ANI_BREAK_EFFECT, vx_initial[i], vy_initial[i], 1000);
			scene->AddEffect(effect);
		}
		if (brick->GetButton())
			CButton::SpawnButton(brick->GetX(), brick->GetY());

		if (brick->GetType() == 3)
		{
			CQuestionBrick* qbrick = new CQuestionBrick(brick->GetX(), brick->GetY(), 2); 
			scene->AddObject(qbrick);
			qbrick->SetState(90000);
			CMushroom* mushroom = new CMushroom(brick->GetX(), brick->GetY(), 2);
			scene->AddObjectBefore(mushroom, qbrick);
		}

		if (brick->GetType() == 5)
		{
			CQuestionBrick* qbrick = new CQuestionBrick(brick->GetX(), brick->GetY(), 1);
			scene->AddObject(qbrick);
			qbrick->SetState(90000);
			CMushroom* mushroom = new CMushroom(brick->GetX(), brick->GetY(), 1);
			scene->AddObjectBefore(mushroom, qbrick);
		}
	}
	else if (brick != nullptr && e->ny > 0 && (brick->GetType() == 1 || brick->GetType() == 4))
	{
		brick->StartBounce();

		if (brick->GetType() == 4)
		{
			CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
			float coinX = brick->GetX();
			float coinY = brick->GetY() - QBRICK_BBOX_HEIGHT / 2 - COIN_BBOX_HEIGHT / 2;

			CCoin* coin = new CCoin(coinX, coinY);
			coin->StartBouncing();
			((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddObject(coin);

			if (brick->GetBounceCount() >= 10)
			{
				brick->Delete();
				CQuestionBrick* qbrick = new CQuestionBrick(brick->GetX(), brick->GetY(), -1);
				scene->AddObject(qbrick);

				qbrick->SetState(90000);
			}
		}
	}
}

void CMario::OnCollisionWithBoomerangBro(LPCOLLISIONEVENT e) {
	CBoomerangBrother* boomerangbro = dynamic_cast<CBoomerangBrother*>(e->obj);
	if (boomerangbro != nullptr && e->ny < 0) {
		boomerangbro->SetState(BOOMERANGBROTHER_STATE_DIE);
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		score += 1000;
		boomerangbro->OnDefeated();
	}
	else
	{
		if (untouchable == 0)
		{
			if (boomerangbro->GetState() != GOOMBA_STATE_DIE)
			{
				if (level == MARIO_LEVEL_BIG)
				{
					this->SetLevel(MARIO_LEVEL_SMALL);
					isTransforming = true;
					transform_start = GetTickCount64();
					//level -= 1;
					StartUntouchable();
				}
				else if (level == MARIO_LEVEL_RACCOON)
				{
					this->SetLevel(MARIO_LEVEL_BIG);
					isTransforming = true;
					transform_start = GetTickCount64();
					//level -= 1;
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

void CMario::OnCollisionWithLift(LPCOLLISIONEVENT e)
{
	CLift* lift = dynamic_cast<CLift*>(e->obj);
	if (lift != nullptr&& e->ny < 0) {
		lift->TriggerFall();
		ay = LIFT_GRAVITY;
		isOnLift = true;
		isOnPlatform = true;
		if (state != MARIO_STATE_JUMP) {
			vy = 0; 
		}
	}

}
void CMario::OnCollisionWithPipe(LPCOLLISIONEVENT e)
{
	CPipe* pipe = dynamic_cast<CPipe*>(e->obj);
	if (pipe != nullptr && pipe->GetType() == PIPE_TYPE_DOWN)
	{
		DebugOut(L"[INFO] Mario is on the pipe can down \n");
		if (CGame::GetInstance()->IsKeyDown(DIK_DOWN)&&!isTravelingNow() && e->ny < 0)
		{
			DebugOut(L"[INFO] Mario is going down the pipe heheheeheheh %d\n",state);
			isSitting = false;
			this->isTraveling = true;
			this->isTraveldown = true;
			this->isOnPlatform = true;
			this->travel_start = GetTickCount64();
			this->SetState(MARIO_STATE_TRAVELING);
			vy = MARIO_TRAVELING_SPEED;
			DebugOut(L"[INFO] Is DOWNING %d\n", state);
			//CGame::GetInstance()->InitiateSwitchScene(1); 
			return;
		}
	}
	else if (pipe != nullptr && pipe->GetType() == PIPE_TYPE_BLACK_BONUS &&e->ny>0 )
	{
		DebugOut(L"[INFO] COLLISION WITH SHIT \n");
		if (CGame::GetInstance()->IsKeyDown(DIK_UP) && !isTravelingNow())
		{
			DebugOut(L"[INFO] Mario is going up the pipe %d\n", state);
			isSitting = false;
			ay = 0;
			vy = 0;
			vx = 0;
			y -= 10;
			this->isTraveling = true;
			this->isTravelup = true;
			//this->isOnPlatform = true;
			this->travel_start = GetTickCount64();
			this->SetState(MARIO_STATE_TRAVELING);
			DebugOut(L"[INFO] Is DOWNING %d\n", state);
			//CGame::GetInstance()->InitiateSwitchScene(1); 
			return;
		}
	}
}
void CMario::OnCollisionWithButton(LPCOLLISIONEVENT e)
{
	CButton* button = dynamic_cast<CButton*>(e->obj);
	if (e->ny < 0)
	{
		button->SetBroken(true);
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		vector<LPGAMEOBJECT> objects = scene->GetObjects();

		for (LPGAMEOBJECT obj : objects)
		{
			CBrick* goldBrick = dynamic_cast<CBrick*>(obj);
			if (goldBrick)
			{
				goldBrick->Delete(); 
				float vx_initial[] = { -1.5f, -1.5f, 1.5f, 1.5f };
				float vy_initial[] = { -0.5f, -0.4f, -0.4f, -0.5f };

				for (int i = 0; i < 4; i++)
				{
					CEffect* effect = new CEffect(goldBrick->GetX(), goldBrick->GetY(), ID_ANI_BREAK_EFFECT, vx_initial[i], vy_initial[i], 1000);
					scene->AddEffect(effect);
				}
			}
		}
	}
}
void CMario::OnCollisionWithParaTroopa(LPCOLLISIONEVENT e)
{
	CParaTroopa* paraTroopa = dynamic_cast<CParaTroopa*>(e->obj);
	CRedParaTroopa* redParaTroopa = dynamic_cast<CRedParaTroopa*>(e->obj);

	if (e->ny < 0) // Mario nhảy lên đầu Troopa
	{
		vy = -MARIO_JUMP_DEFLECT_SPEED;

		if (redParaTroopa && redParaTroopa->getHasWings()) 
		{
			redParaTroopa->loseWings(); 
			score += 100;
		}
		else if (paraTroopa && paraTroopa->getHasWings()) 
		{
			paraTroopa->lostWings(); 
			score += 100;
		}
	}
	else 
	{
		if (untouchable == 0)
		{
			if (level == MARIO_LEVEL_BIG)
			{
				SetLevel(MARIO_LEVEL_SMALL);
				isTransforming = true;
				transform_start = GetTickCount64();
				StartUntouchable();
			}
			else if (level == MARIO_LEVEL_RACCOON)
			{
				SetLevel(MARIO_LEVEL_BIG);
				isTransforming = true;
				transform_start = GetTickCount64();
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

void CMario::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);

	if (e->ny < 0) // Mario nhảy lên mai
	{
		if (koopas->GetState() == KOOPAS_STATE_WALKING)
		{
			koopas->OnDefeated();
			koopas->SetState(KOOPAS_STATE_HIT);
			//koopas->SetNx((koopas->GetVx() > 0) ? 1 : -1);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
			score += 100;
		}
		else if (koopas->GetState() == KOOPAS_STATE_HIT || koopas->GetState() == KOOPAS_STATE_DIE)
		{
				koopas->OnDefeated();
				koopas->SetState(KOOPAS_STATE_HIT_MOVING);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				score += 200;
		}
		else if (koopas->GetState() == KOOPAS_STATE_REVIVE)
		{
			if (koopas->GetState() != KOOPAS_STATE_DIE)
			{
				koopas->SetState(KOOPAS_STATE_HIT);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				score += 100;
			}
		}
		else if (koopas->GetState() == KOOPAS_STATE_HIT_MOVING)
		{
			if (koopas->GetState() != KOOPAS_STATE_DIE)
			{
				koopas->OnDefeated();
				score += 100;
				koopas->SetState(KOOPAS_STATE_HIT);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
		}
	}
	else
	{
		if (untouchable == 0)
		{
			if (koopas->GetState() == KOOPAS_STATE_HIT || koopas->GetState() == KOOPAS_STATE_DIE)
			{
				if (abs(vx) >= MARIO_RUNNING_SPEED && IsHoldingKeyPressed())
				{
					isHolding = true;
					this->SetState(MARIO_STATE_HOLD);
					heldKoopas = koopas;
					koopas->SetBeingHeld(true);
				}
				else
				{
					int direction = (this->x < koopas->GetX()) ? 1 : -1;
					this->isKicking = true;
					this->SetState(MARIO_STATE_KICK);
					this->nx = direction;
					koopas->SetDirection(direction);
					koopas->SetState(KOOPAS_STATE_HIT_MOVING);
				}
			}
			else
			{
				if (level == MARIO_LEVEL_BIG)
				{
					this->SetLevel(MARIO_LEVEL_SMALL);
					isTransforming = true;
					transform_start = GetTickCount64();
					//level -=1;
					StartUntouchable();
				}
				else if (level == MARIO_LEVEL_RACCOON)
				{
					this->SetLevel(MARIO_LEVEL_BIG);
					isTransforming = true;
					transform_start = GetTickCount64();
					//level -= 1;
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
	CMushroom* mushroom = dynamic_cast<CMushroom*>(e->obj);
	if (!mushroom || mushroom->GetIsEaten()) return;
	mushroom->OnDefeated();

	if (this->level == MARIO_STATE_DIE) return;

	if (mushroom->GetType() == 1)
	{
		if (level == MARIO_LEVEL_SMALL)
		{
			score += 1000;
			this->SetLevel(MARIO_LEVEL_BIG);
			isTransforming = true;
			transform_start = GetTickCount64();
			StartUntouchable();
		}
		else if (level == MARIO_LEVEL_BIG)
		{
			score += 1000;
			this->SetLevel(MARIO_LEVEL_RACCOON);
			isTransforming = true;
			transform_start = GetTickCount64();
			StartUntouchable();
		}
		else score += 1000;
	}
	else if (mushroom->GetType() == 2)
	{
		this->SetLives(this->GetLives() + 1);
		score += 1000;
	}
}


void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	CParaGoomba* paraGoomba = dynamic_cast<CParaGoomba*>(e->obj);

	if (e->ny < 0) // Mario nhảy lên đầu Goomba
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			if (paraGoomba && paraGoomba->getHasWings())
			{
				score += 100;
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				paraGoomba->lostWings();
			}
			else
			{
				goomba->SetState(GOOMBA_STATE_DIE);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				score += 100;
				goomba->OnDefeated();
			}
		}
	}
	else
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level == MARIO_LEVEL_BIG)
				{
					this->SetLevel(MARIO_LEVEL_SMALL);
					isTransforming = true;
					transform_start = GetTickCount64();
					//level -= 1;
					StartUntouchable();
				}
				else if (level == MARIO_LEVEL_RACCOON)
				{
					this->SetLevel(MARIO_LEVEL_BIG);
					isTransforming = true;
					transform_start = GetTickCount64();
					//level -= 1;
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
	score += 50;
}

void CMario::OnCollisionWithItemCard(LPCOLLISIONEVENT e)
{
	CItemCard* itemCard = dynamic_cast<CItemCard*>(e->obj);
	if (itemCard->GetState() != ITEMCARD_STATE_BE_COLLECTED)
	{
		itemCard->SetState(ITEMCARD_STATE_BE_COLLECTED);
		AddCollectedItem(itemCard->GetType());
	}
	CGame::GetInstance()->SetIsHasCard(true);
	SetState(MARIO_STATE_COLLECTED_ITEM);
	/*CPlayScene* playScene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
	if (playScene)
	{
		playScene->AddItemCardToHUD(itemCard->GetType());
	}*/
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
	if (leaf->GetIsEaten()) return;

	leaf->OnDefeated();
	if (leaf != nullptr)
	{
			if (this->level == MARIO_LEVEL_BIG)
			{
				score += 1000;
				this->SetLevel(MARIO_LEVEL_RACCOON);
				//DebugOut(L"[INFO] Mario transform to Raccoon\n");
				isTransforming = true;
				transform_start = GetTickCount64();
				StartUntouchable();
			}
			else
				if (this->level == MARIO_LEVEL_SMALL)
				{
					score += 1000;
					this->SetLevel(MARIO_LEVEL_BIG);
					isTransforming = true;
					transform_start = GetTickCount64();
					StartUntouchable();
				}
	}
}

void CMario::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e)
{
	CQuestionBrick* qb = dynamic_cast<CQuestionBrick*>(e->obj);

	if (qb != nullptr  && e->ny > 0 && qb->GetState() != 90000 && qb->GetType() != -1)
	{
		qb->SetState(90000);
		if (qb->getIsEmpty() == 0)
		{
			qb->SetIsEmpty(1);
			qb->StartBounce();

			if (qb->getType() == 0)
			{
				score += 100;
				++coin;
				float coinX = qb->getX();
				float coinY = qb->getY() - QBRICK_BBOX_HEIGHT / 2 - COIN_BBOX_HEIGHT / 2;

				CCoin* coin = new CCoin(coinX, coinY);
				coin->StartBouncing();
				((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddObject(coin);
				qb->SpawnPoint();
			}
			else if ((qb->getType() == 1 ||qb->GetType() == 2) && (this->level == MARIO_LEVEL_SMALL || this->level == MARIO_LEVEL_RACCOON))
			{
				//DebugOut(L"[QBRICK] y: %f, startY: %f\n", qb->getY(), qb->getStartY());
				float mushroomX = qb->getX();
				float mushroomY = qb->getY() - QBRICK_BBOX_HEIGHT / 2;

				CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
				if (qb->GetType() == 2)
				{
					CMushroom* mushroom = new CMushroom(mushroomX, mushroomY, 2);
					scene->AddObjectBefore(mushroom, qb);
				}
				else
				{
					CMushroom* mushroom = new CMushroom(mushroomX, mushroomY, 1);
					scene->AddObjectBefore(mushroom, qb);

				}
			}
			else if (qb->getType() == 1 && this->level == MARIO_LEVEL_BIG)
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
	if (!CGame::GetInstance()->GetIsHasCard()) return;
	CPortal* p = (CPortal*)e->obj;
	this->SetWorld(4);
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithPlant(LPCOLLISIONEVENT e)
{
	CPlantEnemy* plant = dynamic_cast<CPlantEnemy*>(e->obj);
	if (plant->GetState() == 0) return;
	if (untouchable == 0)
	{
		if (level == MARIO_LEVEL_BIG)
		{
			this->SetLevel(MARIO_LEVEL_SMALL);
			isTransforming = true;
			transform_start = GetTickCount64();
			//level -= 1;
			StartUntouchable();
		}
		else if (level == MARIO_LEVEL_RACCOON)
		{
			this->SetLevel(MARIO_LEVEL_BIG);
			isTransforming = true;
			transform_start = GetTickCount64();
			//level -= 1;
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
		if (level == MARIO_LEVEL_BIG)
		{
			this->SetLevel(MARIO_LEVEL_SMALL);
			isTransforming = true;
			transform_start = GetTickCount64();
			//level -= 1;
			StartUntouchable();
		}
		else if (level == MARIO_LEVEL_RACCOON)
		{
			this->SetLevel(MARIO_LEVEL_BIG);
			isTransforming = true;
			transform_start = GetTickCount64();
			//level -= 1;
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
	if (!isOnPlatform && !isOnLift)
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
		else if (isKicking)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SMALL_KICK_RIGHT;
			else
			{
				aniId = ID_ANI_MARIO_SMALL_KICK_LEFT;
			}
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
	if (!isOnPlatform&&!isOnLift)
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
		else if (isKicking)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_KICK_RIGHT;
			else
			{
				aniId = ID_ANI_MARIO_KICK_LEFT;
			}
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

int CMario::GetAniIdRaccoon(){
	int aniId = -1;
	if (isFlying)
	{
		aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_FLY_RIGHT : ID_ANI_MARIO_RACCOON_FLY_LEFT;
	}
	else if (isGliding)
	{
		aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_GLIDE_RIGHT : ID_ANI_MARIO_RACCOON_GLIDE_LEFT;
	}

	else if (!isOnPlatform  && !isOnLift)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_RACCOON_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_SIT_LEFT;
		}
		else if (isKicking)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_RACCOON_KICK_RIGHT;
			else
			{
				aniId = ID_ANI_MARIO_RACCOON_KICK_LEFT;
			}
		}
		else
			if (vx == 0)
			{
				//DebugOut(L"[MARIO] dang dung yen : %d\n", state);
				if (nx > 0) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_RACCOON_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_RACCOON_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACCOON_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_RACCOON_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_RACCOON_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACCOON_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_RACCOON_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;
	//DebugOut(L"[MARIO] ani cua MARIO: %d, \n", aniId);
	//DebugOut(L"[MARIO] isFlying: %d, \n", isFlying);
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

		if (state == MARIO_STATE_DIE)
		{
			aniId = ID_ANI_MARIO_DIE;
			//animations->Get(aniId)->Render(x, y);
		}
		else if (isRunning)
		{
			if (this->GetLevel() == MARIO_LEVEL_BIG)
				aniId = ID_ANI_MARIO_WALKING_RIGHT; 
			else if (this->GetLevel() == MARIO_LEVEL_SMALL)
				aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			else if (this->GetLevel() == MARIO_LEVEL_RACCOON)
				aniId = ID_ANI_MARIO_RACCOON_WALKING_RIGHT;
		}
		
		else if (state==MARIO_STATE_TRAVELING)
		{
			aniId = ID_ANI_MARIO_TRAVEL;
			//animations->Get(aniId)->Render(x, y);
			//return;
		}
		else if (state == MARIO_STATE_KICK)
		{
			if (level == MARIO_LEVEL_BIG)
				aniId = (nx > 0) ? ID_ANI_MARIO_KICK_RIGHT : ID_ANI_MARIO_KICK_LEFT;
			else if (level == MARIO_LEVEL_SMALL)
				aniId = (nx > 0) ? ID_ANI_MARIO_SMALL_KICK_RIGHT : ID_ANI_MARIO_SMALL_KICK_LEFT;
			else if (level == MARIO_LEVEL_RACCOON)
				aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_KICK_RIGHT : ID_ANI_MARIO_RACCOON_KICK_LEFT;
			else if (isAttacking && level == MARIO_LEVEL_RACCOON)
			{
				aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_ATTACKING_RIGHT : ID_ANI_MARIO_RACCOON_ATTACKING_LEFT;
			}
		}
		else {
			if (facingDirection > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
			else aniId = ID_ANI_MARIO_IDLE_LEFT;

		if (state == MARIO_STATE_ATTACKING)
		{
			if (level == MARIO_LEVEL_RACCOON)
				aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_ATTACKING_RIGHT : ID_ANI_MARIO_RACCOON_ATTACKING_LEFT;
			//DebugOut(L"[MARIO] ATTACKING and ani %d\n",aniId);
		}
		else if (level == MARIO_LEVEL_BIG)
			aniId = GetAniIdBig();
		else if (level == MARIO_LEVEL_SMALL)
			aniId = GetAniIdSmall();
		else if (level == MARIO_LEVEL_RACCOON)
			aniId = GetAniIdRaccoon();

		}
		animations->Get(aniId)->Render(x, y);

		//RenderBoundingBox();

		//DebugOutTitle(L"Coins: %d", coin);
	}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == MARIO_LEVEL_BIG)
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
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_RACCOON)
	{
		if (isSitting)
		{
			left = x - MARIO_RACCOON_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_RACCOON_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_RACCOON_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_RACCOON_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_RACCOON_BBOX_WIDTH / 2;
			top = y - MARIO_RACCOON_BBOX_HEIGHT / 2;
			right = left + MARIO_RACCOON_BBOX_WIDTH;
			bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		if (tail) RemoveTail();
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	else if (this->level == MARIO_LEVEL_RACCOON)
	{
		DebugOut(L"[MARIO] dang o raccoon\n");
		if (!tail) CreateTail();
		DebugOutTitle(L"Dang o trang thai raccoon");
		y -= (MARIO_RACCOON_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	else if (this->level == MARIO_LEVEL_BIG)
	{
		if (tail) RemoveTail();
		DebugOutTitle(L"Dang o trang thai big");
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}
void CMario::UpdatePower(DWORD dt)
{
	if ((state == MARIO_STATE_RUNNING_RIGHT && ax > 0) ||
		(state == MARIO_STATE_RUNNING_LEFT && ax < 0))
	{
		if (power < MARIO_MAX_POWER)
			power += MARIO_POWER_UP_RATE * dt;
		else
			power = MARIO_MAX_POWER;
	}
	else
	{
		if (power > 0)
		{
			power -= MARIO_POWER_DOWN_RATE * dt;
			if (power < 0) power = 0;
		}
	}
}
void CMario::CreateTail()
{
	if (!tail)
	{
		tail = new CTail(x, y, nx);
		DebugOut(L"[TAIL] Create tail, x = %f, y = %f\n", x, y);
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddObject(tail);
	}
}

void CMario::RemoveTail()
{
	if (tail)
	{
		tail->Delete();
		tail = nullptr;
	}
}