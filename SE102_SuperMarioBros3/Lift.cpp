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
		float cameraX, camY;
		CGame::GetInstance()->GetCamPos(cameraX, camY);
		float SCREEN_WIDTH = CGame::GetInstance()->GetBackBufferWidth();
		DebugOut(L"Vi tri cam %f \n", cameraX);
		DebugOut(L"SCREEN WIDTH %f \n", SCREEN_WIDTH);
		if (cameraX +SCREEN_WIDTH/2 >= spawnX) {
			this->isActive = true;
			DebugOut(L"Lift: camX = %f, spawnX = %f, isActive = %d\n", cameraX, spawnX, isActive);

		}
		else
			return;
	}
	if (isActive) vx = -LIFT_SPEED_X;
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