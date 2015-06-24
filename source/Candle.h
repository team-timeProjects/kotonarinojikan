
#ifndef __CANDLE_H__
#define	__CANDLE_H__
#include "../IEX/iextreme.h"
#include "TimeObject.h"
#include "Campus.h"
//****************************************************************************************
//
//	Candleクラス
//
//****************************************************************************************

class Candle :public TimeObj
{
	//----------- field --------------
public:
	enum Behavior
	{
		SMALL = 0, BIG
	};
	enum Image
	{
		CANDLESTICK, BIG_CANDLE, SMALL_CANDLE,
		FIRE, ANIMATION_FIRE, SMALL_MELT, SMALL_LIGHT,
		BIG_MELT, BIG_LIGHT,
	};

	enum MOVE_MODE
	{
		FADE_IN, MELT, FADE_OUT,
	};
private:
	const static int IMAGE_WIDTH = 300;
	const static int IMAGE_HEIGHT = 300;
	static const int PICTURE_SPACE = 202;
	static const int MELT_PICTURE_SPACE = 274;
	static const int ADJUST_SCALE = 115;
	static const int PLUS_SCALE = 50;
	static const int MELT_ADJUST_SCALE = 185;
	static const int ANIMATION_MAX = 4;
	static const int ANIMATION_SPEED = 5;
	static const int ADJUST_FIRE_POS = -93;
	static const int ADJUST_CHECK_POS = -25;
	static const int FIRE_SCALE = 100;
	static const int LOAD_WIDTH = 256;
	static const int LOAD_HEIGHT = 256;
	static const int FPS = 60;			// frame per second
	static const int MPH = 24;			// minute per hour

	float			timeCount = 0.0f;					// 秒カウント
	float			frameCount = 0.0f;				// フレームカウント
	float			melt = 0.0f;					//	溶け具合
	float			alpha = 0.0f;				//	透明度
	float         fire_state = true;
	int				mode = FADE_IN;
	int				animationCounter = 0;
	int				animationTimer = 0;
	bool			animationVec = true;		//	アニメーション方向(正で右)
	bool			firevec = false;				//	炎向き(不で右)
	float			org_speed = 0.01f;
	//---------- method -------------
public:
	Candle(void);
	void Init(int id, const Vector2& centerPos, int colW, int colH, float scale, float speed, Behavior behavior);
	void AppendImage(int idx, iex2DObj* image, const ImageParam& param)override;
	void Update(void)override;
	void Render(void)override;
	void	RenderSmallCandle(void);
	void	RenderBigCandle(void);
private:
	void  Update_Time(float speed = 1.0f);
	void  Update_Check(void);
	void  UpdateAnimation(int speed = 1);
	void	Update_Melt_Big(void);
	void	Update_Melt_Small(void);
};


//****************************************************************************************
#endif // !__CANDLE_H__
