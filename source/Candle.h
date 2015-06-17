
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
		BACK, BIG_CANDLE, SMALL_CANDLE,
		SMALL_FIRE, SMALL_MELT, SMALL_LIGHT,
		BIG_MELT, BIG_FIRE, BIG_LIGHT,
	};

	enum MOVE_MODE
	{
		FADE_IN, MELT, FADE_OUT,
	};
private:
	const static int IMAGE_WIDTH = 300;
	const static int IMAGE_HEIGHT = 300;
	static const int FPS = 60;			// frame per second
	static const int MPH = 24;			// minute per hour

	float			timeCount = 0.0f;					// 秒カウント
	float			frameCount = 0.0f;					// フレームカウント
	float			melt = 0.0f;					//	溶け具合
	float			alpha = 0.0f;				//	透明度
	int				mode = FADE_IN;
	int				animationCounter = 0;
	bool			animationflag = true;
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
	void Update_Time(float speed = 1.0f);
	void Update_Check(void);
	void	Update_Melt_Big(void);
	void	Update_Melt_Small(void);
};


//****************************************************************************************
#endif // !__CANDLE_H__
