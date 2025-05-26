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
#include "Boomerang.h"

	void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>*coObjects)
	{

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

		if (y > MARIO_DEAD_Y && !inSafeZone&& state!=MARIO_STATE_TRAVELING)
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
		if (!isOnPlatform && level == MARIO_LEVEL_RACCOON && CGame::GetInstance()->IsKeyDown(DIK_S))
		{
			SetState(nx > 0 ? MARIO_STATE_GLIDING_RIGHT : MARIO_STATE_GLIDING_LEFT);
		}
		if (state == MARIO_STATE_FLYING_RIGHT || state == MARIO_STATE_FLYING_LEFT)
		{
			if (GetTickCount64() - fly_start >= MARIO_FLY_DURATION)
			{
				ay = MARIO_GRAVITY;
				fly_start = 0;
				//SetState(MARIO_STATE_IDLE);
				SetState(nx > 0 ? MARIO_STATE_GLIDING_RIGHT : MARIO_STATE_GLIDING_LEFT);
			}
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
					SetPosition(2201,386); 
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
		//DebugOut(L"[Info]: Co tren platform %d\n", isOnPlatform);
		CCollision::GetInstance()->Process(this, dt, coObjects);
		checkOnLift();
	
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
	else if (dynamic_cast<CBoomerang*>(e->obj))
		OnCollisionWithGoldBoomerang(e);
}

bool CMario::IsHoldingKeyPressed()
{
	return CGame::GetInstance()->IsKeyDown(DIK_A);
}
void CMario::OnCollisionWithGoldBoomerang(LPCOLLISIONEVENT e) {
	CBoomerang* boomerang = dynamic_cast<CBoomerang*>(e->obj);
	if (boomerang != nullptr) {
		if (untouchable == 0)
		{		
			boomerang->Delete();
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
			if (boomerangbro->GetState() != BOOMERANGBROTHER_STATE_DIE)
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
			if (isOnPlatform||isOnLift)
			{
				ay=MARIO_GRAVITY;
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
			break;
		case MARIO_STATE_KICK:
			vx = 0;
			ax = 0;
			kick_start = GetTickCount64();
			//isKicking = false;
			break;
		case MARIO_STATE_DIE:
			ay = MARIO_GRAVITY;
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
			if (isSitting) break;
			if (fly_start == 0)
			{
				fly_start = GetTickCount64();
			}
			if (power >= MARIO_MAX_POWER) {
				ay = 0;
				vy = -MARIO_FLYING_SPEED;
				nx = 1;
				isFlying = true;
				fly_start = GetTickCount64();
				break;
			}
			break;
		case MARIO_STATE_FLYING_LEFT:
			if (fly_start == 0) // chỉ set nếu chưa bay
			{
				fly_start = GetTickCount64();
			}
			if (power >= MARIO_MAX_POWER)
			{
				ay = 0;
				vy = -MARIO_FLYING_SPEED;
				nx = -1;
				isFlying = true;
				fly_start = GetTickCount64();
				break;
			}
			break;
		case MARIO_STATE_GLIDING_RIGHT:
			if (isSitting) break;
			if (isOnPlatform) break;
			if (power >= MARIO_MAX_POWER) {
				ay = MARIO_GLIDING_SPEED;
				nx = 1;
				isFlying = true;
				break;
			}
		case MARIO_STATE_GLIDING_LEFT:
			if (isOnPlatform) break;
			if (power >= MARIO_MAX_POWER)
			{
				ay = MARIO_GLIDING_SPEED;
				nx = -1;
			}
			break;

		case MARIO_STATE_ATTACKING:
			if (isSitting || isFlying || isGliding || !isOnPlatform) break;
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
void CMario::checkOnLift() {
	bool foundLift = false;
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	vector<LPGAMEOBJECT> objects = scene->GetObjects();
	for (auto obj : objects) {
		CLift* lift = dynamic_cast<CLift*>(obj);
		if (lift) {
			float l, t, r, b;
			lift->GetBoundingBox(l, t, r, b);
			float mx, my, mr, mb;
			this->GetBoundingBox(mx, my, mr, mb);
			if (mr > l && mx < r && abs(mb - t) < 4.0f && vy >= 0) {
				foundLift = true;
				break;
			}
		}
	}
	isOnLift = foundLift;
	if (isOnLift) {
		ay = LIFT_GRAVITY;
	}
	else {
		ay = MARIO_GRAVITY;
	}
}
