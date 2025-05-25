#include "Hud.h"
#include "Game.h"
#include "debug.h"
#include "ItemCard.h"

void CHud::Render()
{
	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);
	this->x = cam_x + 170;
	this->y = cam_y + 220;


	CAnimations* animations = CAnimations::GetInstance();
	LPANIMATION ani = animations->Get(ID_ANI_FRAME_HUD);

	CSprites::GetInstance()->Get(ID_ANI_FRAME_HUD)->Draw(x, y);

	// Lives
	int lives = player->GetLives();
	
	if (lives >= 0)
	{
		int score = player->GetScore();
		RenderNumber(score, 7, x - 100, y - 3);

		// Coin
		int coin = player->GetCoin();
		RenderNumber(coin, 1, x - 17, y - 13);
		//DebugOut(L"[HUD - COIN] Render at x = %f, y = %f\n", x - 12, y - 13);

		RenderNumber(lives, 1, x - 115, y - 3);

		// World
		int world = player->GetWorld();
		RenderNumber(world, 1, x - 115, y - 13);

		// Time: tính thời gian trôi qua
		DWORD now = GetTickCount();
		int seconds = (now - startTime) / 1000;
		int timeLeft = max(0, 300 - seconds);
		RenderNumber(timeLeft, 3, x - 17, y - 3);

		int power = player->GetPowerLevel();
		RenderBlackPowerBar(x - 101, y - 14);
		RenderPowerBar(power, x - 101, y - 14);

		CSprites::GetInstance()->Get(ID_SPRITE_LIVES)->Draw(x - 147, y - 3);

		RenderText("WORLD", x - 159, y - 13);

		CSprites::GetInstance()->Get(ID_SPRITE_CLOCK)->Draw(x - 27, y - 3);
		CSprites::GetInstance()->Get(ID_SPRITE_DOLLAR)->Draw(x - 27, y - 13);
		CSprites::GetInstance()->Get(ID_SPRITE_X)->Draw(x - 131, y - 2);

		float px, py;
		player->GetPosition(px, py);

		static ULONGLONG endingSpriteStart = 0;
		if (px >= 2815)
		{
			isCompleteScene = true;
			if (endingSpriteStart == 0)
				endingSpriteStart = GetTickCount64();

			if (GetTickCount64() - endingSpriteStart < 3000)
			{
				CSprites::GetInstance()->Get(ID_SPRITE_ENDING)->Draw(x, y - 180);
				player->SetPosition(2815, py);
			}
		}


		vector<int> cards = player->GetCollectedItems();
		RenderItemCards(cards);

	}
	else if (lives < 0) RenderText("GAME PVER", x - 110, y - 7);
}

void CHud::Update(DWORD dt) 
{
	//CGameObject::Update(dt);

}

void CHud::RenderNumber(int number, int numDigits, float x, float y)
{
	if (number < 0) number = 0;
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

void CHud::RenderBlackPowerBar(float x, float y)
{
	LPSPRITE arrow = CSprites::GetInstance()->Get(ID_SPRITE_BLACKARROW);
	if (!arrow) return;

	for (int i = 0; i < 6; i++)
	{
			arrow->Draw(x + i * 8, y);
	}
	LPSPRITE p = CSprites::GetInstance()->Get(ID_SPRITE_BLACKPOWER_P);
	if (p) p->Draw(x + 6 * 8 + 4, y);
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

		case 'G': spriteId = ID_SPRITE_LETTER_G; break;
		case 'A': spriteId = ID_SPRITE_LETTER_A; break;
		case 'M': spriteId = ID_SPRITE_LETTER_M; break;
		case 'E': spriteId = ID_SPRITE_LETTER_E; break;
		case ' ': spriteId = ID_SPRITE_LETTER_X; break;
		case 'V': spriteId = ID_SPRITE_LETTER_V; break;
		case 'P': spriteId = ID_SPRITE_LETTER_O2; break;
		//case 'P': spriteId = ID_SPRITE_LETTER_O2; break;
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

void CHud::RenderItemCards(vector<int> itemTypes)
{
	float startX = this->x + 32;
	float y = this->y - 8;
	string itemname = "";
	for (size_t i = 0; i < itemTypes.size(); i++)
	{
		int aniId = -1;
		switch (itemTypes[i])
		{
		case ITEMCARD_STATE_MUSHROOM:
		{
			itemname = "mushroom";
			aniId = ID_SPRITE_MUSHROOM;
			break;
		}
		case ITEMCARD_STATE_STAR:
		{
			itemname = "star";
			aniId = ID_SPRITE_STAR;
			break;
		}

		case ITEMCARD_STATE_FLOWER:
		{
			itemname = "flower";
			aniId = ID_SPRITE_FLOWER;
			break;
		}
		}
		//DebugOut(L"[HUD] Rendering ItemCard #%d: %S\n", (int)i, itemname); 
		if (aniId != -1)
		{
			CSprites::GetInstance()->Get(aniId)->Draw(startX + i * 28, y);

			//DebugOut(L"[ITEM] Rendering startX = %f\n", startX + i * 27);
		}
		if (isCompleteScene == true) CSprites::GetInstance()->Get(aniId)->Draw(x + 62, y - 157);
	}
}
