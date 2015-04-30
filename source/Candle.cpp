
#include	"iextreme.h"
#include	"Game.h"
#include	"Control.h"

#include	"Candle.h"

//****************************************************************************************
//
//	Candleクラス
//
//****************************************************************************************

//--------------------------------------------------------------------------------------------
//	初期化・解放
//--------------------------------------------------------------------------------------------

	//	コンストラクタ
	Candle::Candle( void )
	{
	
	}

	//	デストラクタ
	Candle::~Candle( void )
	{
		delete	candle;			candle = NULL;
		delete	fire;					fire = NULL;
		delete	melt_candle;	melt_candle = NULL;
		delete	light;				light = NULL;
	}

	//	初期化
	void	Candle::Initialize( void )
	{
		candle = new iex2DObj( "DATA/ろうそく.png" );
		fire = new iex2DObj( "DATA/ろうそく(火).png" );
		melt_candle = new iex2DObj( "DATA/ろうそく(溶け).png" );
		light = new iex2DObj( "DATA/ろうそく（明かり）.png" );

		//	変数初期化
		org_speed = PI / 180;
		selectNum = 0;
		cul_speed = 0.0f;
		clear = false;
		t = 0.0f;
		step = 0;

		//	元のオブジェクト
		SetObj( 0, 640, 360, 1.0f );

		//	他のオブジェクト
		SetObj( 1, 940, -40, 0.1f );
		SetObj( 2, 340, 60, 0.5f );
		SetObj( 3, 540, -340, 1.0f );
	}

	//	オブジェクト設定
	void	Candle::SetObj( int n, int x, int y, float height )
	{
		obj[n].pos.x = x;
		obj[n].pos.y = y;
		obj[n].height = height;
		obj[n].shift.x = 0;
		obj[n].shift.y = 0;
		obj[n].start = obj[n].pos;
		obj[n].end = obj[n].pos;
	}

//--------------------------------------------------------------------------------------------
//	更新・描画
//--------------------------------------------------------------------------------------------

	//	更新
	void	Candle::Update( void )
	{
		//	動作
		Move();

		//	操作
		Control();

		//	パラメータ加算
		t += 0.01f;
		if ( t >= 1.0f )		t = 1.0f;
	}

	//	描画
	void	Candle::Render( void )
	{
		int	x, y, w, h, sx, sy, sw, sh;

		for ( int i = 0; i < CANDLE_MAX; i++ )
		{
			x = obj[i].pos.x - GameInfo::WNDSIZE / 2 + obj[i].shift.x / 2;
			y = obj[i].pos.y - GameInfo::WNDSIZE / 2 + obj[i].shift.y / 2;
			w = GameInfo::WNDSIZE + obj[i].shift.x;
			h = GameInfo::WNDSIZE + obj[i].shift.y;
			sx = sy = 0;
			sw = sh = 512;
			candle->Render( x, y, w, h, sx, sy, sw, sh );
			light->Render( x, y - 120, w, h, sx, sy, sw, sh );
			fire->Render( x, y - 120, w, h, sx, sy, sw, sh );
			melt_candle->Render( x, y, w, h, sx, sy, sw, sh );
			
		}
	}

//--------------------------------------------------------------------------------------------
//	動作関数
//--------------------------------------------------------------------------------------------

	//	動作
	void	Candle::Move( void )
	{
		//	時計の針の回転
		for ( int i = 0; i < CANDLE_MAX; i++ )
		{
			//	選択中のオブジェクト
			if ( i == selectNum )
			{
				//	中心からの距離を求める
				length.x = 640 - obj[i].start.x;
				length.y = 360 - obj[i].start.y;

				//	最終の位置を求める
				obj[i].end.x = obj[i].start.x + length.x;
				obj[i].end.y = obj[i].start.y + length.y;

				//	中心に移動( 移動補間 )
				Lerp( obj[i].pos, obj[i].start, obj[i].end, t );

				//	選択中のオブジェクトのスピードを保存
				cul_speed = obj[i].speed;
			}
			else
			{
				//	最終の位置を求める
				obj[i].end.x = obj[i].start.x + length.x;
				obj[i].end.y = obj[i].start.y + length.y;

				//	移動
				Lerp( obj[i].pos, obj[i].start, obj[i].end, t );
			}
		}
	}

	//　範囲内にあるか調べる
	bool	Candle::CheckWithin( const POINT& p, const int& startX, const int& startY, const int size )
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
	void	Candle::Lerp( POINT& out, const POINT p1, const POINT p2, float t )
	{
		float rate = t * t * ( 3.0f - 2.0f * t );   // 3次関数補間値に変換

		out.x = ( long )( p1.x * ( 1.0f - rate ) + p2.x * rate );
		out.y = ( long )( p1.y * ( 1.0f - rate ) + p2.y * rate );
	}

	//	コントロール
	void	Candle::Control( void )
	{
		//	エンターで選択
		if ( KEY( KEY_ENTER ) == 3 )
		if ( obj[selectNum].speed == obj[0].speed && selectNum != 0 )
			clear = true;

		//	マウスクリックチェック
		for ( int n = 0; n < CANDLE_MAX; n++ )
		{
			//	マウスカーソルが範囲内だったら
			if ( CheckWithin( Mouse::cursor, obj[n].pos.x - 150, obj[n].pos.y - 150, GameInfo::WNDSIZE ) )
			{
				//	クリック判定
				if ( Mouse::Click() )
				{
					//	移動が終了しているかチェック
					if ( t < 1.0f )		return;

					//	選択中のオブジェクトじゃなかったら
					if ( selectNum != n )
					{
						//	選択したオブジェクトの番号
						selectNum = n;

						//	元の位置保存
						for ( int i = 0; i < CANDLE_MAX; i++ )
							obj[i].start = obj[i].pos;

						//	パラメータ設定
						t = 0.0f;
					}
				}
			}
		}
	}

	//	位置関係計算
	void	Candle::CalcPos( void )
	{
		float cx, cy, vx, vy;
		cx = 640;
		cy = 360;
		float	l;

		for ( int i = 0; i < CANDLE_MAX; i++ )
		{
			vx = obj[i].pos.x - cx;
			vy = obj[i].pos.y - cy;

			l = sqrt( vx * vx + vy * vy );

			vx /= l;
			vy /= l;

			if ( selectNum != i )
			{
				obj[i].shift.x = ( int )( vx * m_Camera.scale * 2 );
				obj[i].shift.y = ( int )( vy * m_Camera.scale * 2 );
			}
		}
	}

	//-----------------------------------------------------------------------------------------------
//	情報取得・設定
//-----------------------------------------------------------------------------------------------

	//	タイトルから情報取得
	void	Candle::SetTitleInfo( float height )
	{
		obj[0].height = height;
	}