
#include	"iextreme.h"
#include	"Control.h"
#include	"Game.h"
#include	"Metronom.h"

//****************************************************************************************
//
//	Metronomクラス
//
//****************************************************************************************

//--------------------------------------------------------------------------------------------
//	初期化・解放
//--------------------------------------------------------------------------------------------

//	コンストラクタ
Metronom::Metronom(void)
{}

//	初期化
void Metronom::Init(int id, const Vector2& centerPos, int colW, int colH, float scale, float speed, Behavior behavior)
{
	TimeObj::Init(id, centerPos, colW, colH, scale, speed, behavior);
	timeCount = 0.0f;
	frameCount = 0;
	melt = 0.0f;
	alpha = 0.0f;
	animationCounter = 0;
	org_speed = 0.01f;
}

//	イメージ追加
void Metronom::AppendImage(int idx, iex2DObj* image, const ImageParam& param)
{
	TimeObj::AppendImage(idx, image, param);
	dst[idx].w = IMAGE_WIDTH;
	dst[idx].h = IMAGE_HEIGHT;
}

//	更新
void Metronom::Update(void)
{
	switch (state)
	{
	case TimeObj::SUCCESS:
		SuccessCnt--;
		if (!Gold_Effect){
			Update_Time();
			angle = 0.0f;
			break;
		}

	case TimeObj::MOVE:
		shakeflag = false;
		switch (behavior)
		{
		case ORIGIN:
			Move();
			break;

		case DOUBLE:
			Move(2.0f);
			break;
		}
		break;
	case TimeObj::STOP:
		timeCount = frameCount = 0;
		break;
	case TimeObj::CHECK:
		Update_Check();
		break;
	default:
		break;
	}
}

//	描画
void Metronom::Render(void)
{
	POINT p;
	p.x = 0;
	p.y = 25;
	float x, y, w, h, sx, sy, sw, sh;

	//	メトロノーム
	x = pos.x - dst[METRONOM].w*scale / 2 + dst[METRONOM].x*scale;
	y = pos.y - dst[METRONOM].h*scale / 2 + dst[METRONOM].y*scale;
	w = dst[METRONOM].w*scale;
	h = dst[METRONOM].h*scale;
	sx = src[METRONOM].x;
	sy = src[METRONOM].y;
	sw = src[METRONOM].w;
	sh = src[METRONOM].h;
	Campus::GetInst()->Add(imageList[METRONOM], x, y, w, h, sx, sy, sw, sh);

	//	針描画
	x = pos.x - dst[NEEDLE].w*scale / 2 + dst[NEEDLE].x*scale;
	y = pos.y - dst[NEEDLE].h*scale / 2 + dst[NEEDLE].y*scale + 8;
	w = dst[NEEDLE].w*scale;
	h = dst[NEEDLE].h*scale;
	sx = src[NEEDLE].x;
	sy = src[NEEDLE].y;
	sw = src[NEEDLE].w;
	sh = src[NEEDLE].h;
	Campus::GetInst()->Add(imageList[NEEDLE], x, y, w, h, sx, sy, sw, sh, p, angle);

	if (behavior == ORIGIN)
	{
		//	錘1描画
		x = pos.x - dst[SPINDLE1].w*scale / 2 + dst[SPINDLE1].x*scale;
		y = pos.y - dst[SPINDLE1].h*scale / 2 + dst[SPINDLE1].y*scale + 8;
		w = dst[SPINDLE1].w*scale;
		h = dst[SPINDLE1].h*scale;
		sx = src[SPINDLE1].x;
		sy = src[SPINDLE1].y;
		sw = src[SPINDLE1].w;
		sh = src[SPINDLE1].h;
		Campus::GetInst()->Add(imageList[SPINDLE1], x, y, w, h, sx, sy, sw, sh, p, angle);
	}

	if (behavior == DOUBLE)
	{
		//	錘2描画
		x = pos.x - dst[SPINDLE2].w*scale / 2 + dst[SPINDLE2].x*scale;
		y = pos.y - dst[SPINDLE2].h*scale / 2 + dst[SPINDLE2].y*scale + 8;
		w = dst[SPINDLE2].w*scale;
		h = dst[SPINDLE2].h*scale;
		sx = src[SPINDLE2].x;
		sy = src[SPINDLE2].y;
		sw = src[SPINDLE2].w;
		sh = src[SPINDLE2].h;
		Campus::GetInst()->Add(imageList[SPINDLE2], x, y, w, h, sx, sy, sw, sh, p, angle);
	}

	//	メトロノーム
	x = pos.x - dst[FACE].w*scale / 2 + dst[FACE].x*scale;
	y = pos.y - dst[FACE].h*scale / 2 + dst[FACE].y*scale;
	w = dst[FACE].w*scale;
	h = dst[FACE].h*scale;
	sx = src[FACE].x;
	sy = src[FACE].y;
	sw = src[FACE].w;
	sh = src[FACE].h;
	Campus::GetInst()->Add(imageList[FACE], x, y, w, h, sx, sy, sw, sh);

	//卍成功演出用時計
	if (state == State::SUCCESS&&SuccessCnt > 0){
		DWORD S_Alpha = ARGB((int)((float)SuccessCnt / (float)SUCCESS_EFFECT_TIME*127.0f), 255, 255, 255);
		float AddScale = (float)Mine_SChain / (float)CHAIN_EFFECT_MAX*CHAIN_MAX_SCALE;
		AddScale *= 1.0f - (float)SuccessCnt / (float)SUCCESS_EFFECT_TIME;
		Campus::GetInst()->Add(imageList[BACK],
			pos.x - dst[BACK].w*(AddScale + scale) / 2 + dst[BACK].x*(AddScale + scale),
			pos.y - dst[BACK].h*(AddScale + scale) / 2 + dst[BACK].y*(AddScale + scale),
			dst[BACK].w*(AddScale + scale), dst[BACK].h*(AddScale + scale),
			src[BACK].x, src[BACK].y, src[BACK].w, src[BACK].h, GetPoint(0, 0), 0, 1, RS_COPY, (DWORD)S_Alpha, 0.0f);
	}
}

//	時間更新
inline void Metronom::Update_Time(float speed)
{
	//時間経過のベクトルに応じて逆転
	float v = this->speed * speed;
	timeCount += org_speed * v;
}

//	チェック中の動作
inline void Metronom::Update_Check(void)
{
	shakeParam = rand() % 5;
	if (angle > D3DX_PI / 180 * -40.0f)
	{
		angle -= D3DX_PI / 180 * 10.0f;
	}
	else
	{
		shakeflag = true;
	}

	if (shakeflag)
	{
		param = D3DX_PI / 180 * -40.0f;
		angle = (D3DX_PI / 180 * -40.0f) + (D3DX_PI / 180 * shakeParam);
	}
}

//	動作かちかち
void	Metronom::Move( float speed )
{
	Update_Time(speed);
	angle = (D3DX_PI / 180 * 45) * sinf(timeCount);
}
