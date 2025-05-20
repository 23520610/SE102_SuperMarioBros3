	#include "SampleKeyEventHandler.h"

	#include "debug.h"
	#include "Game.h"

	#include "Mario.h"
	#include "PlayScene.h"

	void CSampleKeyHandler::OnKeyDown(int KeyCode)
	{
		//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
		CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 

		switch (KeyCode)
		{
		case DIK_A:
			if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
			{
				mario->SetState(MARIO_STATE_ATTACKING);
				DebugOut(L"[MARIO] ATTACK\n");
			}
			
			break;
		case DIK_DOWN:
			if(!mario->isTravelingNow()) mario->SetState(MARIO_STATE_SIT);
			break;
		case DIK_S:
			if (mario->GetLevel() == MARIO_LEVEL_RACCOON && !mario->isOnThePlatForm())
			{
				mario->SetState(mario->GetFacingDirection() > 0 ? MARIO_STATE_GLIDING_RIGHT : MARIO_STATE_GLIDING_LEFT);
				DebugOut(L"[MARIO] GLIDING\n");
			}
			else if (mario->GetLevel() == MARIO_LEVEL_RACCOON && mario->GetPower() >= MARIO_MAX_POWER)
			{
				mario->SetState(mario->GetFacingDirection() > 0 ? MARIO_STATE_FLYING_RIGHT : MARIO_STATE_FLYING_LEFT);
				DebugOut(L"[MARIO] FLYING\n");
			}
			else
			{
				mario->SetState(MARIO_STATE_JUMP);
			}
			break;
		case DIK_1:
			mario->SetLevel(MARIO_LEVEL_SMALL);
			break;
		case DIK_2:
			mario->SetLevel(MARIO_LEVEL_BIG);
			break;
		case DIK_3:
			mario->SetLevel(MARIO_LEVEL_RACCOON);
			break;
		case DIK_4:
			mario->setPosition(2263, 95);
			break;
		case DIK_0:
			mario->SetState(MARIO_STATE_DIE);
			break;
		case DIK_R: // reset
			//Reload();
			break;
		}
	}

	void CSampleKeyHandler::OnKeyUp(int KeyCode)
	{
		//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		switch (KeyCode)
		{
		case DIK_S:
			if (mario->isGlidingNow())
			{
				mario->SetState(MARIO_STATE_IDLE);
			}
			else
			{
				mario->SetState(MARIO_STATE_RELEASE_JUMP);
			}
			break;
		case DIK_DOWN:

			if(!mario->isTravelingNow())mario->SetState(MARIO_STATE_SIT_RELEASE);
			break;
		}
	}

	void CSampleKeyHandler::KeyState(BYTE *states)
	{
		LPGAME game = CGame::GetInstance();
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

		bool isPressingRight = game->IsKeyDown(DIK_RIGHT);
		bool isPressingLeft = game->IsKeyDown(DIK_LEFT);
		bool isPressingA = game->IsKeyDown(DIK_A);
		bool isPressingS = game->IsKeyDown(DIK_S);

		bool isOnGround = mario->isOnThePlatForm(); 
		int level = mario->GetLevel();
		float power = mario->GetPower();
		if (isPressingRight && !mario->IsAttacking())
		{
			if (isPressingS && !isOnGround && level == MARIO_LEVEL_RACCOON)
			{
				if (power >= MARIO_MAX_POWER)
					mario->SetState(MARIO_STATE_FLYING_RIGHT);
				else
					mario->SetState(MARIO_STATE_GLIDING_RIGHT);
			}
			else
			{
				if (isPressingA )
					mario->SetState(MARIO_STATE_RUNNING_RIGHT);
				else 
					mario->SetState(MARIO_STATE_WALKING_RIGHT);
			}
		}
		else if (isPressingLeft && !mario->IsAttacking())
		{
			if (isPressingS && !isOnGround && level == MARIO_LEVEL_RACCOON)
			{
				if (power >= MARIO_MAX_POWER)
					mario->SetState(MARIO_STATE_FLYING_LEFT);
				else
					mario->SetState(MARIO_STATE_GLIDING_LEFT);
			}
			else
			{
				if (isPressingA&&!mario->IsAttacking())
					mario->SetState(MARIO_STATE_RUNNING_LEFT);
				else
					mario->SetState(MARIO_STATE_WALKING_LEFT);
			}
		}
		else
		{
			///CỰC KỲ QUAN TRỌNG 
			if (!mario->IsAttacking() && mario->GetState() != MARIO_STATE_KICK&& !mario->isTravelingNow())
				mario->SetState(MARIO_STATE_IDLE);
		}

	}