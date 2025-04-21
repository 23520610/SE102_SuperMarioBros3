#pragma once
#include "PlantEnemy.h"

#define GREENPLANT_BBOX_WIDTH 16
#define GREENPLANT_BBOX_HEIGHT 31
#define ID_ANI_GREENPLANT 15000
#define GREENPLANT_RISEHEIGHT 32.0f
class CGreenPlant : public CPlantEnemy
{
public:
	CGreenPlant(float x, float y) : CPlantEnemy(x, y, GREENPLANT_RISEHEIGHT) {}
	void Render() override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};

