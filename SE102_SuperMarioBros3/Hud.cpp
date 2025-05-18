#include "Hud.h"
#include "Game.h"
#include "debug.h"

void CHud::Render()
{
	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);
	this->x = cam_x + 170;
	this->y = cam_y + 220;
	DebugOut(L"[HUD] Hud y = %f\n", this->y);

	CAnimations* animations = CAnimations::GetInstance();
	LPANIMATION ani = animations->Get(ID_ANI_FRAME_HUD);

	if (ani)
	{
		ani->Render(x, y);
	}
	else
	{
		DebugOut(L"[HUD] Animation ID %d not found!\n", ID_ANI_FRAME_HUD);
	}

	int score = player->GetScore();
	RenderNumber(score,7, x - 92, y - 10);

	// Coin
	int coin = player->GetCoin();
	RenderNumber(coin,1, x - 11, y - 19);

	// Lives
	int lives = player->GetLives();
	RenderNumber(lives,1, x - 107, y - 10);

	// World
	int world = player->GetWorld();
	RenderNumber(world,1, x - 107, y - 18); 

	// Time: tính thời gian trôi qua
	DWORD now = GetTickCount();
	int seconds = (now - startTime) / 1000;
	int timeLeft = max(0, 300 - seconds); 
	RenderNumber(timeLeft,3, x - 19, y - 10);

	int power = player->GetPowerLevel();
	RenderPowerBar(power, x - 92, y - 19);

	CSprites::GetInstance()->Get(ID_SPRITE_LIVES)->Draw(x - 136, y - 10);
;	//RenderText("WORLD", x - 140, y - 19);
}

void CHud::Update(DWORD dt) 
{
	//CGameObject::Update(dt);
}

void CHud::RenderNumber(int number, int numDigits, float x, float y)
{
	string numStr = to_string(number);
	while (numStr.length() < numDigits)
		numStr = "0" + numStr;

	for (char c : numStr) 
	{
		int digit = c - '0';
		CSprites::GetInstance()->Get(ID_SPRITE_DIGIT_0 + digit)->Draw(x, y);
		x += 8; 
	}
}

void CHud::RenderPowerBar(int level, float x, float y)
{
	LPSPRITE arrow = CSprites::GetInstance()->Get(ID_SPRITE_ARROW);
	if (!arrow) return;

	for (int i = 0; i < 6; i++)
	{
		if (i < level)
		{
			arrow->Draw(x + i * 8, y); 
		}
	}
	if (level == 6)
	{
		LPSPRITE p = CSprites::GetInstance()->Get(ID_SPRITE_POWER_P);
		if (p) p->Draw(x + 6 * 8 + 4, y); 
	}
}

void CHud::RenderText(string text, float x, float y)
{
	for (char c : text)
	{
		int spriteId = -1;

		switch (c)
		{
		case 'W': spriteId = ID_SPRITE_LETTER_W; break;
		case 'O': spriteId = ID_SPRITE_LETTER_O; break;
		case 'R': spriteId = ID_SPRITE_LETTER_R; break;
		case 'L': spriteId = ID_SPRITE_LETTER_L; break;
		case 'D': spriteId = ID_SPRITE_LETTER_D; break;
		default: continue; 
		}

		if (spriteId != -1)
		{
			LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
			if (sprite)
				sprite->Draw(x, y);
		}

		x += 8; 
	}
}
