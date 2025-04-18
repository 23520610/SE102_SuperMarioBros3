#include "Pipe.h"

void CPipe::Render()
{
	int aniId = ID_ANI_PIPE_DEFAULT;
	CAnimations* animations = CAnimations::GetInstance();
	if (pipeType == PIPE_TYPE_PIRANHA) {
		aniId = ID_ANI_PIPE_PIRANHA;
	}

	animations->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}


void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if(pipeType==PIPE_TYPE_PIRANHA){
		l = x - PIPE_BBOX_WIDTH / 2;
		t = y - PIPE_TYPE_PIRANHA_BBOX_HEIGHT / 2;
		r = l + PIPE_BBOX_WIDTH;
		b = t + PIPE_TYPE_PIRANHA_BBOX_HEIGHT;
	}
	else{
		l = x - PIPE_BBOX_WIDTH / 2;
		t = y - PIPE_BBOX_HEIGHT / 2;
		r = l + PIPE_BBOX_WIDTH;
		b = t + PIPE_BBOX_HEIGHT;
	}
	
}