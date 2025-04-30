#pragma once
#include "PlantEnemy.h"

#define PIRANHAPLANT_BBOX_WIDTH 16
#define PIRANHAPLANT_BBOX_HEIGHT 24
#define ID_ANI_PIRANHAPLANT 14000
#define PIRANHAPLANT_RISEHEIGHT 24.0f
class CPiranhaPlant : public CPlantEnemy
{
public:
	CPiranhaPlant(float x, float y) : CPlantEnemy(x, y, PIRANHAPLANT_RISEHEIGHT) {}
	void Render() override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void ShootFireball() override { return; }
};

