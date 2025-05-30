#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "QuestionBrick.h"
#include "Effect.h"
#include "FixedBrick.h"
#include "Hud.h"
#include <vector>
using namespace std;

#define MAX_ITEM_CARDS 3

class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;					

	CHud* hud;

	vector<LPGAMEOBJECT> objects;

	bool isGamePaused = false;
	bool isPaused = false; 
	vector<int> hudItemCards;

	vector<CEffect*> effects;

	bool hasCameraStoppedScrolling = false;
	bool justStoppedScrolling = false;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();
	void AddObject(LPGAMEOBJECT obj); //moi them de xu ly coin 20.4
	float cam_x = 0; // mặc định
	float cam_y = 240.0f;// default camera y position để không bị cà giật=)))
	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);

	void SetGamePaused(bool isPaused) { isGamePaused = isPaused; }

	bool GetGamePaused() { return isGamePaused; }

	void AddEffect(CEffect* effect);

	vector<LPGAMEOBJECT>& GetObjects() { return objects; }

	CHud* GetHud() { return hud; }

	void AddItemCardToHUD(int type);

	vector<int> GetHUDItemCards() { return hudItemCards; }

	void AddObjectBefore(LPGAMEOBJECT obj, LPGAMEOBJECT refObj);

	//std::string GetSceneFilePath() const { return sceneFilePath; }

	void ResetCamera();

	void SetHasCameraStoppedScrolling(bool check) { hasCameraStoppedScrolling = check; }
	void SetJustStoppedScrolling(bool check) { justStoppedScrolling = check; }
	void SetGameTimePaused(bool value) { isPaused = value; }
	bool IsGamePaused() { return isPaused; }
};

typedef CPlayScene* LPPLAYSCENE;
