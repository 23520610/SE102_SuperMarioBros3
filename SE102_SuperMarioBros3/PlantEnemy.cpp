#include "PlantEnemy.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"

#define PLANT_WAIT_TIME 2000 // Thời gian chờ giữa các lần lên xuống
#define PLANT_ATTACK_TIME 1000 // Thời gian đứng lại để tấn công

CPlantEnemy::CPlantEnemy(float x, float y, float riseHeight) : CGameObject(x, y)
{
    startY = y;
    maxY = y - riseHeight; // Chiều cao cây chui lên
    vy = 0;
    state_timer = GetTickCount();
    SetState(PLANT_STATE_IDLE);
}

void CPlantEnemy::SetState(int state)
{
    CGameObject::SetState(state);
    switch (state)
    {
    case PLANT_STATE_IDLE:
        vy = 0;
        break;

    case PLANT_STATE_RISE:
        vy = -PLANT_SPEED_Y;
        break;

    case PLANT_STATE_HIDE:
        vy = PLANT_SPEED_Y;
        break;

    case PLANT_STATE_ATTACK:
        vy = 0;
        break;
    }
    state_timer = GetTickCount();
}

void CPlantEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

    // Kiểm tra khoảng cách với Mario
    CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
    CMario* mario = (CMario*)scene->GetPlayer();

    float mx, my;
    mario->GetPosition(mx, my);

    float distance = abs(mx - x);
    if (distance < 32.0f && state == PLANT_STATE_IDLE) return; // Mario quá gần thì cây không lên

    y += vy*dt;

    DWORD now = GetTickCount();

    switch (state)
    {
    case PLANT_STATE_IDLE:
        if (now - state_timer > PLANT_WAIT_TIME)
            SetState(PLANT_STATE_RISE);
        break;

    case PLANT_STATE_RISE:
        if (y <= maxY)
        {
            y = maxY;
            SetState(PLANT_STATE_ATTACK);
        }
        break;

    case PLANT_STATE_ATTACK:
        if (now - state_timer > PLANT_ATTACK_TIME)
            SetState(PLANT_STATE_HIDE);
        break;

    case PLANT_STATE_HIDE:
        if (y >= startY)
        {
            y = startY;
            SetState(PLANT_STATE_IDLE);
        }
        break;
    }
}

void CPlantEnemy::OnNoCollision(DWORD dt)
{
    y += vy * dt;
}



