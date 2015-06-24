
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
	case TimeObj::SUCCESS:
		SuccessCnt--;
		if (SuccessCnt < 0)SuccessCnt = 0;
		if (!Gold_Effect){
			Update_Time();
			fire_state = false;
			break;
		}
	case TimeObj::MOVE:
		fire_state = true;
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
		melt = 0.0f;
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
	y = pos.y - dst[SMALL_CANDLE].h*scale / 2 + dst[SMALL_CANDLE].y*scale + ADJUST_SCALE + (MELT_ADJUST_SCALE * melt);
	w = dst[SMALL_CANDLE].w*scale;
	h = dst[SMALL_CANDLE].h*scale - (ADJUST_SCALE + (MELT_ADJUST_SCALE * melt));
	sx = src[SMALL_CANDLE].x;
	sy = src[SMALL_CANDLE].y + (PICTURE_SPACE + (MELT_PICTURE_SPACE * melt));
	sw = src[SMALL_CANDLE].w;
	sh = src[SMALL_CANDLE].h - (PICTURE_SPACE + (MELT_PICTURE_SPACE * melt));
	Campus::GetInst()->Add(imageList[SMALL_CANDLE], x, y, w, h, sx, sy, sw, sh);

	//	ろうそく溶け
	x = pos.x - dst[SMALL_MELT].w*scale / 2 + dst[SMALL_MELT].x*scale;
	y = pos.y - dst[SMALL_MELT].h*scale / 2 + dst[SMALL_MELT].y*scale + (MELT_ADJUST_SCALE * melt);
	w = dst[SMALL_MELT].w*scale;
	h = dst[SMALL_MELT].h*scale;
	sx = src[SMALL_MELT].x;
	sy = src[SMALL_MELT].y;
	sw = src[SMALL_MELT].w;
	sh = src[SMALL_MELT].h;
	if (melt <= 0.9f)
		Campus::GetInst()->Add(imageList[SMALL_MELT], x, y, w, h, sx, sy, sw, sh);

	//	普通サイズのろうそく用炎
	int fire_scale = FIRE_SCALE;
	int fire_adjust = 0;
	if (state == CHECK)
	{
		fire_scale = FIRE_SCALE + PLUS_SCALE;
		fire_adjust = ADJUST_CHECK_POS;
	}
	x = pos.x - fire_scale*scale / 2 + dst[ANIMATION_FIRE].x*scale;
	y = pos.y - fire_scale*scale / 2 + dst[ANIMATION_FIRE].y*scale + (ADJUST_FIRE_POS + fire_adjust + (MELT_ADJUST_SCALE * melt));
	w = fire_scale*scale;
	h = fire_scale*scale;
	sx = src[ANIMATION_FIRE].x + LOAD_WIDTH * animationCounter;
	sy = src[ANIMATION_FIRE].y + (firevec * LOAD_HEIGHT);
	sw = src[ANIMATION_FIRE].w;
	sh = src[ANIMATION_FIRE].h;
	if (melt <= 0.9f && fire_state)
		Campus::GetInst()->Add(imageList[ANIMATION_FIRE], x, y, w, h, sx, sy, sw, sh);

	//	燭台
	x = pos.x - dst[CANDLESTICK].w*scale / 2 + dst[CANDLESTICK].x*scale;
	y = pos.y - dst[CANDLESTICK].h*scale / 2 + dst[CANDLESTICK].y*scale;
	w = dst[CANDLESTICK].w*scale;
	h = dst[CANDLESTICK].h*scale;
	sx = src[CANDLESTICK].x;
	sy = src[CANDLESTICK].y;
	sw = src[CANDLESTICK].w;
	sh = src[CANDLESTICK].h;
	Campus::GetInst()->Add(imageList[CANDLESTICK], x, y, w, h, sx, sy, sw, sh);

	//卍成功演出用時計
	if (state == State::SUCCESS&&SuccessCnt > 0){
		DWORD S_Alpha = ARGB((int)((float)SuccessCnt / (float)SUCCESS_EFFECT_TIME*127.0f), 255, 255, 255);
		float AddScale = (float)Mine_SChain / (float)CHAIN_EFFECT_MAX*CHAIN_MAX_SCALE;
		AddScale *= 1.0f - (float)SuccessCnt / (float)SUCCESS_EFFECT_TIME;
		int w = dst[ANIMATION_FIRE].w*(AddScale + scale);
		int h = dst[ANIMATION_FIRE].h*(AddScale + scale);
		Campus::GetInst()->Add(imageList[ANIMATION_FIRE],
			pos.x - dst[ANIMATION_FIRE].w*(AddScale + scale) / 2 + dst[ANIMATION_FIRE].x*(AddScale + scale),
			pos.y - h / 1.5f + dst[ANIMATION_FIRE].y*scale + (ADJUST_FIRE_POS + fire_adjust + (MELT_ADJUST_SCALE * melt)),
			dst[ANIMATION_FIRE].w*(AddScale + scale), dst[ANIMATION_FIRE].h*(AddScale + scale),
			src[ANIMATION_FIRE].x, src[ANIMATION_FIRE].y, src[ANIMATION_FIRE].w, src[ANIMATION_FIRE].h, GetPoint(0, 0), 0, 1, RS_COPY, (DWORD)S_Alpha, 0.0f);
	}
}

//	大きいサイズのロウソク描画
void	Candle::RenderBigCandle(void)
{
	float x, y, w, h, sx, sy, sw, sh;
	//	普通サイズのろうそく
	x = pos.x - dst[BIG_CANDLE].w*scale / 2 + dst[BIG_CANDLE].x*scale;
	y = pos.y - dst[BIG_CANDLE].h*scale / 2 + dst[BIG_CANDLE].y*scale + ADJUST_SCALE + (MELT_ADJUST_SCALE * melt);
	w = dst[BIG_CANDLE].w*scale;
	h = dst[BIG_CANDLE].h*scale - (ADJUST_SCALE + (MELT_ADJUST_SCALE * melt));
	sx = src[BIG_CANDLE].x;
	sy = src[BIG_CANDLE].y + (PICTURE_SPACE + (MELT_PICTURE_SPACE * melt));
	sw = src[BIG_CANDLE].w;
	sh = src[BIG_CANDLE].h - (PICTURE_SPACE + (MELT_PICTURE_SPACE * melt));
	Campus::GetInst()->Add(imageList[BIG_CANDLE], x, y, w, h, sx, sy, sw, sh);

	//	ろうそく溶け
	x = pos.x - dst[BIG_MELT].w*scale / 2 + dst[BIG_MELT].x*scale;
	y = pos.y - dst[BIG_MELT].h*scale / 2 + dst[BIG_MELT].y*scale + (MELT_ADJUST_SCALE * melt);
	w = dst[BIG_MELT].w*scale;
	h = dst[BIG_MELT].h*scale;
	sx = src[BIG_MELT].x;
	sy = src[BIG_MELT].y;
	sw = src[BIG_MELT].w;
	sh = src[BIG_MELT].h;
	if (melt <= 0.9f)
		Campus::GetInst()->Add(imageList[BIG_MELT], x, y, w, h, sx, sy, sw, sh);

	//	普通サイズのろうそく用炎
	int fire_scale = FIRE_SCALE;
	int fire_adjust = 0;
	if (state == CHECK)
	{
		fire_scale = FIRE_SCALE + PLUS_SCALE;
		fire_adjust = ADJUST_CHECK_POS;
	}
	x = pos.x - fire_scale*scale / 2 + dst[ANIMATION_FIRE].x*scale;
	y = pos.y - fire_scale*scale / 2 + dst[ANIMATION_FIRE].y*scale + (ADJUST_FIRE_POS + fire_adjust + (MELT_ADJUST_SCALE * melt));
	w = fire_scale*scale;
	h = fire_scale*scale;
	sx = src[ANIMATION_FIRE].x + LOAD_WIDTH * animationCounter;
	sy = src[ANIMATION_FIRE].y + (firevec * LOAD_HEIGHT);
	sw = src[ANIMATION_FIRE].w;
	sh = src[ANIMATION_FIRE].h;
	if (melt <= 0.9f && fire_state)
		Campus::GetInst()->Add(imageList[ANIMATION_FIRE], x, y, w, h, sx, sy, sw, sh);

	//卍成功演出用時計
	if (state == State::SUCCESS&&SuccessCnt > 0){
		DWORD S_Alpha = ARGB((int)((float)SuccessCnt / (float)SUCCESS_EFFECT_TIME*127.0f), 255, 255, 255);
		float AddScale = (float)Mine_SChain / (float)CHAIN_EFFECT_MAX*CHAIN_MAX_SCALE;
		AddScale *= 1.0f - (float)SuccessCnt / (float)SUCCESS_EFFECT_TIME;
		int w = dst[ANIMATION_FIRE].w*(AddScale + scale);
		int h = dst[ANIMATION_FIRE].h*(AddScale + scale);
		Campus::GetInst()->Add(imageList[ANIMATION_FIRE],
			pos.x - dst[ANIMATION_FIRE].w*(AddScale + scale) / 2 + dst[ANIMATION_FIRE].x*(AddScale + scale),
			pos.y - h / 1.5f+ dst[ANIMATION_FIRE].y*scale + (ADJUST_FIRE_POS + fire_adjust + (MELT_ADJUST_SCALE * melt)),
			dst[ANIMATION_FIRE].w*(AddScale + scale), dst[ANIMATION_FIRE].h*(AddScale + scale),
			src[ANIMATION_FIRE].x, src[ANIMATION_FIRE].y, src[ANIMATION_FIRE].w, src[ANIMATION_FIRE].h, GetPoint(0, 0), 0, 1, RS_COPY, (DWORD)S_Alpha, 0.0f);
	}

	//	燭台
	x = pos.x - dst[CANDLESTICK].w*scale / 2 + dst[CANDLESTICK].x*scale;
	y = pos.y - dst[CANDLESTICK].h*scale / 2 + dst[CANDLESTICK].y*scale;
	w = dst[CANDLESTICK].w*scale;
	h = dst[CANDLESTICK].h*scale;
	sx = src[CANDLESTICK].x;
	sy = src[CANDLESTICK].y;
	sw = src[CANDLESTICK].w;
	sh = src[CANDLESTICK].h;
	Campus::GetInst()->Add(imageList[CANDLESTICK], x, y, w, h, sx, sy, sw, sh);
}

//	時間更新
inline void Candle::Update_Time(float speed)
{
	//時間経過のベクトルに応じて逆転
	float v = this->speed;
	timeCount = org_speed * this->speed;
}

//	チェック中の動作
inline void Candle::Update_Check(void)
{
	UpdateAnimation(4);
}

//	大きいサイズのロウソク動作
inline	 void	Candle::Update_Melt_Big( void )
{
	Update_Time();
	UpdateAnimation();

	melt += timeCount / 2;

	if ( melt > 1.0f )	melt = 0.0f;
	if ( melt < 0.0f )	melt = 1.0f;
}

//	小さいサイズのロウソク動作
inline	 void	Candle::Update_Melt_Small( void )
{
	Update_Time();
	UpdateAnimation();

	melt += timeCount;

	if (melt > 1.0f)	melt = 0.0f;
	if (melt < 0.0f)	melt = 1.0f;
}

//	炎アニメーション
inline	 void	Candle::UpdateAnimation( int speed )
{
	//時間経過のベクトルに応じて逆転
	int v = this->speed > 0 ? speed : -speed;

	animationTimer += speed;

	if ( animationTimer >= ANIMATION_SPEED )
	{
		animationTimer = 0;
		if ( animationVec )
			animationCounter++;
		else
			animationCounter--;

		if (animationCounter >= ANIMATION_MAX)
		{
			animationCounter = ANIMATION_MAX - 1;
			animationVec = !animationVec;
		}

		if ( animationCounter <= 0 )
		{
			animationCounter = 0;
			animationVec = !animationVec;
			firevec = !firevec;
		}
	}
}
