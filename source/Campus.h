#ifndef _CAMPUS_H_
#define _CAMPUS_H_

#include "../IEX/iextreme.h"
#include "Utility.h"


// 全体拡縮、全体移動のサポート
class Campus
{
private:
	Campus();
	Campus(const Campus& r) = delete;
	Campus& operator=(const Campus& r) = delete;
public:
	const int NEARLY_POS;
	const float NEARLY_SCALE;
	float posx,posy;
	float scale;
	POINT target;
	float tscale;
	float friction;
	bool zoomEnd;
	static Campus* Inst()
	{
		static Campus inst;
		return &inst;
	}
	// ターゲット座標を画面中央に向けて動かし、拡縮する
	void Zoom(int targetX, int targetY, float scale,float friction=0.9f);
	// ズーム処理が終わった　or　終わっている
	bool IsZoomEnd();
	void Update();
	// スケールを座標に適用、キャンパス座標==ターゲット座標なら画面中央に描画される
	void Render(iex2DObj* image, int x, int y, int w, int h, int sx, int sy, int sw, int sh);
};

#endif