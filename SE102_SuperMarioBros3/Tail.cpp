#include "Tail.h"
#include "Goomba.h"
#include "Koopas.h"
#include "QuestionBrick.h"
#include "debug.h"
#include "Collision.h"
#include "PlayScene.h"
#include "Effect.h"
#include "Button.h"
#include "Mario.h"
#include "ParaTroopa.h"
#include "BoomerangBrother.h"
#include "RedParaTroopa.h"
#include "PlantEnemy.h"
CTail::CTail(float x, float y, int nx)
{
    this->nx = nx;
    this->x = x;
    this->y = y;
    isAttacking = false;
    attack_start = 0;
}

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (isAttacking && GetTickCount64() - attack_start >= TAIL_DURATION)
    {
        isAttacking = false;
        attack_start = 0;
        //DebugOut(L"[TAIL] Stop attacking\n");
    }

    CGameObject::Update(dt, coObjects);

    // Xử lý va chạm kiểu 2 boundingbox đè lên nhau->va chạm
    if (isAttacking)
    {
        float l1, t1, r1, b1;
        GetBoundingBox(l1, t1, r1, b1);

        for (LPGAMEOBJECT obj : *coObjects)
        {
            if (!obj->IsCollidable()) continue;

            float l2, t2, r2, b2;
            obj->GetBoundingBox(l2, t2, r2, b2);

            bool isOverlapping = !(r1 < l2 || r2 < l1 || b1 < t2 || b2 < t1);
            if (isOverlapping)
            {
                CCollisionEvent* e = new CCollisionEvent(1.0f, 0, 0, 0, 0, obj, this);
                OnCollisionWith(e);
                delete e;
            }
        }
    }

}

void CTail::UpdatePosition(float mario_x, float mario_y, int mario_nx)
{
    nx = mario_nx;
    y = mario_y+5;

    if (nx > 0)
        x = mario_x + 10;
    else
        x = mario_x - 10;
}

void CTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    if (!isAttacking)
    {
        left = right = top = bottom = 0;
        return;
    }
    if (nx > 0)
    {
        left = x;
        right = left + TAIL_BBOX_WIDTH/2;
    }
    else
    {
        right = x;
        left = right - TAIL_BBOX_WIDTH/2;
    }

    top = y - TAIL_BBOX_HEIGHT / 2;
    bottom = top + TAIL_BBOX_HEIGHT/2;
}

void CTail::OnCollisionWith(LPCOLLISIONEVENT e)
{
    //DebugOut(L"[TAIL] OnCollisionWith\n");
    if (dynamic_cast<CGoomba*>(e->obj))
    {
        DebugOut(L"[TAIL] Attacked Goomba\n");
        OnCollisionWithGoomba(e);
    }

    if (dynamic_cast<CKoopas*>(e->obj))
    {
        DebugOut(L"[TAIL] Attacked Koopa\n");
        OnCollisionWithKoopa(e);
    }

    if (dynamic_cast<CBrick*>(e->obj))
        OnCollisionWithGoldBrick(e);
	if (dynamic_cast<CParaTroopa*>(e->obj))
		OnCollisionWithParaTroopa(e);
    if (dynamic_cast<CBoomerangBrother*>(e->obj))
        OnCollisionWithBoomerangBro(e);
    if (dynamic_cast<CPlantEnemy*>(e->obj))
        OnCollisionWithPLant(e);
}

void CTail::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
    CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
    CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
    CEffect* effect = new CEffect(goomba->GetX(), goomba->GetY(), ID_ANI_RACCOON_HIT_EFFECT, 0, 0, 50); 
    scene->AddEffect(effect);
    if (goomba)
        goomba->SetState(GOOMBA_STATE_SUPER_DIE);
}

void CTail::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
    CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
    CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
    CEffect* effect = new CEffect(koopas->GetX(), koopas->GetY(), ID_ANI_RACCOON_HIT_EFFECT, 0, 0, 50);
    scene->AddEffect(effect);
    if (koopas)
        koopas->SetState(KOOPAS_STATE_DIE);
}

void CTail::OnCollisionWithGoldBrick(LPCOLLISIONEVENT e)
{
    CBrick* brick = dynamic_cast<CBrick*>(e->obj);
   if (brick != nullptr && (brick->GetType()==0 || brick->GetType() == 3 || brick->GetType() == 2 || brick->GetType() == 5)) 
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
        DebugOut(L"[BUTTON] BUTTON:, HAS BUTTON = %d\n", brick->GetButton());
        //DebugOut(L"[BUTTON] BUTTON:, is spawn = %d\n", CButton::GetIsSpawn());
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
   
}

void CTail::OnCollisionWithParaTroopa(LPCOLLISIONEVENT e)
{
    CParaTroopa* troopa = dynamic_cast<CParaTroopa*>(e->obj);
    CRedParaTroopa* redParaTroopa = dynamic_cast<CRedParaTroopa*>(e->obj);
    CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
    CEffect* effect = new CEffect(troopa->GetX(), troopa->GetY(), ID_ANI_RACCOON_HIT_EFFECT, 0, 0, 50);
    scene->AddEffect(effect);
    if (redParaTroopa )
    {
        redParaTroopa->getHited();
    }
    else if (troopa )
    {
        troopa->beHited();
    }
        
}
void CTail::OnCollisionWithBoomerangBro(LPCOLLISIONEVENT e)
{
    CBoomerangBrother* boomerangbro = dynamic_cast<CBoomerangBrother*>(e->obj);
    CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
    CEffect* effect = new CEffect(boomerangbro->GetX(), boomerangbro->GetY(), ID_ANI_RACCOON_HIT_EFFECT, 0, 0, 50);
    scene->AddEffect(effect);
    if (boomerangbro)
        boomerangbro->SetState(BOOMERANGBROTHER_STATE_SUPER_DIE);
}
void CTail::OnCollisionWithPLant(LPCOLLISIONEVENT e) {
	CPlantEnemy* plant = dynamic_cast<CPlantEnemy*>(e->obj);
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CEffect* effect = new CEffect(plant->getX(), plant->getY(), ID_ANI_RACCOON_HIT_EFFECT, 0, 0, 50);
	scene->AddEffect(effect);
	if (plant)
	{
		plant->Delete();
		CMario* mario = (CMario*)scene->GetPlayer();
		mario->SetScore(mario->GetScore() + 1000);
	}
}