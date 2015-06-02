
#include	"iextreme.h"
#include	"Control.h"
#include	"Game.h"

#include	"Clock.h"

//****************************************************************************************
//
//	Clockクラス
//
//****************************************************************************************

//----------------------------------------------------------------------------
//	定数宣言
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//	初期化・解放
//----------------------------------------------------------------------------

	//	コンストラクタ
	Clock::Clock( void ) : m_Clock( NULL )
	{

	}

	//	デストラクタ
	Clock::~Clock( void )
	{
		delete	m_Clock;	m_Clock = NULL;
	}

	//	初期化
	bool	Clock::Initialize( void )
	{
		//	時計画像
		m_Clock			=		new iex2DObj( "DATA/timer.png" );

		//	変数初期化
		org_speed		=		PI / 180;
		selectNum		=		0;
		cul_speed		=		0.0f;
		clear					=		false;
		t						=		1.0f;
		step					=		0;

		//	オブジェクト情報初期化
		InitObj();

		return true;
	}

	//	オブジェクト初期化
	void	Clock::InitObj( void )
	{
		//	元のオブジェクト
		SetObj( 0, 640, 360, 30.0f, 0.0f, 1.0f );

		//	他のオブジェクト
		SetObj( 1, 940, -40, 120.0f, 0.0f, 2.0f );
		SetObj( 2, 340, 60, 300.0f, 0.0f, 0.5f );
		SetObj( 3, 540, -340, 240.0f, 0.0f, 1.0f );
	}

	//	オブジェクト情報設定
	void	Clock::SetObj( int n, int x, int y, float hAngle, float mAngle, float s )
	{
		obj[n].pos.x				=		x;
		obj[n].pos.y				=		y;
		obj[n].hourAngle		=		PI / 180.0f * hAngle;
		obj[n].minuteAngle =		PI / 180.0f * mAngle;
		obj[n].speed			=		s;

		//	初期位置保存
		obj[n].start		=		obj[n].pos;
		obj[n].end		=		obj[n].pos;
		obj[n].shift.x	=		0;
		obj[n].shift.y	=		0;
	}

//----------------------------------------------------------------------------
//	更新
//----------------------------------------------------------------------------

	//	更新
	void	Clock::Update( void )
	{
		//	動作
		Move();

		switch ( step )
		{
		case 0:		//	ズームアウト
			m_Camera.param += PI / 180 * CameraInfo::ZOOM_SPEED;
			m_Camera.scale = ( int )( CameraInfo::ZOOM_MAX * sinf( m_Camera.param ) );
			CalcPos();	//	ずれ計算
			if ( m_Camera.scale <= 0 )
			{
				m_Camera.param = 0.0f;
				m_Camera.scale = 0;
				step++;
			}
			break;

		case 1:		//	ゲーム開始
			Control();
			break;
		}

		//	パラメータ加算
		t += 0.01f;
		if ( t >= 1.0f )	t = 1.0f;
	}

//----------------------------------------------------------------------------
//	描画
//----------------------------------------------------------------------------

	//	描画
	void	Clock::Render( void )
	{
		//	変数準備
		DWORD	color[4] = { 0xFFFF7777, 0xFF77FF77, 0xFF7777FF, 0xFFFF77FF };
		int	x, y, w, h, sx, sy, sw, sh;
		POINT	p;

		for ( int i = 0; i < CLOCK_MAX; i++ )
		{
			//	時計描画
			x = obj[i].pos.x - 150 - m_Camera.scale / 2 + obj[i].shift.x / 2;
			y = obj[i].pos.y - 150 - m_Camera.scale / 2 + obj[i].shift.y / 2;
			p.x = obj[i].pos.x + obj[i].shift.x;
			p.y = obj[i].pos.y + obj[i].shift.y;
			w = h = 300 + m_Camera.scale;
			sx = sy = 0;
			sw = sh = 256;
			m_Clock->Render(x, y, w, h, sx, sy, sw, sh, RS_COPY, color[i]);

			//	分針描画
			sx = 256;
			m_Clock->Render(x, y, w, h, sx, sy, sw, sh, p, obj[i].minuteAngle);

			//	時針描画
			sx = 0;
			sy = 256;
			m_Clock->Render( x, y, w, h, sx, sy, sw, sh, p, obj[i].hourAngle );

			//	スピード表示
			x = obj[i].pos.x + 100;
			y = obj[i].pos.y + 100;
		}
	}

//----------------------------------------------------------------------------
//	動作関数
//----------------------------------------------------------------------------

	//　範囲内にあるか調べる
	bool	Clock::CheckWithin( const POINT& p, const int& startX, const int& startY, const int size )
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
	void	Clock::Lerp( POINT& out, const POINT p1, const POINT p2, float t )
	{
		float rate = t * t * ( 3.0f - 2.0f * t );   // 3次関数補間値に変換

		out.x = ( long )( p1.x * ( 1.0f - rate ) + p2.x * rate );
		out.y = ( long )( p1.y * ( 1.0f - rate ) + p2.y * rate );
	}

	//	動作
	void	Clock::Move( void )
	{
		//	時計の針の回転
		for ( int i = 0; i < CLOCK_MAX; i++ )
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

				//	元のスピードで回転
				obj[i].minuteAngle += org_speed;
				obj[i].hourAngle += org_speed / 12.0f;
			}
			else
			{
				//	最終の位置を求める
				obj[i].end.x = obj[i].start.x + length.x;
				obj[i].end.y = obj[i].start.y + length.y;

				//	移動
				Lerp( obj[i].pos, obj[i].start, obj[i].end, t );

				//	回転
				obj[i].minuteAngle += org_speed * ( obj[i].speed / cul_speed );
				obj[i].hourAngle += ( org_speed / 12.0f ) * ( obj[i].speed / cul_speed );
			}
		}
	}

	//	コントロール
	void	Clock::Control( void )
	{
		//	エンターで選択
		if ( KEY( KEY_ENTER ) == 3 )
		if ( obj[selectNum].speed == obj[0].speed && selectNum != 0 )
			clear = true;

		//	マウスクリックチェック
		for ( int n = 0; n < 4; n++ )
		{
			//	マウスカーソルが範囲内だったら
			if ( CheckWithin(Mouse::cursor, obj[n].pos.x - 150, obj[n].pos.y - 150, GameInfo::WNDSIZE ) )
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
						for ( int i = 0; i < CLOCK_MAX; i++ )
							obj[i].start = obj[i].pos;

						//	パラメータ設定
						t = 0.0f;
					}
				}
			}
		}
	}

	//	位置関係計算
	void	Clock::CalcPos( void )
	{
		float cx, cy, vx, vy;
		cx = 640;
		cy = 360;
		float	l;

		for ( int i = 0; i < CLOCK_MAX; i++ )
		{
			vx = obj[i].pos.x - cx;
			vy = obj[i].pos.y - cy;

			l = sqrt( vx * vx + vy *vy );

			vx /= l;
			vy /= l;

			if ( selectNum != i )
			{
				obj[i].shift.x = ( int )( vx * m_Camera.scale * 2 );
				obj[i].shift.y = ( int )( vy * m_Camera.scale * 2 );
			}
		}
	}

//----------------------------------------------------------------------------
//	情報取得・設定
//----------------------------------------------------------------------------

	//	タイトル情報取得
	void	Clock::GetTitleInfo( float hAngle, float mAngle )
	{
		obj[0].hourAngle		=	hAngle;
		obj[0].minuteAngle = mAngle;
	}