#include <iostream>
#include <fstream>
#include "AssetIDs.h"
#include "Effect.h"
#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "Pipe.h"
#include "RedPlant.h"
#include "PiranhaPlant.h"
#include "GreenPlant.h"
#include "FireBall.h"
#include "SampleKeyEventHandler.h"
#include "Mushroom.h"
#include "QuestionBrick.h"
#include "Koopas.h"
#include "ParaGoomba.h"
#include "Map.h"
#include "FixedBrick.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: 
	{
		float spawnX = stof(tokens[3]); 
		obj = new CGoomba(x, y, spawnX); 
	}
	break;
	
	case OBJECT_TYPE_PARA_GOOMBA:
	{
		float spawnX = stof(tokens[3]);
		obj = new CParaGoomba(x, y, spawnX);
		break;
	}
	case OBJECT_TYPE_KOOPAS:
	{
		float spawnX = stof(tokens[3]);
		obj = new CKoopas(x, y, spawnX);
	}
	break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(x,y); break;
	case OBJECT_TYPE_FIXEDBRICK:
	{
		int type = stoi(tokens[3]);
		obj = new CFixedBrick(x, y, type); 
		break;
	}
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_PIPE:
	{
		float x = stof(tokens[1]);
		float y = stof(tokens[2]);
		int pipeType = stoi(tokens[3]);
		obj = new CPipe(x, y, pipeType);
		break;
	}
	case OBJECT_TYPE_REDPLANT:
	{
		float x = stof(tokens[1]);
		float y = stof(tokens[2]);
		obj = new CRedPlant(x, y);
		break;
	}
	case OBJECT_TYPE_PIRANHAPLANT:
	{
		float x = stof(tokens[1]);
		float y = stof(tokens[2]);
		obj = new CPiranhaPlant(x, y);
		break;
	}
	case OBJECT_TYPE_GREENPLANT:
	{
		float x = stof(tokens[1]);
		float y = stof(tokens[2]);
		obj = new CGreenPlant(x, y);
		break;
	}

	case OBJECT_TYPE_PLATFORM:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());
		bool isGround = atoi(tokens[9].c_str()) == 1 ? true : false;
		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end, isGround
		);

		break;
	}
	case OBJECT_TYPE_MAP:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());
		obj = new CMap(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}
	case OBJECT_TYPE_QUESTIONBRICK:
	{
		int QuesBrick_type = atoi(tokens[3].c_str());
		obj = new CQuestionBrick(x, y, 
								QuesBrick_type);
	}
		break;
	case OBJECT_TYPE_FIREBALL: obj = new CFireBall(x, y); break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;

	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < effects.size(); )
	{
		CEffect* effect = effects[i];
		effect->Update(dt, &objects);

		if (effect->IsExpired())
		{
			delete effect;
			effects.erase(effects.begin() + i); 
		}
		else
		{
			i++; 
		}
	}

	if (isGamePaused)
	{
		if (player) 
			player->Update(dt, nullptr); 
		return;
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;
	// Update camera to follow mario

	float px, py;
	player->GetPosition(px, py);

	CGame* game = CGame::GetInstance();

	float cam_x = px - game->GetBackBufferWidth() / 2;
	if (cam_x < 0) cam_x = 0;

	float mapWidth = 2805.0f; 
	float screenWidth = game->GetBackBufferWidth();
	if (cam_x > mapWidth - screenWidth)
		cam_x = mapWidth - screenWidth;

	// --- CAMERA Y ---
	bool inSafeZone = (px > 1967 && px < 2478 && py > 440 && py < 624);
	if (inSafeZone)
		cam_y = 424;
	else
	{
		float target_cam_y = py - 30;
		if (target_cam_y < 0) target_cam_y = 0;

		float diff = abs(target_cam_y - cam_y);
		if (diff > 1.0f)
		{
			float smooth = 0.05f;
			cam_y += (target_cam_y - cam_y) * smooth;
			if (cam_y > 240) cam_y = 240;
		}
	}
	


	game->SetCamPos(cam_x, cam_y);

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	CMario* mario = (CMario*)player;

	if (mario && mario->isTravelingNow())
		mario->Render();

	//Sửa lỗi render cái vật thể đè lên mario
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i] != player)
			objects[i]->Render();
	}
	for (auto effect : effects)
	{
		effect->Render();
	}
	//Nếu Mario KHÔNG traveling thì render SAU cùng
	if (mario && !mario->isTravelingNow())
		mario->Render();
}



/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}

//moi them de xu ly coin 20.4
void CPlayScene::AddObject(LPGAMEOBJECT obj)
{
	objects.push_back(obj);
}

void CPlayScene::AddEffect(CEffect* effect)
{
	effects.push_back(effect);
}