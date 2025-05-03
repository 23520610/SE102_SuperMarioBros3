#include "PlantEnemy.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"
#include "FireBall.h"
#define PLANT_WAIT_TIME 2000 // Thời gian chờ giữa các lần lên xuống
#define PLANT_ATTACK_TIME 1000 // Thời gian đứng lại để tấn công
#define PLANT_AIMING_TIME 500 // Thời gian ngắm bắn
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

    CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
    CMario* mario = (CMario*)scene->GetPlayer();

    float mx, my;
    mario->GetPosition(mx, my);

    float distance = abs(mx - x);
    
    if (state == PLANT_STATE_IDLE && distance < PLANT_PLAYER_SAFE_DISTANCE_X)
    {
        return;
    }
    // Mario quá gần thì cây không lên

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
        if (!hasFired && now - state_timer >= PLANT_AIMING_TIME)
        {
            ShootFireball();
            hasFired = true;
        }

        if (now - state_timer > PLANT_ATTACK_TIME)
        {
            SetState(PLANT_STATE_HIDE);
            hasFired = false;
        }
        break;


    case PLANT_STATE_HIDE:
        if (y >= startY)
        {
            y = startY;
            SetState(PLANT_STATE_IDLE);
			hasFired = false;
        }
        break;
    }
}

void CPlantEnemy::OnNoCollision(DWORD dt)
{
    y += vy * dt;
}


void CPlantEnemy::ShootFireball()
{
    CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
    CMario* mario = (CMario*)scene->GetPlayer();

    float mx, my;
    mario->GetPosition(mx, my);

    float dx = mx - x;
    float dy = my - y;

	float length = sqrt(dx * dx + dy * dy);// tính khoảng cách giữa Mario và cây
    if (length > 175.0f) {
        return;
    }
    bool facingRight = dx > 0;
    if (dx == 0 && dy == 0) return;

	//Tính góc giữa Mario và cây
    float angle = atan2f(dy, dx); // rad
    float angleDeg = angle * 180.0f / 3.14159265f;

    float downAngle = angleDeg - 90.0f;
    if (downAngle < -180.0f) downAngle += 360.0f;
    if (downAngle > 180.0f)  downAngle -= 360.0f;


    int dirIndex = 0; // mặc định chéo lên cao

    if (dy >= 0) // Mario ở dưới hoặc ngang
    {
        if (dx > 0) // Mario bên phải
        {
            if (downAngle < -45)
                dirIndex = 1; // chéo nhẹ xuống
            else
                dirIndex = 2; // chéo mạnh xuống
        }
        else // Mario bên trái
        {
            if (downAngle > 45)
                dirIndex = 1; // chéo nhẹ xuống
            else
                dirIndex = 2; // chéo mạnh xuống
        }
    }
    else //Mario ở trên
    {
        if (dx > 0) // Mario bên phải
        {

            if (downAngle > -110)
				dirIndex = 3; // chéo nhẹ lên
        }
        else // Mario bên trái
        {
            if (downAngle < 110)
                dirIndex = 3; // chéo nhẹ lên trái 
        }
    }

    //góc bắn theo độ (tính từ trục ngang)
    float degreeAngles[4] = { -45, 20, 45 , -18}; // chéo lên, thấp xa, thấp gần, chéo lên thấp

    float angle_shoot = degreeAngles[dirIndex];
    float rad = angle_shoot * 3.14159265f / 180.0f; // đổi sang radian
    float speed = 0.1f;

    float vx = cos(rad) * speed;
    float vy = sin(rad) * speed;

    if (!facingRight) vx = -vx; 

    CFireBall* fireball = new CFireBall(x, y - 5);
    fireball->SetSpeed(vx, vy);

    scene->AddObject(fireball);
}





