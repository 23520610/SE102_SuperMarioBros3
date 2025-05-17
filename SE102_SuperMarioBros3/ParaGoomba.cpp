#include "ParaGoomba.h"
#include "Goomba.h"
#include "debug.h"

CParaGoomba::CParaGoomba(float x, float y, float spawnX) : CGoomba(x, y, spawnX)
{
    isActive = false;
    hasWings = true;
    isOnGround = false;
    ay = PARA_GOOMBA_GRAVITY;
    SetState(GOOMBA_STATE_WALKING);
    hopCount = 0;
    last_action_time = GetTickCount64();
}

void CParaGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGoomba::Update(dt, coObjects);

    if (hasWings && state != GOOMBA_STATE_DIE&&state !=GOOMBA_STATE_SUPER_DIE)
    {
        ULONGLONG now = GetTickCount64();
        if (isOnGround)
        {
            if (hopCount == 5)
            {
                if (now - last_action_time >= PARA_GOOMBA_WAIT_TIME)
                {
                    hopCount = 0;
                    last_action_time = now;
                    SetState(GOOMBA_STATE_WALKING);
                }
                else
                {
                    if (state != GOOMBA_STATE_WALKING)
                        SetState(GOOMBA_STATE_WALKING);
                    return;
                }
            }
            if (hopCount == 0 && now - last_action_time >= PARA_GOOMBA_HOP_INTERVAL)
            {
                vy = -PARA_GOOMBA_SMALL_HOP_SPEED;
                hopCount = 1;
                SetState(GOMBA_STATE_HOPPING);
                last_action_time = now;
                //DebugOut(L"[INFO] Start small hop, hopCount = 1\n");
                return;
            }

            // Nhảy nhỏ tiếp theo
            if (hopCount >= 1 && hopCount < NUM_SMALL_HOPS && now - last_action_time >= PARA_GOOMBA_HOP_INTERVAL)
            {
                vy = -PARA_GOOMBA_SMALL_HOP_SPEED;
                hopCount++;
                SetState(GOMBA_STATE_HOPPING);
                last_action_time = now;
                //DebugOut(L"[INFO] Continue small hop, hopCount = %d\n", hopCount);
                return;
            }

            // Nhảy lớn
            if (hopCount == NUM_SMALL_HOPS && now - last_action_time >= PARA_GOOMBA_HOP_INTERVAL)
            {
                vy = -PARA_GOOMBA_BIG_JUMP_SPEED;
                hopCount = 4;
                SetState(GOMBA_STATE_FLYING);
                last_action_time = now;
                //DebugOut(L"[INFO] Big jump, hopCount = 4\n");
                return;
            }
        }

        // Chạm đất sau nhảy lớn
        if (hopCount == 4 && isOnGround)
        {
            hopCount = 5;
            last_action_time = now;
            SetState(GOOMBA_STATE_WALKING);
            //DebugOut(L"[INFO] Landed after big jump, hopCount = 5\n");
            return;
        }
    }

    if (!hasWings && state != GOOMBA_STATE_DIE && vx == 0)
    {
        vx = -GOOMBA_WALKING_SPEED;
    }
}

void CParaGoomba::Render()
{
    int aniId = -1;

    if (state == GOOMBA_STATE_DIE)
    {
        aniId = ID_ANI_PARA_GOOMBA_DIE;
    }
	else if (state == GOOMBA_STATE_SUPER_DIE)
	{
		aniId = ID_ANI_PARA_GOOMBA_SUPER_DIE;
	}
    else if (hasWings)
    {
        if (state == GOMBA_STATE_FLYING)
        {
            aniId = ID_ANI_PARA_GOOMBA_JUMPING;
        }
        else if (state == GOMBA_STATE_HOPPING)
        {
            aniId = ID_ANI_PARA_GOOMBA_HOPING;
        }
        else
        {
            aniId = ID_ANI_PARA_GOOMBA_WALKING_WINGED;
        }
    }
    else
    {
        aniId = ID_ANI_PARA_GOOMBA_WALK;
    }

    if (aniId != -1) {
        CAnimations::GetInstance()->Get(aniId)->Render(x, y);
    }
	//RenderBoundingBox();
}

void CParaGoomba::SetState(int state)
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
        hopCount = 0;
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
        ay = PARA_GOOMBA_GRAVITY;
        break;

    case GOMBA_STATE_HOPPING:
    case GOMBA_STATE_FLYING:
        break;
    }
}

void CParaGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    if (state == GOOMBA_STATE_DIE)
    {
        left = x - PARA_GOOMBA_BBOX_WIDTH / 2;
        top = y - PARA_GOOMBA_BBOX_HEIGHT_DIE / 2;
        right = left + PARA_GOOMBA_BBOX_WIDTH;
        bottom = top + PARA_GOOMBA_BBOX_HEIGHT_DIE;
    }
    else if (hasWings)
    {
		if (state == GOMBA_STATE_FLYING)
		{
			left = x - PARA_GOOMBAWING_BBOX_WIDTH / 2;
			top = y - PARA_GOOMBA_FLYING_BBOX_HEIGHT / 2;
			right = left + PARA_GOOMBAWING_BBOX_WIDTH;
			bottom = top + PARA_GOOMBA_FLYING_BBOX_HEIGHT;
		}
		else
		{
			left = x - PARA_GOOMBAWING_BBOX_WIDTH / 2;
			top = y - PARA_GOOMBAWING_BBOX_HEIGHT / 2;
			right = left + PARA_GOOMBAWING_BBOX_WIDTH;
			bottom = top + PARA_GOOMBAWING_BBOX_HEIGHT;
		}
        
    }
    else
    {
        left = x - PARA_GOOMBA_BBOX_WIDTH / 2;
        top = y - PARA_GOOMBA_BBOX_HEIGHT / 2;
        right = left + PARA_GOOMBA_BBOX_WIDTH;
        bottom = top + PARA_GOOMBA_BBOX_HEIGHT;
    }
}

void CParaGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
    CGoomba::OnCollisionWith(e);
    //va chạm xử lý bên mario
    if (!e->obj->IsBlocking()) return;

    if (e->ny != 0)
    {
        if (!isOnGround)
        {
            isOnGround = true;
            //DebugOut(L"[INFO] ParaGoomba is on ground, hopCount: %d\n", hopCount);
        }
    }
}

void CParaGoomba::OnNoCollision(DWORD dt)
{
    CGoomba::OnNoCollision(dt);
    if (isOnGround)
    {
        isOnGround = false;
    }
}

