#include "Lift.h"
#include "debug.h"

void CLift::Render()
{
	if (isActive);
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
	CMario* mario = dynamic_cast<CMario*>(e->obj);
	if (mario && e->nx < 0) {
		//mario->SetX(mario->getX() + LIFT_SPEED_X ); 
		return;
	}
	else if (!e->obj->IsBlocking()) return;
}

void CLift::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float cameraX, camY;
	CGame::GetInstance()->GetCamPos(cameraX, camY);
	float SCREEN_WIDTH = CGame::GetInstance()->GetBackBufferWidth();
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)scene->GetPlayer();
	//DebugOut(L"[UPDATE]Vi tri cam %f , world %d \n", cameraX, mario->GetWorld());
	//DebugOut(L"[UPDATE]: camX = %f, spawnX = %f, isActive = %d\n", cameraX, spawnX, isActive);
	if (cameraX + SCREEN_WIDTH / 2 < spawnX ) {
		isActive = false;
	}
	if (!isActive)
	{
		/*if (cameraX + SCREEN_WIDTH / 2 < spawnX) {
		
			DebugOut(L"Vi tri cam %f, world %d \n", cameraX, mario->GetWorld());
		}*/
		//DebugOut(L"Vi tri cam %f , world %d \n", cameraX, mario->GetWorld() );
		//DebugOut(L"Lift: camX = %f, spawnX = %f, isActive = %d\n", cameraX, spawnX, isActive);
		//DebugOut(L"SCREEN WIDTH %f \n", SCREEN_WIDTH);
		if (cameraX +SCREEN_WIDTH/2 >= spawnX) {
			this->isActive = true;
			//DebugOut(L"Lift: camX = %f, spawnX = %f, isActive = %d\n", cameraX, spawnX, isActive);

		}
		else
			return;
	}
	if (isActive) vx = -LIFT_SPEED_X;
	vy += ay * dt;
	vx += ax * dt;
	if (y > LIFT_MAX_Y) {
		this->Delete();
		isActive = false;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CLift::TriggerFall() {
	isTouched = true;
	ay = LIFT_GRAVITY;
	ax = 0;
	vx = 0;
	ax = 0;
}
int CLift::IsCollidable()
{
	return checkIsMario() ? 1 : 0;
}

bool CLift::checkIsMario()
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)scene->GetPlayer();
	if (!mario) return false;

	float ml, mt, mr, mb;
	mario->GetBoundingBox(ml, mt, mr, mb);

	float l, t, r, b;
	GetBoundingBox(l, t, r, b);
	float Left = l - 8.0f;
	bool checkX = !(mr < Left || ml > r);
	bool checkY = !(mb < t || mt > b);

	return checkX && checkY;
}
