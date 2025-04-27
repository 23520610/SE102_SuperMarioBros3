#pragma once
#include "PlantEnemy.h"

#define REDPLANT_BBOX_WIDTH 16
#define REDPLANT_BBOX_HEIGHT 31
#define ID_ANI_REDPLANT 13000
#define REDPLANT_RISEHEIGHT 32.0f
class CRedPlant : public CPlantEnemy
{
public:
	CRedPlant(float x, float y) : CPlantEnemy(x, y,REDPLANT_RISEHEIGHT) {}
	void Render() override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};

