
#include	"iextreme.h"
#include	"Control.h"
#include	"Clock.h"
#include	"Metronom.h"
#include	"Candle.h"

#include	"Game.h"

//*****************************************************************************************************
//
//	Gameクラス
//
//*****************************************************************************************************

CAMERA	m_Camera;

//----------------------------------------------------------------------------
//	定数宣言
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//	初期化・解放
//----------------------------------------------------------------------------

	//	コンストラクタ
	Game::Game( void )
	{
		
	}

	//	デストラクタ
	Game::~Game( void )
	{
		delete	m_BG;		m_BG		=	NULL;
		delete	m_Clock;	m_Clock		=	NULL;
		delete	m_GameClock;	m_GameClock = NULL;
	}

	//	初期化
	bool	Game::Initialize( void )
	{
		//	背景設定
		m_BG = new iex2DObj( "DATA/BG/和紙.png" );

		//	時計画像
		m_Clock = new iex2DObj( "DATA/timer.png" );

		m_GameClock = new Clock();
		m_Metronom = new Metronom();
		m_Candle = new Candle();

		//	シーン設定
		m_State = MAIN_STATE_INIT;

		return true;
	}

	//	全体初期化
	void	Game::MainInitialize( void )
	{
		//	カメラ初期化
		m_Camera.c_pos.x	=	640;
		m_Camera.c_pos.y	=	360;
		m_Camera.length.x	=	0;
		m_Camera.length.y	=	0;
		m_Camera.scale		=	0;
		m_Camera.param		=	0.0f;

		TitleInitialize();
		GameInitialize();

		//	タイトルへ
		m_State = MAIN_STATE_GAME;
	}

	//	タイトル初期化
	void	Game::TitleInitialize( void )
	{
		m_Title.obj.pos.x		=	640;
		m_Title.obj.pos.y		=	360;
		m_Title.obj.hourAngle	=	PI / 180.0f * 30.0f;
		m_Title.obj.minuteAngle =	0.0f;
		m_Title.obj.start		=	m_Title.obj.pos;
		m_Title.obj.end			=	m_Title.obj.pos;
		m_Title.step			=	0;
		m_Title.t				=	0.0f;
	}

	//	ゲーム初期化
	void	Game::GameInitialize( void )
	{
		m_GameClock->Initialize();
		m_Metronom->Initialize();
		m_Candle->Initialize();
	}

//----------------------------------------------------------------------------
//	更新
//----------------------------------------------------------------------------
	
	//	更新
	void	Game::Update( void )
	{
		switch ( m_State )
		{
		case MAIN_STATE_INIT:	//	breakせずそのまま下へ
			MainInitialize();

		case MAIN_STATE_TITLE:	//	タイトル
			TitleUpdate();
			break;

		case MAIN_STATE_GAME:	//	ゲーム本編
			GameUpdate();
			break;
		}
	}

	//	タイトル更新
	void	Game::TitleUpdate( void )
	{		
		//	常に回転
		m_Title.obj.minuteAngle +=	PI / 180 * 1.0f;
		m_Title.obj.hourAngle	+= ( PI / 180 ) / 12.0f;

		//	ゲーム本体と同期
		m_GameClock->GetTitleInfo( m_Title.obj.hourAngle, m_Title.obj.minuteAngle );

		switch ( m_Title.step )
		{
		case 0:		//	選択
			if ( CheckWithin( Mouse::cursor, m_Title.obj.pos.x - 150, m_Title.obj.pos.y - 150, GameInfo::WNDSIZE ) )
			{
				if ( Mouse::Click() )
				{
					m_Title.step++;
				}
			}
			break;

		case 1:		//	ズームアップ
			m_Camera.param += PI / 180 * CameraInfo::ZOOM_SPEED;
			m_Camera.scale = ( int )( CameraInfo::ZOOM_MAX * sinf( m_Camera.param ) );

			//	一定以上近づいたら
			if ( m_Camera.scale >= CameraInfo::ZOOM_MAX )
			{
				m_Camera.param = PI / 2;
				m_Camera.scale = CameraInfo::ZOOM_MAX;
				m_State = MAIN_STATE_GAME;

				//	切り替わる前にズレを計算しておく
				m_GameClock->CalcPos();
			}
			break;
		}
	}

	//	ゲーム本編更新
	void	Game::GameUpdate( void )
	{
		//m_GameClock->Update();
		m_Metronom->Update();
		//m_Candle->Update();
	}

//----------------------------------------------------------------------------
//	描画
//----------------------------------------------------------------------------
	
	//	描画
	void	Game::Render( void )
	{
		//	背景描画
		//m_BG->Render( 0, 0, 1280, 720, 0, 0, 2048, 1024 );

		switch ( m_State )
		{
		case MAIN_STATE_INIT:
			break;

		case MAIN_STATE_TITLE:
			TitleRender();
			break;

		case MAIN_STATE_GAME:
			GameRender();
			break;
		}
	}

	//	タイトル描画
	void	Game::TitleRender( void )
	{
		int	x, y, w, h, sx, sy, sw, sh;

		//	時計描画
		x = m_Title.obj.pos.x - 150 - m_Camera.scale / 2;
		y = m_Title.obj.pos.y - 150 - m_Camera.scale / 2;
		w = h = 300 + m_Camera.scale;
		sx = sy = 0;
		sw = sh = 256;
		m_Clock->Render( x, y, w, h, sx, sy, sw, sh, RS_COPY, 0xFFFF7777 );

		//	分針描画
		sx = 256;
		m_Clock->Render( x, y, w, h, sx, sy, sw, sh, m_Title.obj.pos, m_Title.obj.minuteAngle );

		//	時針描画
		sx = 0;
		sy = 256;
		m_Clock->Render( x, y, w, h, sx, sy, sw, sh, m_Title.obj.pos, m_Title.obj.hourAngle );
	}

	//	ゲーム本編描画
	void	Game::GameRender( void )
	{
		//m_GameClock->Render();
		m_Metronom->Render();
		//m_Candle->Render();
	}

//----------------------------------------------------------------------------
//	動作関数
//----------------------------------------------------------------------------
	
	//　範囲内にあるか調べる
	bool	Game::CheckWithin( const POINT& p, const int& startX, const int& startY, const int size )
	{
		POINT	cursorPos = p;

		//	カーソル座標設定
		GetCursorPos( &cursorPos );
		ScreenToClient( iexSystem::Window, &cursorPos );

		if ( ( startX < cursorPos.x ) && ( cursorPos.x < startX + size ) )
		{
			if ( ( startY < cursorPos.y ) && ( cursorPos.y < startY + size ) )
				return true;
		}
		return false;
	}

	//	POINT補間
	void	Game::Lerp( POINT& out, const POINT p1, const POINT p2, float t )
	{
		float rate = t * t * ( 3.0f - 2.0f * t );   // 3次関数補間値に変換

		out.x = ( long )( p1.x * ( 1.0f - rate ) + p2.x * rate );
		out.y = ( long )( p1.y * ( 1.0f - rate ) + p2.y * rate );
	}

