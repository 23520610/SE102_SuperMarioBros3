#include "Goomba.h"
#include "PlayScene.h"
#include "Mario.h"
#include "PlayScene.h"

CGoomba::CGoomba(float x, float y, float _spawnX)
	: CGameObject(x, y)
	, spawnX(_spawnX)
	, isActive(false)    
{
	this->ax = 0;
	isBouncing = false;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	super_die_start = -1;
	SetState(GOOMBA_STATE_WALKING);
	vx = -GOOMBA_WALKING_SPEED;
	startY = y;
	pointY = y;
	pointStartTime = 0;
	isPointVisible = false;
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{ 
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::OnDefeated()
{
	isPointVisible = true;
	pointY = y;
	pointStartTime = GetTickCount64();
}


void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CGoomba*>(e->obj)) return; 

	if (e->ny != 0 )
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
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
		vy += GOOMBA_GRAVITY * dt;
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
	
	if ( (state==GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT) )
	{
		isDeleted = true;
		return;
	}
	if ((state == GOOMBA_STATE_SUPER_DIE) && (GetTickCount64() - super_die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	if (!isActive) return;
	//CScene* current_scene = (CScene*)CGame::GetInstance()->GetCurrentScene();

	int aniId = ID_ANI_GOOMBA_WALKING;
	if (state == GOOMBA_STATE_DIE) 
	{
		aniId = ID_ANI_GOOMBA_DIE;
	}
	else if (state == GOOMBA_STATE_SUPER_DIE)
	{
		aniId = ID_ANI_GOOMBA_SUPER_DIE;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	if (isPointVisible)
	{
		CAnimations::GetInstance()->Get(ID_ANI_POINT_100)->Render(x, pointY);
	}
	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;

	case GOOMBA_STATE_SUPER_DIE:
	{
		super_die_start = GetTickCount64();
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		CMario* mario = (CMario*)scene->GetPlayer();
		if (this->x > mario->getX())
			vx = abs(vx) * GOOMBA_SUPER_DIE_VX;
		else if (this->x <= mario->getX() && this->vx < 0)
			vx = vx * GOOMBA_SUPER_DIE_VX;
		else
			vx = -vx * GOOMBA_SUPER_DIE_VX;
		vy = -GOOMBA_SUPER_DIE_VY;
		break;
	}

	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	}
}


void CGoomba::StartBouncing()
{
	if (!isBouncing)
	{
		vy = BOUNCE_VELOCITY;
		isBouncing = true;
	}
}