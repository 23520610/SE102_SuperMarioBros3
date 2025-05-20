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

	if (ani)
	{
		ani->Render(x, y);
	}
	else
	{
		DebugOut(L"[HUD] Animation ID %d not found!\n", ID_ANI_FRAME_HUD);
	}

	int score = player->GetScore();
	RenderNumber(score,7, x - 95, y - 3);

	// Coin
	int coin = player->GetCoin();
	RenderNumber(coin,1, x - 12, y - 13);
	//DebugOut(L"[HUD - COIN] Render at x = %f, y = %f\n", x - 12, y - 13);

	// Lives
	int lives = player->GetLives();
	RenderNumber(lives,1, x - 110, y - 3);

	// World
	int world = player->GetWorld();
	RenderNumber(world,1, x - 110, y - 13); 

	// Time: tính thời gian trôi qua
	DWORD now = GetTickCount();
	int seconds = (now - startTime) / 1000;
	int timeLeft = max(0, 300 - seconds); 
	RenderNumber(timeLeft,3, x - 12, y - 3);

	int power = player->GetPowerLevel();
	RenderBlackPowerBar(x - 96, y - 14);
	RenderPowerBar(power, x - 96, y - 14);

	CSprites::GetInstance()->Get(ID_SPRITE_LIVES)->Draw(x - 142, y - 3);

	RenderText("WORLD", x - 154, y - 13);

	CSprites::GetInstance()->Get(ID_SPRITE_CLOCK)->Draw(x - 22, y - 3);
	CSprites::GetInstance()->Get(ID_SPRITE_DOLLAR)->Draw(x - 22, y - 13);
	CSprites::GetInstance()->Get(ID_SPRITE_X)->Draw(x - 126, y - 2);

	if (player)
	{
		vector<int> cards = player->GetCollectedItems();
		RenderItemCards(cards);
	}
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
	float startX = this->x + 36;
	float y = this->y - 8;

	for (size_t i = 0; i < itemTypes.size(); i++)
	{
		const char* itemName = "UNKNOWN";
		int aniId = -1;
		switch (itemTypes[i])
		{
		case ITEMCARD_STATE_MUSHROOM:
			aniId = ID_ANI_ITEMCARD_MUSHROOM;
			itemName = "MUSHROOM";
			break;
		case ITEMCARD_STATE_STAR:
			aniId = ID_ANI_ITEMCARD_STAR;
			itemName = "STAR";
			break;
		case ITEMCARD_STATE_FLOWER:
			aniId = ID_ANI_ITEMCARD_FLOWER;
			itemName = "FLOWER";
			break;
		}
		DebugOut(L"[HUD] Rendering ItemCard #%d: %S\n", (int)i, itemName); 
		if (aniId != -1)
			CAnimations::GetInstance()->Get(aniId)->Render(startX + i * 20, y);
	}
}
