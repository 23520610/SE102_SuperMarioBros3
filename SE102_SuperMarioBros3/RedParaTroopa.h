#pragma once
#include "ParaTroopa.h"

#define RED_PARATROOPA_SPEED_Y 0.05f
#define RED_PARATROOPA_INTERVAL 1700
#define ID_ANI_RED_PARA_TROOPA_FLY 180998   

class CRedParaTroopa : public CParaTroopa
{
    ULONGLONG fly_timer = 0;
    bool flyUp = true;

public:
    CRedParaTroopa(float x, float y, float spawnX);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void Render() override;
    void loseWings()
    {
        hasWings = false;
        CKoopas* redkoopas = new CKoopas(x, y, 0, 2);
        LPGAMEOBJECT self = this;
        CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
        scene->AddObject(redkoopas);
        this->Delete();
        vy = 0;
        return;
    } 
    void getHited()
    {
        hasWings = false;
        CKoopas* koopas = new CKoopas(x, y, 0, 2);
        LPGAMEOBJECT self = this;
        CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
        scene->AddObject(koopas);
        this->Delete();
        vy = 0;
        koopas->SetState(KOOPAS_STATE_DIE);
        return;
    }
};
