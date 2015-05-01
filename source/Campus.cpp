#include "Campus.h"

Campus::Campus() :NEARLY_POS(2), NEARLY_SCALE(0.01f)
{
	posx = 0;
	posy = 0;
	scale = 0.0f;
	target = {0, 0};
	tscale = 0.0f;
	friction = 0.0f;
	zoomEnd = true;
}

void Campus::Zoom(int targetX, int targetY, float scale, float friction)
{
	target.x = -targetX;
	target.y = -targetY;
	tscale = scale;
	this->friction = friction;
	zoomEnd = false;
}

bool Campus::IsZoomEnd()
{
	return zoomEnd;
}

void Campus::Update()
{
	if (abs(target.x - posx) < NEARLY_POS &&
		abs(target.y - posy) < NEARLY_POS &&
		abs(tscale - scale) < NEARLY_SCALE)
	{
		posx = (float)target.x;
		posy = (float)target.y;
		scale = tscale;
		zoomEnd = true;
	}
	else
	{
		float vx = (target.x - posx)*(1.0f - friction);
		float vy = (target.y - posy)*(1.0f - friction);
		float vs = (tscale - scale)*(1.0f - friction);
		// ‘¬“x’²®
		vs = abs(vs) > 1.0f ? (vs > 0 ? 1.0f : -1.0f) : vs;
		posx += vx;
		posy += vy;
		scale += vs;
	}
}

void Campus::Render(iex2DObj* image, int x, int y, int w, int h, int sx, int sy, int sw, int sh)
{
	image->Render((int)(x*scale + (this->posx*scale) + iexSystem::ScreenWidth / 2),
				  (int)(y*scale + (this->posy*scale) + iexSystem::ScreenHeight / 2),
				  (int)(w*scale),
				  (int)(h*scale),
				  sx,
				  sy,
				  sw,
				  sh);
}
