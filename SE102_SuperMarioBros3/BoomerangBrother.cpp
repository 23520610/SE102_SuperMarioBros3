#include "BoomerangBrother.h"
#include "PlayScene.h"
#include "Mario.h"
#include "PlayScene.h"

CBoomerangBrother::CBoomerangBrother(float x, float y, float _spawnX)
	: CGameObject(x, y)
	, spawnX(_spawnX)
	, isActive(false)
{
	startX = x;
	this->ax = 0;
	isBouncing = false;
	this->ay = BOOMERANGBROTHER_GRAVITY;
	die_start = -1;
	super_die_start = -1;
	SetState(BOOMERANGBROTHER_STATE_WALKING);
	vx = -BOOMERANGBROTHER_WALKING_SPEED;
	startY = y;
	pointY = y;
	pointStartTime = 0;
	isPointVisible = false;
}

void CBoomerangBrother::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == BOOMERANGBROTHER_STATE_DIE)
	{
		left = x - BOOMERANGBROTHER_BBOX_WIDTH / 2;
		top = y - BOOMERANGBROTHER_BBOX_HEIGHT_DIE / 2;
		right = left + BOOMERANGBROTHER_BBOX_WIDTH;
		bottom = top + BOOMERANGBROTHER_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - BOOMERANGBROTHER_BBOX_WIDTH / 2;
		top = y - BOOMERANGBROTHER_BBOX_HEIGHT / 2;
		right = left + BOOMERANGBROTHER_BBOX_WIDTH;
		bottom = top + BOOMERANGBROTHER_BBOX_HEIGHT;
	}
}

void CBoomerangBrother::OnDefeated()
{
	isPointVisible = true;
	pointY = y;
	pointStartTime = GetTickCount64();
}


void CBoomerangBrother::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CBoomerangBrother::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CBoomerangBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
	if (isBouncing)
	{
		vy += BOOMERANGBROTHER_GRAVITY * dt;
		y += vy * dt;
		if (y >= startY + 500)
		{
			//DebugOut(L"[COIN] Done bouncing! y = %.2f\n", y);
			isBouncing = false;
			y = startY;
			this->isDeleted = true;
		}
	}
	if (isPointVisible)
	{
		DebugOut(L"[POINT] Done bouncing! y = %.2f\n", y);
		if (pointY > y - 30)
			pointY -= 0.05f * dt;
		else
			pointY = y - 10;

		if (GetTickCount64() - pointStartTime > 500)
			isPointVisible = false;
	}

	vy += ay * dt;
	vx += ax * dt;

	if ((state == BOOMERANGBROTHER_STATE_DIE) && (GetTickCount64() - die_start > BOOMERANGBROTHER_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	if ((state == BOOMERANGBROTHER_STATE_SUPER_DIE) && (GetTickCount64() - super_die_start > BOOMERANGBROTHER_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	if (this->x > startX + maxDistanceX) {
		this->x = startX + maxDistanceX;
		vx = -vx;
	}
	else if (this->x < startX - maxDistanceX) {
		this->x = startX - maxDistanceX;
		vx = -vx;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CBoomerangBrother::Render()
{
	if (!isActive) return;
	//CScene* current_scene = (CScene*)CGame::GetInstance()->GetCurrentScene();

	int aniId = ID_ANI_BOOMERANGBROTHER_WALKING;
	if (state == BOOMERANGBROTHER_STATE_DIE)
	{
		aniId = ID_ANI_BOOMERANGBROTHER_DIE;
	}
	else if (state == BOOMERANGBROTHER_STATE_SUPER_DIE)
	{
		aniId = ID_ANI_BOOMERANGBROTHER_SUPER_DIE;
	}
	if (isPointVisible)
	{
		CAnimations::GetInstance()->Get(ID_ANI_POINT_1000)->Render(x, pointY);
		isPointVisible = false;
	}
	else
	{
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	}
	//RenderBoundingBox();
}

void CBoomerangBrother::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOOMERANGBROTHER_STATE_DIE:
		die_start = GetTickCount64();
		vx = 0;
		vy = 0.05f;
		ay = 0;
		break;

	case BOOMERANGBROTHER_STATE_SUPER_DIE:
	{
		super_die_start = GetTickCount64();
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		CMario* mario = (CMario*)scene->GetPlayer();
		if (this->x > mario->getX())
			vx = abs(vx) * BOOMERANGBROTHER_SUPER_DIE_VX;
		else if (this->x <= mario->getX() && this->vx < 0)
			vx = vx * BOOMERANGBROTHER_SUPER_DIE_VX;
		else
			vx = -vx * BOOMERANGBROTHER_SUPER_DIE_VX;
		vy = -BOOMERANGBROTHER_SUPER_DIE_VY;
		break;
	}

	case BOOMERANGBROTHER_STATE_WALKING:
		vx = -BOOMERANGBROTHER_WALKING_SPEED;
		break;
	}
}


void CBoomerangBrother::StartBouncing()
{
	if (!isBouncing)
	{
		vy = BOUNCE_VELOCITY;
		isBouncing = true;
	}
}