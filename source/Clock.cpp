
#include	"iextreme.h"
#include	"Control.h"
#include	"Game.h"

#include	"Clock.h"
#include	<random>
//****************************************************************************************
//
//	Clockクラス
//
//****************************************************************************************

//----------------------------------------------------------------------------
//	定数宣言
//----------------------------------------------------------------------------
const float Clock::ANGLE_1MINUTE = 2 * PI / MPH;
const float Clock::ANGLE_1HOUR = 2 * PI / HOUR_CYCLE;

//----------------------------------------------------------------------------
//	初期化・解放
//----------------------------------------------------------------------------

Clock::Clock()
{}

void Clock::Init(int id, const Vector2& centerPos, int colW, int colH, float scale, float speed, Behavior behavior)
{
	TimeObj::Init(id, centerPos, colW, colH, scale, speed, behavior);
	timeCount = 0;
	frameCount = 0;
	hourAngle = 0;
	minuteAngle = 0;

}

void Clock::AppendImage(int idx, iex2DObj* image, const ImageParam& param)
{
	TimeObj::AppendImage(idx, image, param);
	dst[idx].w = IMAGE_WIDTH;
	dst[idx].h = IMAGE_HEIGHT;
}

void Clock::Update()
{
	switch (state)
	{
	case TimeObj::SUCCESS:
		SuccessCnt--;
		if (SuccessCnt < 0)SuccessCnt = 0;
		if (!Gold_Effect)
		{
			switch (behavior)
			{
			case Behavior::SMOOTH:
			case Behavior::STEPING:
				Update_Time();
				break;
			case Behavior::STANDSTILL:
				Update_Time_Standstill();
				break;
			}
			minuteAngle = 0;
			hourAngle = 0;
			break;
		}
	case TimeObj::MOVE:
		switch (behavior)
		{
		case Behavior::SMOOTH:
			Update_Time();
			Update_Smooth();
			break;

		case Behavior::STEPING:
			Update_Time();
			Update_Steping();
			break;

		case Behavior::GO3_BACK2_STEPING:
			Update_Time_Go3Back2_Steping();
			break;

		case Behavior::GO3_BACK2_SMOOTH:
			Update_Time_Go3Back2_Smooth();
			break;

		case Behavior::STANDSTILL:
			Update_Time_Standstill();
			Update_Steping();
			break;

		default:
			break;
		}
		break;
	case TimeObj::STOP:
		timeCount = frameCount = 0;
		// 分単位の角度+秒単位の角度
		minuteAngle = (timeCount % MPH * ANGLE_1MINUTE) + (frameCount*(ANGLE_1MINUTE / (FPS / abs(speed))));
		// 時間単位+分単位+秒単位
		hourAngle = (timeCount / MPH * ANGLE_1HOUR) + (timeCount %MPH * (ANGLE_1HOUR / MPH)) + (frameCount*(ANGLE_1HOUR / MPH / (FPS / abs(speed))));
		break;
	case TimeObj::CHECK:
		Update_Check();
		break;
	default:
		break;
	}
}

void Clock::Render()
{
	POINT p;
	p.x = 0;
	p.y = 12 * scale;

	Campus::GetInst()->Add(imageList[BACK], pos.x - dst[BACK].w*scale / 2 + dst[BACK].x*scale,
		pos.y - dst[BACK].h*scale / 2 + dst[BACK].y*scale,
		dst[BACK].w*scale, dst[BACK].h*scale,
		src[BACK].x, src[BACK].y, src[BACK].w, src[BACK].h);

	p.y = 0;

	Campus::GetInst()->Add(imageList[HOUR], pos.x - dst[HOUR].w*scale / 2 + dst[HOUR].x*scale,
		pos.y - dst[HOUR].h*scale / 2 + dst[HOUR].y*scale,
		dst[HOUR].w*scale, dst[HOUR].h*scale,
		src[HOUR].x, src[HOUR].y, src[HOUR].w, src[HOUR].h,
		p, hourAngle);
	Campus::GetInst()->Add(imageList[MINUTE], pos.x - dst[MINUTE].w*scale / 2 + dst[MINUTE].x*scale,
		pos.y - dst[MINUTE].h*scale / 2 + dst[MINUTE].y*scale,
		dst[MINUTE].w*scale, dst[MINUTE].h*scale,
		src[MINUTE].x, src[MINUTE].y, src[MINUTE].w, src[MINUTE].h,
		p, minuteAngle);
	//卍成功演出用時計
	if (state == State::SUCCESS&&SuccessCnt > 0)
	{
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

void Clock::Update_Time(float speed)
{
	//時間経過のベクトルに応じて逆転
	int v = this->speed > 0 ? speed : -speed;

	frameCount += v;
	stepTimer += v;

	// 正側
	if (frameCount >= FPS / abs(this->speed))
	{
		frameCount -= FPS / abs(this->speed);
		timeCount += v;
		if (timeCount >= MPH*HOUR_CYCLE)
			timeCount -= timeCount / (MPH*HOUR_CYCLE)*(MPH*HOUR_CYCLE);
	}
	// 逆側
	else if (frameCount < 0)
	{
		frameCount += FPS / abs(this->speed);
		timeCount += v;
		if (timeCount < 0)
			timeCount += timeCount / (MPH*HOUR_CYCLE)*(MPH*HOUR_CYCLE);
	}
}

void Clock::Update_Check()
{
	Update_Time(50);
	// 分単位の角度+秒単位の角度
	minuteAngle = (timeCount % MPH * ANGLE_1MINUTE) + (frameCount*(ANGLE_1MINUTE / (FPS / speed)));
	// 時間単位+分単位+秒単位
	hourAngle = (timeCount / MPH * ANGLE_1HOUR) + (timeCount %MPH * (ANGLE_1HOUR / MPH)) + (frameCount*(ANGLE_1HOUR / MPH / (FPS / speed)));
}

void Clock::Update_Smooth()
{
	// 分単位の角度+秒単位の角度
	minuteAngle = (timeCount % MPH * ANGLE_1MINUTE) + (frameCount*(ANGLE_1MINUTE / (FPS / abs(speed))));
	// 時間単位+分単位+秒単位
	hourAngle = (timeCount / MPH * ANGLE_1HOUR) + (timeCount %MPH * (ANGLE_1HOUR / MPH)) + (frameCount*(ANGLE_1HOUR / MPH / (FPS / abs(speed))));
}

void Clock::Update_Steping()
{
	minuteAngle = timeCount % MPH * ANGLE_1MINUTE;
	hourAngle = (timeCount / MPH * ANGLE_1HOUR) + (timeCount % MPH * (ANGLE_1HOUR / MPH));
}

void Clock::Update_Time_Standstill(float speed)
{
	int v = this->speed > 0 ? speed : -speed;
	int timePerFrame = (int)(FPS / abs(this->speed));
	timeTotal += v;
	frameTotal++;
	if (timeTotal % timePerFrame == 0)
	{
		frameTotal = 0;
	}
	if (frameTotal < 2)
		timeCount = timeOffset + v;
	else
		timeCount = timeOffset;

}

void Clock::Update_Time_Go3Back2_Steping(float speed)
{
	if (vecflag)
	{
		Update_Time();
		if (stepTimer % (int)((FPS) / abs(this->speed)) == 0)
		{
			stepCount++;
		}

		if (stepCount > 3)
		{
			vecflag = false;
			stepCount = 0;
			for (int i = 0; i < (int)((FPS) / abs(this->speed)); i++)Update_Time(-1.0f);
		}
	}
	else
	{
		if (stepTimer % (int)((FPS) / abs(this->speed)) == 0)
		{
			stepCount++;
		}

		if (stepCount > 2)
		{
			vecflag = true;
			stepCount = 0;
			for (int i = 0; i < (int)((FPS) / abs(this->speed)); i++)Update_Time();
		}
		Update_Time(-1.0f);
	}

	minuteAngle = timeCount % MPH * ANGLE_1MINUTE;
	hourAngle = (timeCount / MPH * ANGLE_1HOUR) + (timeCount % MPH * (ANGLE_1HOUR / MPH));
}

void Clock::Update_Time_Go3Back2_Smooth(float speed)
{
	if (vecflag)
	{
		Update_Time();
		if (stepTimer % (int)((FPS) / abs(this->speed)) == 0)
		{
			stepCount++;
		}

		if (stepCount > 3)
		{
			vecflag = false;
			stepCount = 0;
			Update_Time(-1.0f);
		}
	}
	else
	{
		if (stepTimer % (int)((FPS) / abs(this->speed)) == 0)
		{
			stepCount++;
		}

		if (stepCount > 2)
		{
			vecflag = true;
			stepCount = 0;
			Update_Time();
		}
		Update_Time(-1.0f);
	}

	// 分単位の角度+秒単位の角度
	minuteAngle = (timeCount % MPH * ANGLE_1MINUTE) + (frameCount*(ANGLE_1MINUTE / (FPS / abs(speed))));
	// 時間単位+分単位+秒単位
	hourAngle = (timeCount / MPH * ANGLE_1HOUR) + (timeCount %MPH * (ANGLE_1HOUR / MPH)) + (frameCount*(ANGLE_1HOUR / MPH / (FPS / abs(speed))));
}

