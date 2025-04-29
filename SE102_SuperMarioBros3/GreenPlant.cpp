#include "GreenPlant.h"
#include "Animations.h"
#include "PlayScene.h"

void CGreenPlant::Render()
{
    int aniId = -1;

    CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
    CMario* mario = (CMario*)scene->GetPlayer();

    float mx, my;
    mario->GetPosition(mx, my);

    bool faceUp = (my < y);
    bool isRight = (mx > x);

    if (state == PLANT_STATE_RISE || state == PLANT_STATE_HIDE)
    {
        // Lúc RISE hoặc HIDE thì lấy Animation ngáp ngáp
        if (faceUp)
            aniId = isRight ? ID_ANI_GREENPLANT_DIVING_RIGHT_FACEUP : ID_ANI_GREENPLANT_DIVING_LEFT_FACEUP;
        else
            aniId = isRight ? ID_ANI_GREENPLANT_DIVING_RIGHT : ID_ANI_GREENPLANT_DIVING_LEFT;
    }
    else // IDLE hoặc ATTACK
    {
        // Đứng im
        if (faceUp)
            aniId = isRight ? ID_ANI_GREENPLANT_RIGHT_FACEUP : ID_ANI_GREENPLANT_LEFT_FACEUP;
        else
            aniId = isRight ? ID_ANI_GREENPLANT_RISED_RIGHT : ID_ANI_GREENPLANT_RISED_LEFT;
    }

    if (aniId != -1)
        CAnimations::GetInstance()->Get(aniId)->Render(x, y);

    //RenderBoundingBox();
}

void CGreenPlant::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - GREENPLANT_BBOX_WIDTH / 2;
	t = y - GREENPLANT_BBOX_HEIGHT / 2;
	r = l + GREENPLANT_BBOX_WIDTH;
	b = t + GREENPLANT_BBOX_HEIGHT;
}
