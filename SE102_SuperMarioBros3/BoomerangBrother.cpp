#include "BoomerangBrother.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Boomerang.h"

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
    jump_start = 0;
    throw_start = 0;
    boomerangCount = 0;
    isThrowing = false;
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
	else if (state == BOOMERANGBROTHER_STATE_THROW)
	{
		left = x - BOOMERANGBROTHER_BBOX_WIDTH / 2;
		top = y - BOOMERANGBROTHER_BBOX_HEIGHT_THROW / 2;
		right = left + BOOMERANGBROTHER_BBOX_WIDTH;
		bottom = top + BOOMERANGBROTHER_BBOX_HEIGHT_THROW;
	}
    else
    {
        left = x - BOOMERANGBROTHER_BBOX_WIDTH / 2;
        top = y - BOOMERANGBROTHER_BBOX_HEIGHT / 2;
        right = left + BOOMERANGBROTHER_BBOX_WIDTH;
        bottom = top + BOOMERANGBROTHER_BBOX_HEIGHT;
    }
}

void CBoomerangBrother::OnDefeated(int score)
{
    pointValue = score;
    isPointVisible = true;
    pointY = y - 10;
    pointStartTime = GetTickCount64();
}

void CBoomerangBrother::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void CBoomerangBrother::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!e->obj->IsBlocking()) return;
    if (dynamic_cast<CGoomba*>(e->obj)) return;
    if (dynamic_cast<CBoomerang*>(e->obj))
    {
        DebugOut(L"[BOOMERANG] cham vao boomerang \n");
        CBoomerang* boomerang = dynamic_cast<CBoomerang*>(e->obj);
        if (boomerang->IsReturning() /*&& CanCatchBoomerang(boomerang->getX(), boomerang->getY()) *Để fix hàm này sau*/)
        {
            boomerang->Delete();
            DebugOut(L"[BOOMERANG] vua tro ve\n");
            boomerangCount--;
            isThrowing = false;
        }
    }

    if (e->ny != 0)
    {
        vy = 0;
        if (e->ny < 0 && state == BOOMERANGBROTHER_STATE_JUMPING)
        {
            SetState(BOOMERANGBROTHER_STATE_WALKING);
        }
    }
    else if (e->nx != 0)
    {
        vx = -vx;
    }
}

void CBoomerangBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    nx = checkMarioDirection();
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
            isBouncing = false;
            y = startY;
            this->isDeleted = true;
        }
    }

    if (isPointVisible)
    {
        if (pointY < y + 30)
            pointY -= 0.05f * dt;
        else
            pointY = y - 10;
        if (GetTickCount64() - pointStartTime > 500)
            isPointVisible = false;
    }

    vy += ay * dt;
    vx += ax * dt;

    if (state == BOOMERANGBROTHER_STATE_WALKING && GetTickCount64() - jump_start > BOOMERANGBROTHER_JUMP_INTERVAL)
    {
        SetState(BOOMERANGBROTHER_STATE_JUMPING);
        jump_start = GetTickCount64();
    }

    if ((state == BOOMERANGBROTHER_STATE_WALKING || state == BOOMERANGBROTHER_STATE_JUMPING) &&
        !isThrowing && boomerangCount < 2 &&
        GetTickCount64() - throw_start > BOOMERANGBROTHER_THROW_INTERVAL)
    {
        SetState(BOOMERANGBROTHER_STATE_THROW);
        throw_start = GetTickCount64();
        isThrowing = true;
        hasThrownBoomerang = false; 
    }

    if (state == BOOMERANGBROTHER_STATE_THROW && !hasThrownBoomerang &&
        GetTickCount64() - throw_start > 500 && boomerangCount < 2)
    {
        ThrowBoomerang();    
        hasThrownBoomerang = true;     
    }


    if (state == BOOMERANGBROTHER_STATE_THROW && GetTickCount64() - throw_start > 550) 
    {
        SetState(BOOMERANGBROTHER_STATE_WALKING);
    }

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
    if (this->x > startX + maxDistanceX)
    {
        this->x = startX + maxDistanceX;
        vx = -BOOMERANGBROTHER_WALKING_SPEED;
    }
    else if (this->x < startX - maxDistanceX)
    {
        this->x = startX - maxDistanceX;
        vx = BOOMERANGBROTHER_WALKING_SPEED;
    }
    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBoomerangBrother::Render()
{
    if (!isActive) return;

    int aniId = (nx > 0) ? ID_ANI_BOOMERANGBROTHER_WALKINGRIGHT : ID_ANI_BOOMERANGBROTHER_WALKINGLEFT;
    if (state == BOOMERANGBROTHER_STATE_DIE)
    {
        aniId = ID_ANI_BOOMERANGBROTHER_DIE;
    }
    else if (state == BOOMERANGBROTHER_STATE_SUPER_DIE)
    {   
        aniId = ID_ANI_BOOMERANGBROTHER_SUPER_DIE;
    }
    else if (state == BOOMERANGBROTHER_STATE_JUMPING)
    {
        aniId = (nx > 0) ? ID_ANI_BOOMERANGBROTHER_JUMPINGRIGHT : ID_ANI_BOOMERANGBROTHER_JUMPINGLEFT;
    }
    else if (state == BOOMERANGBROTHER_STATE_THROW)
    {
        aniId = (nx > 0) ? ID_ANI_BOOMERANGBROTHER_THROWRIGHT : ID_ANI_BOOMERANGBROTHER_THROWLEFT;
    }

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
    if (isPointVisible)
    {
        int pointAni = (pointValue == 1000) ? ID_ANI_POINT_1000 : ID_ANI_POINT_100;
        CAnimations::GetInstance()->Get(pointAni)->Render(x, pointY);
    }
}

void CBoomerangBrother::SetState(int state)
{
    CGameObject::SetState(state);
    if (state != BOOMERANGBROTHER_STATE_THROW)
        isThrowing = false;
    switch (state)
    {
    case BOOMERANGBROTHER_STATE_DIE:
        die_start = GetTickCount64();
        vx = 0;
        vy = 0.4f;
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
        vx = (vx > 0) ? BOOMERANGBROTHER_WALKING_SPEED : -BOOMERANGBROTHER_WALKING_SPEED;
        ay = BOOMERANGBROTHER_GRAVITY;
        break;

    case BOOMERANGBROTHER_STATE_JUMPING:
        vy = -BOOMERANGBROTHER_JUMP_SPEED_Y;
        ay = BOOMERANGBROTHER_GRAVITY;
        break;

    case BOOMERANGBROTHER_STATE_THROW:
        vx = 0;
        y += (BOOMERANGBROTHER_BBOX_HEIGHT - BOOMERANGBROTHER_BBOX_HEIGHT_THROW)/2;
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

void CBoomerangBrother::ThrowBoomerang()
{
    CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
    float boomerang_vx = (nx > 0) ? 0.1f : -0.1f; 
    CBoomerang* boomerang = new CBoomerang(x, y - 8, boomerang_vx,this);
    scene->AddObject(boomerang);
    boomerangCount++;
    //SetState(BOOMERANGBROTHER_STATE_THROW);
}

//Hàm này đang lỗi nên tạm cho va chạm là nhặt được
//bool CBoomerangBrother::CanCatchBoomerang(float boomerang_x, float boomerang_y)
//{
//    if (state == BOOMERANGBROTHER_STATE_DIE || state == BOOMERANGBROTHER_STATE_SUPER_DIE)
//    {
//        return false;
//    }
//
//    float left = x - BOOMERANGBROTHER_BBOX_WIDTH;
//    float top = y - BOOMERANGBROTHER_BBOX_HEIGHT;
//    float right = left + BOOMERANGBROTHER_BBOX_WIDTH;
//    float bottom = top + BOOMERANGBROTHER_BBOX_HEIGHT;
//
//    bool canCatch = (boomerang_x >= left && boomerang_x <= right && boomerang_y >= top && boomerang_y <= bottom);
//    DebugOut(L"[BOOMERANGBROTHER] Can catch boomerang: %d, boomerang_x=%.2f, boomerang_y=%.2f, left=%.2f, right=%.2f, top=%.2f, bottom=%.2f\n",
//        canCatch, boomerang_x, boomerang_y, left, right, top, bottom);
//    return canCatch;
//}

int CBoomerangBrother::checkMarioDirection()
{
    CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
    CMario* mario = (CMario*)scene->GetPlayer();
    if (mario)
    {
        return (this->x < mario->getX() ? 1 : -1);
    }
    return 0;
}