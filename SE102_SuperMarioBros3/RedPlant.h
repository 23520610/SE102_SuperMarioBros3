#pragma once
#include "PlantEnemy.h"

#define REDPLANT_BBOX_WIDTH 16
#define REDPLANT_BBOX_HEIGHT 32
#define ID_ANI_REDPLANT_RISED_LEFT 13000
#define ID_ANI_REDPLANT_RISED_RIGHT 13001
#define ID_ANI_REDPLANT_LEFT_FACEUP 13002
#define ID_ANI_REDPLANT_RIGHT_FACEUP 13003
#define ID_ANI_REDPLANT_DIVING_LEFT 13004
#define ID_ANI_REDPLANT_DIVING_RIGHT 13005
#define ID_ANI_REDPLANT_DIVING_LEFT_FACEUP 13006
#define ID_ANI_REDPLANT_DIVING_RIGHT_FACEUP 13007


#define REDPLANT_RISEHEIGHT 32.0f
class CRedPlant : public CPlantEnemy
{
public:
	CRedPlant(float x, float y) : CPlantEnemy(x, y,REDPLANT_RISEHEIGHT) {}
	void Render() override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};

