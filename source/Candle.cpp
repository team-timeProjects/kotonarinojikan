
#include	"iextreme.h"
#include	"Game.h"
#include	"Control.h"

#include	"Candle.h"

//****************************************************************************************
//
//	Candleクラス
//
//****************************************************************************************
//----------------------------------------------------------------------------
//	初期化・解放
//----------------------------------------------------------------------------

//	コンストラクタ
Candle::Candle(void)
{}

//	初期化
void Candle::Init(int id, const Vector2& centerPos, int colW, int colH, float scale, float speed, Behavior behavior)
{
	TimeObj::Init(id, centerPos, colW, colH, scale, speed, behavior);
	timeCount = 0.0f;
	frameCount = 0;
	melt = 0.0f;
	alpha = 0.0f;
	mode = MELT;
	animationCounter = 0;
	org_speed = 0.01f;
}

//	イメージ追加
void Candle::AppendImage(int idx, iex2DObj* image, const ImageParam& param)
{
	TimeObj::AppendImage(idx, image, param);
	dst[idx].w = IMAGE_WIDTH;
	dst[idx].h = IMAGE_HEIGHT;
}

//	更新
void Candle::Update(void)
{
	switch (state)
	{
	case TimeObj::MOVE:
		switch (behavior)
		{
		case Behavior::SMALL:
			Update_Melt_Small();
			break;
		case Behavior::BIG:
			Update_Melt_Big();
			break;
		default:
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
void Candle::Render(void)
{
	POINT p;
	p.x = pos.x;
	p.y = pos.y;

	switch (behavior)
	{
	case Behavior::SMALL:
		RenderSmallCandle();
		break;
	case Behavior::BIG:
		RenderBigCandle();
		break;
	default:
		break;
	}
}

//	普通サイズのロウソク描画
void	Candle::RenderSmallCandle(void)
{
	float x, y, w, h, sx, sy, sw, sh;

	//	普通サイズのろうそく
	x = pos.x - dst[SMALL_CANDLE].w*scale / 2 + dst[SMALL_CANDLE].x*scale;
	y = pos.y - dst[SMALL_CANDLE].h*scale / 2 + dst[SMALL_CANDLE].y*scale + (110 * 0.6f) + (200 * 0.6f) * melt;
	w = dst[SMALL_CANDLE].w*scale;
	h = dst[SMALL_CANDLE].h*scale - (110 * 0.6f) - (200 * 0.6f) * melt;
	sx = src[SMALL_CANDLE].x;
	sy = src[SMALL_CANDLE].y + (200 * melt);
	sw = src[SMALL_CANDLE].w;
	sh = src[SMALL_CANDLE].h - (200 * melt);
	Campus::GetInst()->Add(imageList[SMALL_CANDLE], x, y, w, h, sx, sy, sw, sh);

	//	普通サイズのロウソク用明かり
	x = pos.x - dst[SMALL_LIGHT].w*scale / 2 + dst[SMALL_LIGHT].x*scale;
	y = pos.y - dst[SMALL_LIGHT].h*scale / 2 + dst[SMALL_LIGHT].x*scale + (110 * 0.6f) + (200 * 0.6f) * melt;
	h = dst[SMALL_LIGHT].h*scale;
	sx = src[SMALL_LIGHT].x;
	sy = src[SMALL_LIGHT].y;
	sw = src[SMALL_LIGHT].w;
	sh = src[SMALL_LIGHT].h;
	Campus::GetInst()->Add(imageList[SMALL_LIGHT], x, y - 175, w, h, sx, sy, sw, sh);

	//	普通サイズのろうそく用炎
	x = pos.x - dst[SMALL_FIRE].w*scale / 2 + dst[SMALL_FIRE].x*scale;
	y = pos.y - dst[SMALL_FIRE].h*scale / 2 + dst[SMALL_FIRE].y*scale + (110 * 0.6f) + (200 * 0.6f) * melt;
	h = dst[SMALL_FIRE].h*scale;
	sx = src[SMALL_FIRE].x;
	sy = src[SMALL_FIRE].y;
	sw = src[SMALL_FIRE].w;
	sh = src[SMALL_FIRE].h;
	if (melt <= 0.9f)
		Campus::GetInst()->Add(imageList[SMALL_FIRE], x, y - 195, w, h, sx, sy, sw, sh);
}

//	大きいサイズのロウソク描画
void	Candle::RenderBigCandle(void)
{
	float x, y, w, h, sx, sy, sw, sh;
	//	大きいサイズのろうそく
	x = pos.x - dst[BIG_CANDLE].w*scale / 2 + dst[BIG_CANDLE].x*scale;
	y = pos.y - dst[BIG_CANDLE].h*scale / 2 + dst[BIG_CANDLE].x*scale + (120 * 0.6f) + (190 * 0.6f) * melt;
	w = dst[BIG_CANDLE].w*scale;
	h = dst[BIG_CANDLE].h*scale - (120 * 0.6f) - (190 * 0.6f) * melt;
	sx = 0;
	sy = 120 + (190 * melt);
	sw = 512;
	sh = 402 - (190 * melt);
	Campus::GetInst()->Add(imageList[BIG_CANDLE], x, y, w, h, sx, sy, sw, sh);

	//	大きいサイズのロウソク用明かり
	x = pos.x - dst[BIG_LIGHT].w*scale / 2 + dst[BIG_LIGHT].x*scale;
	y = pos.y - dst[BIG_LIGHT].h*scale / 2 + dst[BIG_LIGHT].x*scale + (120 * 0.6f) + (190 * 0.6f) * melt;
	w = dst[BIG_CANDLE].w*scale;
	h = dst[BIG_LIGHT].h*scale;
	sx = src[BIG_LIGHT].x;
	sy = src[BIG_LIGHT].y;
	sw = src[BIG_LIGHT].w;
	sh = src[BIG_LIGHT].h;
	Campus::GetInst()->Add(imageList[BIG_LIGHT], x, y - 135, w, h, sx, sy, sw, sh);

	//	大きいサイズのろうそく用炎
	x = pos.x - dst[BIG_FIRE].w*scale / 2 + dst[BIG_FIRE].x*scale;
	y = pos.y - dst[BIG_FIRE].h*scale / 2 + dst[BIG_FIRE].x*scale + (120 * 0.6f) + (190 * 0.6f) * melt;
	w = dst[BIG_FIRE].w*scale;
	h = dst[BIG_FIRE].h*scale;
	sx = src[BIG_FIRE].x;
	sy = src[BIG_FIRE].y;
	sw = src[BIG_FIRE].w;
	sh = src[BIG_FIRE].h;

	if (melt <= 0.9f)
		Campus::GetInst()->Add(imageList[BIG_FIRE], x + 10, y - 185, w, h, sx, sy, sw, sh);
}

//	時間更新
inline void Candle::Update_Time(float speed)
{
	//時間経過のベクトルに応じて逆転
	float v = this->speed > 0 ? speed : -speed;
	timeCount = org_speed * v;
}

//	チェック中の動作
inline void Candle::Update_Check(void)
{

}

//	大きいサイズのロウソク動作
inline	 void	Candle::Update_Melt_Big(void)
{
	Update_Time();

	switch (mode)
	{
	case FADE_IN:
		alpha += timeCount;
		if (alpha > 1.0f){
			alpha = 1.0f;
			mode = MELT;
		}
		if (alpha < 0.0f)
		{
			alpha = 0.0f;
			mode = FADE_OUT;
		}
		break;
	case MELT:
		melt += timeCount * 0.5f;
		if (melt > 1.0f){
			melt = 1.0f;
			mode = FADE_OUT;
		}

		if (melt < 0.0f)
		{
			melt = 0.0f;
			mode = FADE_IN;
		}
		break;

	case FADE_OUT:
		alpha -= timeCount;
		if (alpha < 0.0f){
			alpha = 0.0f;
			melt = 0.0f;
			mode = FADE_IN;
		}
		if (alpha > 1.0f)
		{
			alpha = 1.0f;
			melt = 1.0f;
			mode = MELT;
		}
		break;
	}
}

//	小さいサイズのロウソク動作
inline	 void	Candle::Update_Melt_Small(void)
{
	Update_Time();

	switch (mode)
	{
	case FADE_IN:
		alpha += timeCount;
		if (alpha > 1.0f){
			alpha = 1.0f;
			mode = MELT;
		}
		if (alpha < 0.0f)
		{
			alpha = 0.0f;
			mode = FADE_OUT;
		}
		break;
	case MELT:
		melt += timeCount;
		if (melt > 1.0f){
			melt = 1.0f;
			mode = FADE_OUT;
		}

		if (melt < 0.0f)
		{
			melt = 0.0f;
			mode = FADE_IN;
		}
		break;

	case FADE_OUT:
		alpha -= timeCount;
		if (alpha < 0.0f){
			alpha = 0.0f;
			melt = 0.0f;
			mode = FADE_IN;
		}
		if (alpha > 1.0f)
		{
			alpha = 1.0f;
			melt = 1.0f;
			mode = MELT;
		}
		break;
	}
}
