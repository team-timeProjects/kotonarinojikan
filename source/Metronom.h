
#ifndef		__METRONOM_H__
#define	__METRONOM_H__

//****************************************************************************************
//
//		Metronomクラス
//
//****************************************************************************************
class Metronom
{
private:
	struct METRONOM
	{
		POINT		pos;			//	位置
		POINT		start;		//	元の位置
		POINT		end;			//	移動後の位置
		POINT		shift;		//	カメラ用ずれ幅
		float			speed;		//	速度
		float			param;		//	回転用
		float			height;		//	高さ
		float			angle;		//	向き
	};

private:
	//	オブジェクト情報
	static	const	int	METRONOM_MAX = 4;
	METRONOM		obj[4];
	iex2DObj*		back;
	iex2DObj*		metronom;
	iex2DObj*		needle;
	
	//	パラメータ
	POINT		length;			//	距離
	float			org_speed;	//	元のスピード
	float			cul_speed;	//	現在選択中のそもそものスピード
	int			selectNum;	//	選択中の番号
	bool			clear;			//	クリア
	float			t;					//	割合
	int			step;			//	ステップ
	float			plus_speed;	//	スピード加算値

public:
	//	初期化・解放
	Metronom( void );
	~Metronom( void );
	void	Initialize( void );
	void	SetObj( int n, int x, int y, float height, float speed );

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
	void	SetTitleInfo( float angle );
};

//****************************************************************************************
#endif // !__METRONOM_H__
