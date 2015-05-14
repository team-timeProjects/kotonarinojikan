
#ifndef __GAME_H__
#define __GAME_H__

//*****************************************************************************************************
//
//	Gameクラス
//
//*****************************************************************************************************

//----------------------------------------------------------------------------------
//	クラス前方宣言
//----------------------------------------------------------------------------------
	class		Clock;
	class		Metronom;
	class		Candle;

//----------------------------------------------------------------------------------
//	定数宣言
//----------------------------------------------------------------------------------
	namespace GameInfo
	{
		//	画像幅
		const	int		WNDSIZE = 300;
	}
	namespace CameraInfo
	{
		const	int		ZOOM_MAX = 850;
		const	float		ZOOM_SPEED = 2.0f;
	}

//----------------------------------------------------------------------------------
//	構造体
//----------------------------------------------------------------------------------
	struct CAMERA
	{
		POINT	c_pos;
		POINT	length;
		float		param;
		int		scale;
	};
	struct CLOCK_OBJ	//	とりあえず時計の置き物
	{
		POINT		pos;			//	位置
		POINT		start;			//	元の位置
		POINT		end;			//	移動後の位置
		POINT		shift;			//	カメラ用ずれ幅
		float		minuteAngle;	//	分針の向き
		float		hourAngle;		//	時針の向き
		float		speed;			//	速度
	};
	struct METRONOM
	{
		POINT		pos;			//	位置
		POINT		start;		//	元の位置
		POINT		end;			//	移動後の位置
		POINT		shift;		//	カメラ用ずれ幅
		float			speed;		//	速度
		float			height;		//	高さ
		float			angle;		//	向き
	};
	struct CANDLE
	{
		POINT		pos;			//	位置
		POINT		start;		//	元の位置
		POINT		end;			//	移動後の位置
		POINT		shift;		//	カメラ用ずれ幅
		float			speed;		//	速度
		float			height;		//	高さ
	};

//----------------------------------------------------------------------------------
//	extern宣言
//----------------------------------------------------------------------------------
	extern	CAMERA	m_Camera;

//----------------------------------------------------------------------------------
//	Gameクラス
//----------------------------------------------------------------------------------
class Game
{
public:

private:

private:
	//	オブジェクト
	iex2DObj*	m_BG;		//	背景
	iex2DObj*	m_Clock;	//	時計画像
	
	//	パラメータ
	Clock*				m_GameClock;		//	時計選択時の動作
	Metronom*		m_Metronom;			//	メトロノーム選択時の動作
	Candle*			m_Candle;				//	ろうそく選択時の動作
	
	int			m_State;		//	シーン遷移

public:
	//	初期化・解放
	Game( void );
	~Game( void );
	
	//　初期化・読み込み
	bool	Initialize( void );
	void	MainInitialize( void );
	void	GameInitialize( void );
	
	//　更新
	void	Update( void );
	void	GameUpdate( void );

	//		描画
	void	Render( void );
	void	GameRender( void );

	//　MAIN_STATE_GAME動作関数
	bool	CheckWithin( const POINT& p, const int& startX, const int& startY, const int size );	//	範囲内かどうか調べる
	void	Lerp( POINT& out, const POINT p1, const POINT p2, float t );	//	POINT補間
};

//*****************************************************************************************************
#endif