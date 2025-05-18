#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Mario.h"
#include <string>

#define ID_SPRITE_LETTER_W 600000
#define ID_SPRITE_LETTER_O 600001
#define ID_SPRITE_LETTER_R 600002
#define ID_SPRITE_LETTER_L 600003
#define ID_SPRITE_LETTER_D 600004


#define ID_SPRITE_LETTER_0 600100
#define ID_SPRITE_LETTER_1 600101
#define ID_SPRITE_LETTER_2 600102
#define ID_SPRITE_LETTER_3 600103
#define ID_SPRITE_LETTER_4 600104
#define ID_SPRITE_LETTER_5 600105
#define ID_SPRITE_LETTER_6 600106
#define ID_SPRITE_LETTER_7 600107
#define ID_SPRITE_LETTER_8 600108
#define ID_SPRITE_LETTER_9 600109


#define HUD_HEIGHT 28
#define ID_ANI_FRAME_HUD 510000

#define ID_SPRITE_DIGIT_0 500000
#define ID_SPRITE_DIGIT_1 500001
#define ID_SPRITE_DIGIT_2 500002
#define ID_SPRITE_DIGIT_3 500003
#define ID_SPRITE_DIGIT_4 500004
#define ID_SPRITE_DIGIT_5 500005
#define ID_SPRITE_DIGIT_6 500006
#define ID_SPRITE_DIGIT_7 500007
#define ID_SPRITE_DIGIT_8 500008
#define ID_SPRITE_DIGIT_9 500009

#define ID_SPRITE_ARROW 500020
#define ID_SPRITE_POWER_P 500021

#define ID_SPRITE_LIVES 500014

class CHud : public CGameObject {
    CMario* player;
    DWORD startTime;
public:
    CHud(CMario* player) {
        this->player = player;
        startTime = GetTickCount();
    }
    void GetBoundingBox(float& l, float& t, float& r, float& b) override {}
    void Render();
    void Update(DWORD dt);
    void RenderNumber(int number, int numDigits, float x, float y);
    void RenderPowerBar(int powerLevel, float x, float y);
    void RenderText(string text, float x, float y);
};