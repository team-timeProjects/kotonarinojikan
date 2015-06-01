
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
	switch(state)
	{
		case TimeObj::MOVE:
			switch(behavior)
			{
				case Behavior::SMOOTH:
					Update_Smooth();
					break;
				case Behavior::STEPING:
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
	p.x = pos.x;
	p.y = pos.y;

	Campus::GetInst()->Add(imageList[BACK], pos.x - dst[BACK].w*scale / 2 + dst[BACK].x*scale,
						   pos.y - dst[BACK].h*scale / 2 + dst[BACK].y*scale,
						   dst[BACK].w*scale, dst[BACK].h*scale,
						   src[BACK].x, src[BACK].y, src[BACK].w, src[BACK].h);

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
}

inline void Clock::Update_Time(float speed)
{
	//時間経過のベクトルに応じて逆転
	int v = this->speed > 0 ? speed : -speed;

	frameCount += v;
	// 正側
	if(frameCount >= FPS / abs(this->speed))
	{
		frameCount -= FPS / abs(this->speed);
		timeCount += v;
		if(timeCount >= MPH*HOUR_CYCLE)
			timeCount -= timeCount / (MPH*HOUR_CYCLE)*(MPH*HOUR_CYCLE);
	}
	// 逆側
	else if(frameCount < 0)
	{
		frameCount += FPS / abs(this->speed);
		timeCount += v;
		if(timeCount < 0)
			timeCount += timeCount / (MPH*HOUR_CYCLE)*(MPH*HOUR_CYCLE);
	}
}

inline void Clock::Update_Check()
{
	Update_Time(50);
	// 分単位の角度+秒単位の角度
	minuteAngle = (timeCount % MPH * ANGLE_1MINUTE) + (frameCount*(ANGLE_1MINUTE / (FPS / abs(speed))));
	// 時間単位+分単位+秒単位
	hourAngle = (timeCount / MPH * ANGLE_1HOUR) + (timeCount %MPH * (ANGLE_1HOUR / MPH)) + (frameCount*(ANGLE_1HOUR / MPH / (FPS / abs(speed))));
}

inline void Clock::Update_Smooth()
{
	Update_Time();
	// 分単位の角度+秒単位の角度
	minuteAngle = (timeCount % MPH * ANGLE_1MINUTE) + (frameCount*(ANGLE_1MINUTE / (FPS / abs(speed))));
	// 時間単位+分単位+秒単位
	hourAngle = (timeCount / MPH * ANGLE_1HOUR) + (timeCount %MPH * (ANGLE_1HOUR / MPH)) + (frameCount*(ANGLE_1HOUR / MPH / (FPS / abs(speed))));
}

inline void Clock::Update_Steping()
{
	Update_Time();
	minuteAngle = timeCount % MPH * ANGLE_1MINUTE;
	hourAngle = (timeCount / MPH * ANGLE_1HOUR) + (timeCount % MPH * (ANGLE_1HOUR / MPH));
}


//	コンストラクタ
//Clock::Clock(void) :TimeObject(256, 256)
//{
//
//}
//
////	デストラクタ
//Clock::~Clock(void)
//{
//	delete	m_Clock;	m_Clock = NULL;
//}
//
////	初期化
//bool		Clock::Initialize(void)
//{
//	//	時計画像
//	m_Clock = new iex2DObj("DATA/timer.png");
//
//	//	変数初期化
//	org_speed = PI / 180;
//	selectNum = 0;
//	cul_speed = 0.0f;
//	clear = false;
//	t = 1.0f;
//	step = 0;
//
//	//	オブジェクト情報初期化
//	InitObj();
//
//	return true;
//}
//
////	オブジェクト初期化
//void	Clock::InitObj(void)
//{
//	//	元のオブジェクト
//	SetObj(0, 640, 360, 30.0f, 0.0f, 1.0f);
//
//	//	他のオブジェクト
//	SetObj(1, 940, -40, 120.0f, 0.0f, 2.0f);
//	SetObj(2, 340, 60, 300.0f, 0.0f, 0.5f);
//	SetObj(3, 540, -340, 240.0f, 0.0f, 1.0f);
//}
//
////	オブジェクト情報設定
//void	Clock::SetObj(int n, int x, int y, float hAngle, float mAngle, float s)
//{
//	obj[n].pos.x = x;
//	obj[n].pos.y = y;
//	obj[n].hourAngle = PI / 180.0f * hAngle;
//	obj[n].minuteAngle = PI / 180.0f * mAngle;
//	obj[n].speed = s;
//
//	//	初期位置保存
//	obj[n].start = obj[n].pos;
//	obj[n].end = obj[n].pos;
//	obj[n].shift.x = 0;
//	obj[n].shift.y = 0;
//}
//
////----------------------------------------------------------------------------
////	更新
////----------------------------------------------------------------------------
//
////	更新
//void	Clock::Update(void)
//{
//	//	動作
//	Move();
//
//	switch (step)
//	{
//		case 0:		//	ズームアウト
//			m_Camera.param += PI / 180 * CameraInfo::ZOOM_SPEED;
//			m_Camera.scale = (int)(CameraInfo::ZOOM_MAX * sinf(m_Camera.param));
//			CalcPos();	//	ずれ計算
//			if (m_Camera.scale <= 0)
//			{
//				m_Camera.param = 0.0f;
//				m_Camera.scale = 0;
//				step++;
//			}
//			break;
//
//		case 1:		//	ゲーム開始
//			Control();
//			break;
//	}
//
//	//	パラメータ加算
//	t += 0.01f;
//	if (t >= 1.0f)	t = 1.0f;
//}
//
////----------------------------------------------------------------------------
////	描画
////----------------------------------------------------------------------------
//
////	描画
//void	Clock::Render(void)
//{
//	//	変数準備
//	DWORD	color[4] = {0xFFFF7777, 0xFF77FF77, 0xFF7777FF, 0xFFFF77FF};
//	int	x, y, w, h, sx, sy, sw, sh;
//	POINT	p;
//
//	for (int i = 0; i < CLOCK_MAX; i++)
//	{
//		//	時計描画
//		x = obj[i].pos.x - 150 - m_Camera.scale / 2 + obj[i].shift.x / 2;
//		y = obj[i].pos.y - 150 - m_Camera.scale / 2 + obj[i].shift.y / 2;
//		p.x = obj[i].pos.x + obj[i].shift.x;
//		p.y = obj[i].pos.y + obj[i].shift.y;
//		w = h = 300 + m_Camera.scale;
//		sx = sy = 0;
//		sw = sh = 256;
//		m_Clock->Render(x, y, w, h, sx, sy, sw, sh, RS_COPY, color[i]);
//
//		//	分針描画
//		sx = 256;
//		m_Clock->Render(x, y, w, h, sx, sy, sw, sh, p, obj[i].minuteAngle);
//
//		//	時針描画
//		sx = 0;
//		sy = 256;
//		m_Clock->Render(x, y, w, h, sx, sy, sw, sh, p, obj[i].hourAngle);
//
//		//	スピード表示
//		x = obj[i].pos.x + 100;
//		y = obj[i].pos.y + 100;
//	}
//}
//
////----------------------------------------------------------------------------
////	動作関数
////----------------------------------------------------------------------------
//
////　範囲内にあるか調べる
//bool	Clock::CheckWithin(const POINT& p, const int& startX, const int& startY, const int size)
//{
//	POINT	cursorPos = p;
//
//	//	カーソル座標設定
//	GetCursorPos(&cursorPos);
//	ScreenToClient(iexSystem::Window, &cursorPos);
//
//	if ((startX < cursorPos.x) && (cursorPos.x < startX + size))
//	{
//		if ((startY < cursorPos.y) && (cursorPos.y < startY + size))
//			return true;
//	}
//	return false;
//}
//
////	POINT補間
//void	Clock::Lerp(POINT& out, const POINT p1, const POINT p2, float t)
//{
//	float rate = t * t * (3.0f - 2.0f * t);   // 3次関数補間値に変換
//
//	out.x = (long)(p1.x * (1.0f - rate) + p2.x * rate);
//	out.y = (long)(p1.y * (1.0f - rate) + p2.y * rate);
//}
//
////	動作
//void	Clock::Move(void)
//{
//	//	時計の針の回転
//	for (int i = 0; i < CLOCK_MAX; i++)
//	{
//		//	選択中のオブジェクト
//		if (i == selectNum)
//		{
//			//	中心からの距離を求める
//			length.x = 640 - obj[i].start.x;
//			length.y = 360 - obj[i].start.y;
//
//			//	最終の位置を求める
//			obj[i].end.x = obj[i].start.x + length.x;
//			obj[i].end.y = obj[i].start.y + length.y;
//
//			//	中心に移動( 移動補間 )
//			Lerp(obj[i].pos, obj[i].start, obj[i].end, t);
//
//			//	選択中のオブジェクトのスピードを保存
//			cul_speed = obj[i].speed;
//
//			//	元のスピードで回転
//			obj[i].minuteAngle += org_speed;
//			obj[i].hourAngle += org_speed / 12.0f;
//		}
//		else
//		{
//			//	最終の位置を求める
//			obj[i].end.x = obj[i].start.x + length.x;
//			obj[i].end.y = obj[i].start.y + length.y;
//
//			//	移動
//			Lerp(obj[i].pos, obj[i].start, obj[i].end, t);
//
//			//	回転
//			obj[i].minuteAngle += org_speed * (obj[i].speed / cul_speed);
//			obj[i].hourAngle += (org_speed / 12.0f) * (obj[i].speed / cul_speed);
//		}
//	}
//}
//
////	コントロール
//void	Clock::Control(void)
//{
//	//	エンターで選択
//	if (KEY(KEY_ENTER) == 3)
//	if (obj[selectNum].speed == obj[0].speed && selectNum != 0)
//		clear = true;
//
//	//	マウスクリックチェック
//	for (int n = 0; n < 4; n++)
//	{
//		//	マウスカーソルが範囲内だったら
//		if (CheckWithin(Mouse::cursor, obj[n].pos.x - 150, obj[n].pos.y - 150, GameInfo::WNDSIZE))
//		{
//			//	クリック判定
//			if (Mouse::Click())
//			{
//				//	移動が終了しているかチェック
//				if (t < 1.0f)		return;
//
//				//	選択中のオブジェクトじゃなかったら
//				if (selectNum != n)
//				{
//					//	選択したオブジェクトの番号
//					selectNum = n;
//
//					//	元の位置保存
//					for (int i = 0; i < CLOCK_MAX; i++)
//						obj[i].start = obj[i].pos;
//
//					//	パラメータ設定
//					t = 0.0f;
//				}
//			}
//		}
//	}
//}
//
////	位置関係計算
//void	Clock::CalcPos(void)
//{
//	float cx, cy, vx, vy;
//	cx = 640;
//	cy = 360;
//	float	l;
//
//	for (int i = 0; i < CLOCK_MAX; i++)
//	{
//		vx = obj[i].pos.x - cx;
//		vy = obj[i].pos.y - cy;
//
//		l = sqrt(vx * vx + vy *vy);
//
//		vx /= l;
//		vy /= l;
//
//		if (selectNum != i)
//		{
//			obj[i].shift.x = (int)(vx * m_Camera.scale * 2);
//			obj[i].shift.y = (int)(vy * m_Camera.scale * 2);
//		}
//	}
//}
//
//void Clock::Check()
//{
//
//	//移動終了
//	if (t >= 1.0f)
//	{
//		t = 0.0f;
//
//		//判定
//		switch (obj[selectNum].flag)
//		{
//			case FLAG_RED:
//				if (obj[selectNum].speed == 1.0f)
//				{
//					obj[selectNum].state = STATE_SUCCESS;
//					obj[selectNum].SuccessFlg = true;
//				}
//				else
//				{
//					obj[selectNum].state = STATE_BERAK;
//					ShakeFlg = true;
//				}
//				break;
//			case FLAG_GREEN:
//				if (obj[selectNum].speed == 2.0f)
//				{
//					obj[selectNum].state = STATE_SUCCESS;
//					obj[selectNum].SuccessFlg = true;
//				}
//				else
//				{
//					obj[selectNum].state = STATE_BERAK;
//					ShakeFlg = true;
//				}
//				break;
//			case FLAG_BLUE:
//				if (obj[selectNum].speed == 0.5f)
//				{
//					obj[selectNum].state = STATE_SUCCESS;
//					obj[selectNum].SuccessFlg = true;
//				}
//				else
//				{
//					obj[selectNum].state = STATE_BERAK;
//					ShakeFlg = true;
//				}
//				break;
//		}
//		//位置保存
//		for (int i = 0; i < 4; i++)
//		{
//			obj[i].start = obj[i].pos;
//		}
//		//次の時計へ
//		for (int i = selectNum + 1; i < 4; i++)
//		{
//			//フラグがまだ立っていなくて、成功していないもの
//			if (obj[i].flag&&obj[i].state != STATE_SUCCESS)
//			{
//				selectNum = i;
//				goto Check_found;
//			}
//		}
//
//		//見つからなければ終了
//
//		CheckFlg = false;
//		return;
//
//		//見つかっていれば次の移動
//	Check_found:;
//	}
//}
//
//void Clock::ClearCheck()
//{
//
//	for (int i = 0; i < 4; i++)
//	{
//		if (obj[i].state != STATE_SUCCESS)return;
//	}
//	clear = true;
//}
//
//void Clock::Shake()
//{
//	std::uniform_real_distribution<float> rad(0.0f, 1.0f);
//
//	std::mt19937 mtRand{std::random_device()()};
//
//	ShakeVar.x = rad(mtRand)*SHAKE_POWER - SHAKE_POWER / 2;
//	ShakeVar.y = rad(mtRand)*SHAKE_POWER - SHAKE_POWER / 2;
//}
//
//void Clock::FlagUpdate()
//{
//	HaveFlag[FLAG_NOT] = DefaultHaveFlag[FLAG_NOT];
//	HaveFlag[FLAG_RED] = DefaultHaveFlag[FLAG_RED];
//	HaveFlag[FLAG_GREEN] = DefaultHaveFlag[FLAG_GREEN];
//	HaveFlag[FLAG_BLUE] = DefaultHaveFlag[FLAG_BLUE];
//	for (int i = 0; i < 4; i++)
//	{
//		switch (obj[i].flag)
//		{
//			case FLAG_NOT:
//				HaveFlag[FLAG_NOT]--;
//				break;
//			case FLAG_RED:
//				HaveFlag[FLAG_RED]--;
//				break;
//			case FLAG_GREEN:
//				HaveFlag[FLAG_GREEN]--;
//				break;
//			case FLAG_BLUE:
//				HaveFlag[FLAG_BLUE]--;
//				break;
//		}
//	}
//}