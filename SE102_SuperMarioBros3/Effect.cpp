#include "Effect.h"
#include "debug.h"

CEffect::CEffect(float x, float y, int ani_id, DWORD duration)
{
    this->x = x;
    this->y = y;
    //this->ani_id = ani_id;
    this->duration = duration;
    this->time_start = GetTickCount64();
}
