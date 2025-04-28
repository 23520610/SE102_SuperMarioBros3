#pragma once
#include "PlantEnemy.h"

#define GREENPLANT_BBOX_WIDTH 16
#define GREENPLANT_BBOX_HEIGHT 24
#define GREENPLANT_RISEHEIGHT 24.0f
#define ID_ANI_GREENPLANT_RISED_LEFT 15000
#define ID_ANI_GREENPLANT_RISED_RIGHT 15001
#define ID_ANI_GREENPLANT_LEFT_FACEUP 15002
#define ID_ANI_GREENPLANT_RIGHT_FACEUP 15003
#define ID_ANI_GREENPLANT_DIVING_LEFT 15004
#define ID_ANI_GREENPLANT_DIVING_RIGHT 15005
#define ID_ANI_GREENPLANT_DIVING_LEFT_FACEUP 15006
#define ID_ANI_GREENPLANT_DIVING_RIGHT_FACEUP 15007
class CGreenPlant : public CPlantEnemy
{
public:
	CGreenPlant(float x, float y) : CPlantEnemy(x, y, GREENPLANT_RISEHEIGHT) {}
	void Render() override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};

