
#ifndef		__CLOCK_H__
#define	__CLOCK_H__

//****************************************************************************************
//
//	Clockクラス
//
//****************************************************************************************
class Clock
{
private:
	struct CLOCK_OBJ	//	とりあえず時計の置き物
	{
		POINT		pos;			//	位置
		POINT		start;			//	元の位置
		POINT		end;			//	移動後の位置
		POINT		shift;			//	カメラ用ずれ幅
		float		speed;			//	速度

		float		minuteAngle;	//	分針の向き
		float		hourAngle;		//	時針の向き
	};

private:
	//	パラメータ
	static	const	int		CLOCK_MAX = 4;
	CLOCK_OBJ	obj[4];		//	時計構造体
	POINT		length;		//	距離
	float		org_speed;	//	元のスピード
	float		cul_speed;	//	現在選択中のそもそものスピード
	int			selectNum;	//	選択中の番号
	bool		clear;		//	クリア
	float		t;			//	割合
	int			step;		//	ステップ

private:
	//	オブジェクト
	iex2DObj*	m_Clock;	//	時計画像

	//	パラメータ
	int			m_State;		//	シーン遷移

public:
	//	初期化・解放
	Clock(void);
	~Clock(void);

	//　初期化・読み込み
	bool	Initialize(void);
	void	InitObj(void);		//	オブジェクト初期化
	void	SetObj(int n, int x, int y, float hAngle, float mAngle, float s);

	//　更新
	void	Update(void);

	//	描画
	void	Render(void);

	//　MAIN_STATE_GAME動作関数
	void	Move(void);		//	動作
	bool	CheckWithin(const POINT& p, const int& startX, const int& startY, const int size);	//	範囲内かどうか調べる
	void	Lerp(POINT& out, const POINT p1, const POINT p2, float t);	//	POINT補間
	void	Control(void);	//	操作
	void	CalcPos(void);	//	位置関係計算

	//	情報取得・設定
	void	GetTitleInfo(float hAngle, float mAngle);
};

//****************************************************************************************
#endif // !__CLOCK_H__
