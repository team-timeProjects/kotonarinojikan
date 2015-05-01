#ifndef _CAMPUS_H_
#define _CAMPUS_H_

#include "../IEX/iextreme.h"
#include "Utility.h"


// 全体拡縮、全体移動のサポート
class Campus// 続投
{
private:
	Campus() {}
	Campus(const Campus& r) = delete;
	Campus& operator=(const Campus& r) = delete;
public:
	POINT pos;
	float scale;
	POINT target;
	float tscale;
	bool zoomEnd;
	~Campus() {}
	static Campus* Inst()
	{
		static Campus inst;
		return &inst;
	}
	// ターゲット座標を画面中央に向けて動かし、拡縮する
	void Zoom(int targetX, int targetY, float scale)
	{
		target.x = -targetX;
		target.y = -targetY;
		tscale = scale;
		zoomEnd = false;
	}
	// ズーム処理が終わった　or　終わっている
	bool IsZoomEnd()
	{
		return zoomEnd;
	}
	void Update()
	{
		if (abs(target.x - pos.x) < 2 && abs(target.y - pos.y) < 2 && abs(tscale - scale) < 0.01f)
		{
			pos.x = target.x;
			pos.y = target.y;
			scale = tscale;
			zoomEnd = true;
		}
		else
		{
			// 摩擦係数=0.9f
			int vx = (int)((target.x - pos.x)*(1.0f - 0.9f));
			int vy = (int)((target.y - pos.y)*(1.0f - 0.9f));
			float vs = (tscale - scale)*(1.0f - 0.9f);
			// 速度調整
			vs = abs(vs) > 1.0f ? vs > 0 ? 1.0f : -1.0f : vs;
			pos.x += vx;
			pos.y += vy;
			scale += vs;
		}
	}
	// スケールを座標に適用、キャンパス座標==ターゲット座標なら画面中央に描画される
	void Render(iex2DObj* image, int x, int y, int w, int h, int sx, int sy, int sw, int sh)
	{
		image->Render((int)(x*scale + (this->pos.x*scale) + iexSystem::ScreenWidth / 2),
					  (int)(y*scale + (this->pos.y*scale) + iexSystem::ScreenHeight / 2),
					  (int)(w*scale),
					  (int)(h*scale),
					  sx,
					  sy,
					  sw,
					  sh);
	}
};

#endif