#include "Koopas.h"
#include "PlayScene.h"
#include "Platform.h"
#include "Leaf.h"
CKoopas::CKoopas(float x, float y, float _spawnX)
	: CGameObject(x, y)
	, spawnX(_spawnX)
	, isActive(false)
{
	this->ax = 0;
	this->hit_start = 0;
	this->die_start = 0;
	this->ay = KOOPAS_GRAVITY;
	SetState(KOOPAS_STATE_WALKING);
	vx = -KOOPAS_WALKING_SPEED;
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
	else if (state == KOOPAS_STATE_HIT)
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
	//if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopas*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CQuestionBrick*>(e->obj))
		OnCollisionWithQuestionBrick(e);
}

void CKoopas::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	if (e->nx != 0 || e->ny != 0)
	{
		CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
		if (goomba->GetState() != GOOMBA_STATE_DIE && state == KOOPAS_STATE_HIT_MOVING)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
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
	if (!isActive)
	{
		float camX, camY;
		CGame::GetInstance()->GetCamPos(camX, camY);

		if (camX + CGame::GetInstance()->GetBackBufferWidth() / 2 >= this->spawnX)
			this->isActive = true;
		else
			return;
	}

	vy += ay * dt;
	vx += ax * dt;

	if (vx <= 0) 
	{
		if (IsNearEdge(coObjects))
		{
			vx = -vx; 
		}
	}
	else if (vx >= 0)
	{
		if (IsNearEdge(coObjects)) 
		{
			vx = -vx; 
		}
	}

	// Xử lý hồi sinh
	if (state == KOOPAS_STATE_HIT)
	{
		if (GetTickCount64() - hit_start > KOOPAS_REVIVE_TIMEOUT)
		{
			SetState(KOOPAS_STATE_REVIVE);
		}
	}
	else if (state == KOOPAS_STATE_REVIVE)
	{
		if (GetTickCount64() - hit_start > KOOPAS_REVIVE_TIMEOUT + 500)
		{
			SetState(KOOPAS_STATE_WALKING);
		}
	}

	// Xử lý bị xóa sau khi chết
	if ((state == KOOPAS_STATE_DIE) && (GetTickCount64() - die_start > KOOPAS_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

bool CKoopas::IsNearEdge(vector<LPGAMEOBJECT>* coObjects)
{
	float x, y;
	this->GetPosition(x, y);
	if (this->GetState() == KOOPAS_STATE_WALKING)
	{
		if (x <= 540 || x >= 627)
		{
			this->nx = -this->nx;
			return true;
		}
		
	}
	return false;
}

void CKoopas::Render()
{
	if (!isActive) return;

	int aniId = -1;
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
		aniId = ID_ANI_KOOPAS_HIT_MOVING; //MOVING
	}

	RenderBoundingBox();
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
	switch (state)
	{
	case KOOPAS_STATE_WALKING:
		newHeight = KOOPAS_BBOX_HEIGHT_WALK;
		vx = -KOOPAS_WALKING_SPEED;
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
		vx = (nx >= 0 ? KOOPAS_WALKING_SPEED * 4 : -KOOPAS_WALKING_SPEED * 4);
		vy = -0.1;
		ay = KOOPAS_GRAVITY;
		break;

	case KOOPAS_STATE_REVIVE:
		newHeight = KOOPAS_BBOX_HEIGHT_REVIVE;
		break;

	case KOOPAS_STATE_DIE:
		newHeight = KOOPAS_BBOX_HEIGHT_WALK;
		die_start = GetTickCount64();
		vx = 0;
		vy = 0;
		ay = 0;
		break;

	default:
		newHeight = KOOPAS_BBOX_HEIGHT_WALK;
		break;
	}

	if (oldHeight != newHeight)
		y += (oldHeight - newHeight) / 2.0f;
}
