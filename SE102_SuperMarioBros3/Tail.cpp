#include "Tail.h"
#include "Goomba.h"
#include "Koopas.h"
#include "QuestionBrick.h"
#include "debug.h"
#include "Collision.h"
#include "PlayScene.h"
#include "Effect.h"
#include "Mario.h"

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
        DebugOut(L"[TAIL] Stop attacking\n");
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
    DebugOut(L"[TAIL] OnCollisionWith\n");

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
}

void CTail::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
    CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
    CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
    CEffect* effect = new CEffect(goomba->GetX(), goomba->GetY(), ID_ANI_RACCOON_HIT_EFFECT, 0, 0, 50); 
    scene->AddEffect(effect);
    if (goomba)
        goomba->SetState(GOOMBA_STATE_DIE);
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
    
}


