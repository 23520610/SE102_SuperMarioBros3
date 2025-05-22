#include "Lift.h"
#include "debug.h"

void CLift::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_LIFT)->Render(x, y);
	//RenderBoundingBox();
}
void CLift::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LIFT_BBOX_WIDTH / 2;
	t = y - LIFT_BBOX_HEIGHT / 2;
	r = l + LIFT_BBOX_WIDTH;
	b = t + LIFT_BBOX_HEIGHT;
}
#include "Goomba.h"
#include "PlayScene.h"
#include "Mario.h"
#include "PlayScene.h"


void CLift::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CLift::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
}

void CLift::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isActive)
	{
		float camX, camY;
		CGame::GetInstance()->GetCamPos(camX, camY);

		if (camX + CGame::GetInstance()->GetBackBufferWidth() / 2 >= this->spawnX)
		{
			this->isActive = true;
			vx -= LIFT_SPEED_X;
		}
		else
			return;
	}
	vy += ay * dt;
	vx += ax * dt;


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CLift::TriggerFall() {
	isTouched = true;
	ay = LIFT_GRAVITY;
	vx = 0;
}