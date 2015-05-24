
#ifndef	__CLOCK_H__
#define	__CLOCK_H__

#include "TimeObject.h"
#include "Campus.h"

//****************************************************************************************
//
//	Clockクラス
//
//****************************************************************************************
//class Clock :public TimeObject
//{
//private:
//	//static	const	int		CLOCK_MAX = 4;
//	//Clock*			obj[CLOCK_MAX];
//
//	float		minuteAngle;	//	分針の向き
//	float		hourAngle;		//	時針の向き
//
//
//public:
//	//	初期化・解放
//	Clock(void);
//	~Clock(void);
//
//	//　初期化・読み込み
//	bool	Initialize(void);
//	void	InitObj(void);		//	オブジェクト初期化
//	void	SetObj(int n, int x, int y, float hAngle, float mAngle, float s);
//
//	//　更新
//	void	Update(void);
//
//	//	描画
//	void	Render(void);
//
//	void	Move(void);		//	動作
//
//	//-------------------------------------------------------
//	//----------------- develop:堀田 ------------------------
//	//-------------------------------------------------------
//
//	///<summary>引数pとの当たり判定</summary>
//	///<param name="p">オブジェクトのスクリーン座標</param>
//	bool CheckWithin(const POINT& p)
//	{
//		return CheckWithin(Campus::Inst()->TransCampusPos(p), obj[0]->pos.x - 150, obj[0]->pos.y - 150, 300);
//	}
//
//	///<summary>時計の座標取得</summary>
//	///<return>時計の中心座標</return>
//	POINT& GetPosCC()
//	{
//		return obj[0]->pos;
//	}
//
//	//-------------------------------------------------------
//	//----------------ここまで　develop:堀田-----------------
//	//-------------------------------------------------------
//
//};

class Clock :public TimeObj
{
	//----------- field --------------
public:
	enum Behavior
	{
		STOP=-2, CHECK,
		SMOOTH=0, STEPING
	};
	enum Image
	{
		BACK, HOUR, MINUTE
	};
private:
	static const int FPS = 60;			// frame per second
	static const int MPH = 60;			// minute per hour
	static const int HOUR_CYCLE = 12;	// 一周の時間数
	static const float ANGLE_1MINUTE;	// 一分の角度
	static const float ANGLE_1HOUR;		// 一時間の角度
	int timeCount = 0;					// 秒カウント
	int frameCount = 0;					// フレームカウント
	float hourAngle = 0;				// 短針の角度
	float minuteAngle = 0;				// 長針の角度

	//---------- method -------------
public:
	Clock();
	void Init(const Vector2& centerPos, int colW, int colH, float scale, float speed, Behavior behavior);
	void Update()override;
	void Render()override;
private:
	void Update_Time();
	void Update_Check();
	void Update_Smooth();
	void Update_Steping();
};

//****************************************************************************************
#endif // !__CLOCK_H__
