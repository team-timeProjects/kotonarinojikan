
#include	"iextreme.h"
#include	"Control.h"
#include	"Clock.h"
#include	"Metronom.h"
#include	"Candle.h"

#include	"Game.h"

//*****************************************************************************************************
////
////	Gameクラス
////
////*****************************************************************************************************
//
//CAMERA	m_Camera;
//
////----------------------------------------------------------------------------
////	定数宣言
////----------------------------------------------------------------------------
//
////----------------------------------------------------------------------------
////	初期化・解放
////----------------------------------------------------------------------------
//
//	//	コンストラクタ
//	Game::Game( void )
//	{
//		
//	}
//
//	//	デストラクタ
//	Game::~Game( void )
//	{
//		delete	m_GameClock; m_GameClock = NULL;
//		delete	m_Metronom;	 m_Metronom = NULL;
//		delete	m_Candle;	 m_Candle = NULL;
//	}
//
//	//	初期化
//	bool	Game::Initialize( void )
//	{
//		m_GameClock = new Clock();
//		m_Metronom = new Metronom();
//		m_Candle = new Candle();
//
//		//	カメラ初期化
//		m_Camera.c_pos.x = 640;
//		m_Camera.c_pos.y = 360;
//		m_Camera.length.x = 0;
//		m_Camera.length.y = 0;
//		m_Camera.scale = 0;
//		m_Camera.param = 0.0f;
//
//		GameInitialize();
//
//		return true;
//	}
//
//	//	ゲーム初期化
//	void	Game::GameInitialize( void )
//	{
//		m_GameClock->Initialize();
//		m_Metronom->Initialize();
//		m_Candle->Initialize();
//	}
//
////----------------------------------------------------------------------------
////	更新
////----------------------------------------------------------------------------
//	
//	//	更新
//	void	Game::Update( void )
//	{
//		GameUpdate();
//	}
//
//	//	ゲーム本編更新
//	void	Game::GameUpdate( void )
//	{
//		m_GameClock->Update();
//		//m_Metronom->Update();
//		//m_Candle->Update();
//	}
//
////----------------------------------------------------------------------------
////	描画
////----------------------------------------------------------------------------
//	
//	//	描画
//	void	Game::Render( void )
//	{
//		GameRender();
//	}
//
//	//	ゲーム本編描画
//	void	Game::GameRender( void )
//	{
//		//m_GameClock->Render();
//		m_Metronom->Render();
//		//m_Candle->Render();
//	}
//
////----------------------------------------------------------------------------
////	動作関数
////----------------------------------------------------------------------------
//	
//	//　範囲内にあるか調べる
//	bool	Game::CheckWithin( const POINT& p, const int& startX, const int& startY, const int size )
//	{
//		POINT	cursorPos = p;
//
//		//	カーソル座標設定
//		GetCursorPos( &cursorPos );
//		ScreenToClient( iexSystem::Window, &cursorPos );
//
//		if ( ( startX < cursorPos.x ) && ( cursorPos.x < startX + size ) )
//		{
//			if ( ( startY < cursorPos.y ) && ( cursorPos.y < startY + size ) )
//				return true;
//		}
//		return false;
//	}
//
//	//	POINT補間
//	void	Game::Lerp( POINT& out, const POINT p1, const POINT p2, float t )
//	{
//		float rate = t * t * ( 3.0f - 2.0f * t );   // 3次関数補間値に変換
//
//		out.x = ( long )( p1.x * ( 1.0f - rate ) + p2.x * rate );
//		out.y = ( long )( p1.y * ( 1.0f - rate ) + p2.y * rate );
//	}

