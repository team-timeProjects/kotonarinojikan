
#ifndef __CANDLE_H__
#define	__CANDLE_H__

//****************************************************************************************
//
//	Candleクラス
//
//****************************************************************************************
class Candle
{
private:
	struct CANDLE
	{
		POINT		pos;			//	位置
		POINT		start;		//	元の位置
		POINT		end;			//	移動後の位置
		POINT		shift;		//	カメラ用ずれ幅
		float			speed;		//	速度
		float			height;		//	高さ
	};

private:
	//	オブジェクト情報
	static	const	int	CANDLE_MAX = 4;
	CANDLE		obj[4];
	iex2DObj*		candle;
	iex2DObj*		fire;
	iex2DObj*		melt_candle;
	iex2DObj*		light;

	//	パラメータ
	POINT		length;			//	距離
	float			org_speed;	//	元のスピード
	float			cul_speed;	//	現在選択中のそもそものスピード
	int			selectNum;	//	選択中の番号
	bool			clear;			//	クリア
	float			t;					//	割合
	int			step;			//	ステップ

public:
	//	初期化・解放
	Candle( void );
	~Candle( void );
	void	Initialize( void );
	void	SetObj( int n, int x, int y, float height );

	//	更新
	void	Update( void );

	//	描画
	void	Render( void );

	//	動作関数
	void	Move( void );
	bool	CheckWithin( const POINT& p, const int& startX, const int& startY, const int size );	//	範囲内かどうか調べる
	void	Lerp( POINT& out, const POINT p1, const POINT p2, float t );	//	POINT補間
	void	Control( void );	//	操作
	void	CalcPos( void );	//	位置関係計算

	//	情報取得・設定
	void	SetTitleInfo( float height );
};

//****************************************************************************************
#endif // !__CANDLE_H__
