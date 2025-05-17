#include "Koopas.h"
#include "PlayScene.h"
#include "Platform.h"
#include "Brick.h"
#include "Leaf.h"
#include "debug.h"
#include "FireBall.h"
#include "Button.h"
#include "Effect.h"
#include "Coin.h"
CKoopas::CKoopas(float x, float y, float _spawnX, int type)
	: CGameObject(x, y)
	, spawnX(_spawnX)
	, isActive(false)
{
	this->kooopasType = type;
	this->ax = 0;
	this->hit_start = 0;
	this->die_start = 0;
	this->ay = KOOPAS_GRAVITY;
	SetState(KOOPAS_STATE_WALKING);
	vx = -KOOPAS_WALKING_SPEED;
}

void CKoopas::OnDefeated()
{
	isPointVisible = true;
	pointY = y;
	pointStartTime = GetTickCount64();
}


void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - KOOPAS_BBOX_WIDTH_WALK / 2;
	top = y - KOOPAS_BBOX_HEIGHT_WALK / 2;

	if (state == KOOPAS_STATE_WALKING)
	{
		right = x + KOOPAS_BBOX_WIDTH_WALK / 2;
		bottom = y + KOOPAS_BBOX_HEIGHT_WALK / 2;
	}
	else if (state == KOOPAS_STATE_HIT||state == KOOPAS_STATE_DIE)
	{
		right = x + KOOPAS_BBOX_WIDTH_HIT / 2;
		bottom = y + KOOPAS_BBOX_HEIGHT_HIT / 2;
	}
	else if (state == KOOPAS_STATE_REVIVE)
	{
		right = x + KOOPAS_BBOX_WIDTH_REVIVE / 2;
		bottom = y + KOOPAS_BBOX_HEIGHT_REVIVE / 2;
	}
	else if (state == KOOPAS_STATE_HIT_MOVING)
	{
		right = x + KOOPAS_BBOX_WIDTH_HIT / 2;
		bottom = y + KOOPAS_BBOX_HEIGHT_HIT / 2;
	}
	else
	{
		right = x + KOOPAS_BBOX_WIDTH_WALK / 2;
		bottom = y + KOOPAS_BBOX_HEIGHT_WALK / 2;
	}
}

void CKoopas::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CFireBall*>(e->obj)) return;
	if (dynamic_cast<CCoin*>(e->obj)) return;
	//if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopas*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	//else if (e->nx != 0)
	//{
	//	vx = -vx;
	//}
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CQuestionBrick*>(e->obj))
		OnCollisionWithQuestionBrick(e);
	else if (dynamic_cast<CPlatform*>(e->obj))
		OnCollisionWithPlatform(e);
	else if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithGoldBrick(e);

	if (e->nx != 0 && !dynamic_cast<CGoomba*>(e->obj))
	{
		vx = -vx;
	}
}

void CKoopas::OnCollisionWithGoldBrick(LPCOLLISIONEVENT e)
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)scene->GetPlayer();

	if (e->ny < 0)
	{
		CBrick* brick = dynamic_cast<CBrick*>(e->obj);
		if (brick)
		{
			float px, py, pr, pb;
			brick->GetBoundingBox(px, py, pr, pb);

			float koopas_l, koopas_t, koopas_r, koopas_b;
			this->GetBoundingBox(koopas_l, koopas_t, koopas_r, koopas_b);

			if (this->GetState() == KOOPAS_STATE_WALKING)
			{
				if (!isTurning &&
					(x <= px + EDGE_MARGIN || x >= pr - EDGE_MARGIN))
				{
					vx = -vx;
					isTurning = true;
				}
				else if (x > px + EDGE_MARGIN && x < pr - EDGE_MARGIN)
				{
					isTurning = false;
				}
			}
		}
	}
	else if (!e->ny>0)
	{
		CBrick* goldBrick = dynamic_cast<CBrick*>(e->obj);
		if (goldBrick != nullptr && this->GetState() == KOOPAS_STATE_HIT_MOVING)
		{
			goldBrick->Delete();

			float vx_initial[] = { -1.5f, -1.5f, 1.5f, 1.5f };
			float vy_initial[] = { -0.5f, -0.4f, -0.4f, -0.5f };

			for (int i = 0; i < 4; i++)
			{
				CEffect* effect = new CEffect(goldBrick->GetX(), goldBrick->GetY(), ID_ANI_BREAK_EFFECT, vx_initial[i], vy_initial[i], 1000);
				scene->AddEffect(effect);
			}
			
			if (goldBrick->GetButton())
			{
				CButton::SpawnButton(goldBrick->GetX(), goldBrick->GetY());
			}
				
		}
	}
}

void CKoopas::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	if (e->ny < 0)
	{
		CPlatform* platform = dynamic_cast<CPlatform*>(e->obj);
		if (platform && !platform->GetIsGround())
		{
			if (this->y < 384 && this->kooopasType == 2)
			{
				float px, py, pr, pb;
				platform->GetBoundingBox(px, py, pr, pb);

				float koopas_l, koopas_t, koopas_r, koopas_b;
				this->GetBoundingBox(koopas_l, koopas_t, koopas_r, koopas_b);

				if (this->GetState() == KOOPAS_STATE_WALKING)
				{
					if (!isTurning &&
						(x <= px + EDGE_MARGIN || x >= pr - EDGE_MARGIN))
					{
						vx = -vx;
						isTurning = true;
					}
					else if (x > px + EDGE_MARGIN && x < pr - EDGE_MARGIN)
					{
						isTurning = false;
					}
				}
			}

		}
	}
}

void CKoopas::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	if (e->nx != 0 || e->ny != 0)
	{
		CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
		if (goomba->GetState() != GOOMBA_STATE_DIE && state == KOOPAS_STATE_HIT_MOVING)
		{
			goomba->SetState(GOOMBA_STATE_BOUNCE);
			goomba->SetState(GOOMBA_STATE_DIE);
			goomba->StartBouncing();
		}

	}
}

void CKoopas::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e)
{
	CQuestionBrick* qb = dynamic_cast<CQuestionBrick*>(e->obj);
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)scene->GetPlayer();
	if (qb != nullptr && e->nx < 0 && qb->GetState() != 90000)
	{
		qb->SetState(90000);
		if (qb->getType() == 1 && mario->GetLevel() == MARIO_LEVEL_BIG)
		{
			float leafX = qb->getX();
			float leafY = qb->getY() - LEAF_BBOX_HEIGHT / 2 - LEAF_BBOX_HEIGHT / 2;

			CLeaf* leaf = new CLeaf(leafX, leafY);
			leaf->StartBouncing();
			scene->AddObject(leaf);
		}
		else if (qb->getType() == 1 && mario->GetLevel() == MARIO_LEVEL_SMALL)
		{
			float mushroomX = qb->getX();
			float mushroomY = qb->getY() - QBRICK_BBOX_HEIGHT / 2;

			CMushroom* mushroom = new CMushroom(mushroomX, mushroomY);
			((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddObject(mushroom);
			((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddObject(qb);
		}
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//if (!isActive)
	//{
	//	float camX, camY;
	//	CGame::GetInstance()->GetCamPos(camX, camY);

	//	if (camX + CGame::GetInstance()->GetBackBufferWidth() / 2 >= this->spawnX)
	//		this->isActive = true;
	//	else
	//		return;
	//}

	if (this->y >= 415) this->Delete();
	// HUONG MAT
	if (vx > 0)
	{
		nx = 1;
	}
	else if (vx < 0)
	{
		nx = -1;
	}
	if ((state == KOOPAS_STATE_HIT &&
		GetTickCount64() - hit_start > KOOPAS_REVIVE_TIMEOUT))
	{
		SetState(KOOPAS_STATE_REVIVE);
		if (beingHeld)
		{
			vy = -0.25f;
			vx = 0.0f;
		}
		//else return;
	}

	if (isPointVisible)
	{
		//DebugOut(L"[POINT] Done bouncing! y = %.2f\n", y);
		if (pointY > y - 30)
			pointY -= 0.05f * dt;
		else
			pointY = y - 10;

		if (GetTickCount64() - pointStartTime > 500)
			isPointVisible = false;
	}

	if (state == KOOPAS_STATE_REVIVE &&
		GetTickCount64() - hit_start > KOOPAS_REVIVE_TIMEOUT + 500 && !hasRevived)
	{
		SetState(KOOPAS_STATE_WALKING);
		revive_time = GetTickCount64();
		hasRevived = true;
	}

	if (hasRevived && GetTickCount64() - revive_time > 200)
	{
		if (beingHeld)
		{
			CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
			CMario* mario = (CMario*)scene->GetPlayer();
			mario->SetState(MARIO_STATE_DIE);
		}
		hasRevived = false;
	}

	if (beingHeld) return;
	// Tính toán gia tốc
	vy += ay * dt;
	vx += ax * dt;

	if (state == KOOPAS_STATE_DIE) {
		if (GetTickCount64() - die_start > KOOPAS_DIE_TIMEOUT)
		{
			vx = ax*dt;
			vy = ay*dt;
		}
		if (GetTickCount64() - die_start > KOOPAS_REVIVE_TIMEOUT) {
			SetState(KOOPAS_STATE_REVIVE);
			if (beingHeld)
			{
				vy = -0.25f;
				vx = 0.0f;
			}
		}
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopas::Render()
{
	//if (!isActive) return;
	int aniId = -1;
	if (this->GetType() == 2)
	{
		if (vx > 0)
		{
			aniId = ID_ANI_KOOPAS_WALKING_RIGHT;
		}
		else
		{
			aniId = ID_ANI_KOOPAS_WALKING_LEFT;
		}

		if (state == KOOPAS_STATE_HIT)
		{
			aniId = ID_ANI_KOOPAS_HIT;
		}
		else if (state == KOOPAS_STATE_REVIVE)
		{
			aniId = ID_ANI_KOOPAS_REVIVE;
		}
		else if (state == KOOPAS_STATE_HIT_MOVING)
		{
			aniId = ID_ANI_KOOPAS_HIT_MOVING;
		}
		else if (state == KOOPAS_STATE_DIE)
		{
			aniId = ID_ANI_KOOPAS_DIE;
		}
		//RenderBoundingBox();
	}
	else if (this->GetType() == 1)
	{
		if (vx > 0)
		{
			aniId = ID_ANI_GREENKOOPAS_WALKING_RIGHT;
		}
		else
		{
			aniId = ID_ANI_GREENKOOPAS_WALKING_LEFT;
		}

		if (state == KOOPAS_STATE_HIT)
		{
			aniId = ID_ANI_GREENKOOPAS_HIT;
		}
		else if (state == KOOPAS_STATE_REVIVE)
		{
			aniId = ID_ANI_GREENKOOPAS_REVIVE;
		}
		else if (state == KOOPAS_STATE_HIT_MOVING)
		{
			aniId = ID_ANI_GREENKOOPAS_HIT_MOVING;
		}
		else if (state == KOOPAS_STATE_DIE)
		{
			aniId = ID_ANI_GREENKOOPAS_DIE;
		}
		//RenderBoundingBox();
	}
	if (isPointVisible)
	{
		if (this->GetState() == KOOPAS_STATE_HIT)
			CAnimations::GetInstance()->Get(ID_ANI_POINT_100)->Render(x, pointY);
		else
			CAnimations::GetInstance()->Get(ID_ANI_POINT_200)->Render(x, pointY);
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CKoopas::SetState(int state)
{
	float oldHeight;
	switch (this->state)
	{
	case KOOPAS_STATE_WALKING:
		oldHeight = KOOPAS_BBOX_HEIGHT_WALK;
		break;
	case KOOPAS_STATE_HIT:
	case KOOPAS_STATE_HIT_MOVING:
		oldHeight = KOOPAS_BBOX_HEIGHT_HIT;
		break;
	case KOOPAS_STATE_REVIVE:
		oldHeight = KOOPAS_BBOX_HEIGHT_REVIVE;
		break;
	default:
		oldHeight = KOOPAS_BBOX_HEIGHT_WALK;
		break;
	}

	CGameObject::SetState(state);

	float newHeight;
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)scene->GetPlayer();

	switch (state)
	{
	case KOOPAS_STATE_WALKING:
		newHeight = KOOPAS_BBOX_HEIGHT_WALK;
		//vx = -KOOPAS_WALKING_SPEED;
		DebugOut(L"Koopas nx! ", nx);
		vx = (nx > 0) ? KOOPAS_WALKING_SPEED : -KOOPAS_WALKING_SPEED;
		ay = KOOPAS_GRAVITY;
		break;

	case KOOPAS_STATE_HIT:
		newHeight = KOOPAS_BBOX_HEIGHT_HIT;
		hit_start = GetTickCount64();
		vx = 0;
		vy = 0;
		ay = 0;
		break;

	case KOOPAS_STATE_HIT_MOVING:
		newHeight = KOOPAS_BBOX_HEIGHT_HIT;
		vx = (mario->GetFacingDirection() > 0 ? KOOPAS_WALKING_SPEED * 6 : -KOOPAS_WALKING_SPEED * 6);
		vy = -0.1;
		ay = KOOPAS_GRAVITY;
		break;

	case KOOPAS_STATE_REVIVE:
		newHeight = KOOPAS_BBOX_HEIGHT_REVIVE;
		ay = -KOOPAS_GRAVITY / 1000;
		break;

	case KOOPAS_STATE_DIE:
		newHeight = KOOPAS_BBOX_HEIGHT_HIT;
		die_start = GetTickCount64();
		if (this->x >= mario->getX())
			vx = KOOPAS_DIE_VX;  
		else
			vx = -KOOPAS_DIE_VX; 

		vy = -KOOPAS_DIE_VY;
		ay = KOOPAS_GRAVITY;
		break;

	default:
		newHeight = KOOPAS_BBOX_HEIGHT_WALK;
		break;
	}

	if (oldHeight != newHeight)
		y += (oldHeight - newHeight) / 2.0f;
}
