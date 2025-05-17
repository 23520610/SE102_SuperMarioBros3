#include "Button.h"
#include "PlayScene.h"
void CButton::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (isBroken)
		animations->Get(ID_ANI_BUTTON_BROKEN)->Render(x, y);
	else
		animations->Get(ID_ANI_BUTTON)->Render(x, y);

	//RenderBoundingBox();
}

void CButton::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BUTTON_BBOX_WIDTH / 2;
	r = x + BUTTON_BBOX_WIDTH / 2;

	if (!isBroken)
	{
		t = y - BUTTON_BBOX_HEIGHT / 2;
		b = y + BUTTON_BBOX_HEIGHT / 2;
	}
	else
	{
		t = y - BUTTON_BROKEN_BBOX_HEIGHT / 2;
		b = y + BUTTON_BROKEN_BBOX_HEIGHT / 2;
	}
}

void CButton::SpawnButton(float x, float y)
{
	CButton* button = new CButton(x, y);
	CQuestionBrick* qbrick = new CQuestionBrick(x, y, -1);

	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	if (scene)
	{
		scene->AddObject(button);
		scene->AddObject(qbrick);
	}
		
}

void CButton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isSpawning)
	{
		y += vy * dt;

		if (y <= targetY)
		{
			y = targetY;
			vy = 0;
			isSpawning = false;
		}
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
