#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Mario.h"
#include <string>

#define ID_SPRITE_ENDING 500030
#define ID_SPRITE_GAMEOVER 500040

#define ID_SPRITE_LETTER_W 600000
#define ID_SPRITE_LETTER_O 600001
#define ID_SPRITE_LETTER_R 600002
#define ID_SPRITE_LETTER_L 600003
#define ID_SPRITE_LETTER_D 600004

#define ID_SPRITE_LETTER_G 500050
#define ID_SPRITE_LETTER_A 500051
#define ID_SPRITE_LETTER_M 500052
#define ID_SPRITE_LETTER_E 500053
#define ID_SPRITE_LETTER_X 500054
#define ID_SPRITE_LETTER_O2 500055
#define ID_SPRITE_LETTER_V 500056
#define ID_SPRITE_LETTER_E 500057
#define ID_SPRITE_LETTER_R 500058


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
#define ID_SPRITE_BLACKARROW 500022
#define ID_SPRITE_BLACKPOWER_P 500023

#define ID_SPRITE_DOLLAR 500016
#define ID_SPRITE_CLOCK 500015
#define ID_SPRITE_PAUSE 140116
#define ID_SPRITE_LIVES 500014
#define ID_SPRITE_X 500017

class CHud : public CGameObject {
    CMario* player;
    DWORD startTime;
    int timeLeft;          
    DWORD pauseStartTime;
public:
    CHud(CMario* player) {
        this->player = player;
        this->timeLeft = 300;                  
        this->pauseStartTime = 0; 
        ResetTimer();
    }
    void GetBoundingBox(float& l, float& t, float& r, float& b) override {}
    void Render();
    void Update(DWORD dt);
    void RenderNumber(int number, int numDigits, float x, float y);
    void RenderPowerBar(int powerLevel, float x, float y);
    void RenderBlackPowerBar(float x, float y);
    void RenderText(string text, float x, float y);
    void RenderItemCards(vector<int> itemTypes);
	void GetPosition(float& x, float& y) {
		x = this->x;
		y = this->y;
	}
    void ResetTimer(int totalTime = 300); 
    void PauseTimer();
    void ResumeTimer();
    int GetTimeLeft() const { return timeLeft; }
};