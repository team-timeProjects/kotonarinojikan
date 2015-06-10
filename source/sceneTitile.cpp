#include	"../IEX/iextreme.h"
#include	"system/System.h"
#include	"system/Framework.h"
#include	"DataOwner.h"
#include	"Campus.h"
#include	"sceneMain.h"
#include	"sceneSelect.h"

#include	"sceneTitle.h"

//******************************************************************************
//
//	sceneTitleクラス
//
//******************************************************************************

//----------------------------------------------------------------------
//	初期化・解放
//----------------------------------------------------------------------

	//	コンストラクタ
	sceneTitle::sceneTitle( void ) : m_Back( NULL ), m_Logo( NULL ), m_Start( NULL ), m_End( NULL )
	{
		
	}

	//	デストラクタ
	sceneTitle::~sceneTitle( void )
	{
		SafeDelete(m_Back);
		SafeDelete(m_Logo);
		SafeDelete(m_Start);
		SafeDelete(m_End);
		SafeDelete(m_Bat);
	}

	//	初期化
	bool sceneTitle::Initialize( void )
	{
		//	環境設定
		iexLight::SetFog( 800, 1000, 0 );

		//	環境光
		DataOwner::GetInst()->Init();

		//	画像初期化
		m_Back = new iex2DObj( "DATA/タイトル画面/title backround.png" );
		backparam = { 640, 360, 1280, 720 };
		m_Logo = new iex2DObj( "DATA/タイトル画面/title logo.png" );
		logoparam = { 670, 300, 500, 500 };
		m_Start = new iex2DObj( "DATA/タイトル画面/gaem start.png" );
		startparam = { 400, 550, 250, 150 };
		startAngle = 0.0f;
		m_End = new iex2DObj( "DATA/タイトル画面/game exit.png" );
		endparam = { 900, 550, 250, 150 };
		endAngle = 0.0f;
		m_Bat = new iex2DObj( "DATA/移行演出/koumori taigun.png" );
		batparam = { 2400, 1500, 2200, 2200 };

		//	変数
		mode = MODE_SELECT;


		return true;
	}

//----------------------------------------------------------------------
//	更新・描画
//----------------------------------------------------------------------

	//	更新
	void sceneTitle::Update( void )
	{
		switch ( mode )
		{
		case MODE_SELECT:				ModeSelect();			break;
		case MODE_MOVE_BAT:			ModeMoveMenu();		break;
		case MODE_SCENE_CHANGE:	ModeChangeScene();	break;
		}
	}
	
	//	描画
	void sceneTitle::Render( void )
	{
		//	画面クリア
		DataOwner::GetInst()->view->Activate();
		DataOwner::GetInst()->view->Clear();

		//	画像描画
		if (mode != MODE_SCENE_CHANGE)
		{
			RenderObject( m_Back, backparam, 0, 0, 1280, 720, 0.0f );
			RenderObject( m_Logo, logoparam, 0, 0, 512, 512, 0.0f );
			RenderObject( m_Start, startparam, 0, 0, 300, 170, PI / 180 * 30 * sinf( startAngle ) );
			RenderObject( m_End, endparam, 0, 0, 300, 170, PI / 180 * 30 * sinf( endAngle ) );
		}
		
		RenderObject( m_Bat, batparam, 0, 0, 4096, 4096, 0.0f );
	}

	//	オブジェクト描画
	void	sceneTitle::RenderObject( iex2DObj* obj, ImageParam param, int sx, int sy, int sw, int sh, float angle )
	{
		POINT	p;
		p.x = param.x;
		p.y = param.y;
		obj->Render( param.x - param.w / 2, param.y - param.h / 2, param.w, param.h, sx, sy, sw, sh, p, angle );
	}

//----------------------------------------------------------------------
//	動作関数
//----------------------------------------------------------------------

	//	選択
	void	sceneTitle::ModeSelect( void )
	{
		//	カーソル位置取得
		POINT p;
		GetCursorPos( &p );
		ScreenToClient( iexSystem::Window, &p );

		//	カーソルが乗ってたら
		if ( OnCursorCheck( p, startparam ) )
		{
			//	クリック
			if (EDX::MouseGet(EDX::EDX_CLICK_L))
				mode = MODE_MOVE_BAT;

			startAngle += 0.05f;
			startparam.w = 300;
			startparam.h = 200;
		}
		else
		{
			startAngle = 0.0f;
			startparam.w = 250;
			startparam.h = 150;
		}

		if ( OnCursorCheck( p, endparam ) )
		{
			//	クリック
			if (EDX::MouseGet(EDX::EDX_CLICK_L))
				exit( 0 );

			endAngle += 0.05f;
			endparam.w = 300;
			endparam.h = 200;
		}
		else
		{
			endAngle = 0.0f;
			endparam.w = 250;
			endparam.h = 150;
		}
	}

	//	コウモリが((((⊂（´∀｀ｏ)⊃))))飛ぶｿﾞｫ！
	void	sceneTitle::ModeMoveMenu( void )
	{
		startAngle = 0.0f;

		if ( batparam.x >= 700 )
		{
			batparam.x -= 10;
			batparam.y -= 5;
		}
		else
		{
			mode = MODE_SCENE_CHANGE;
		}

	}

	//	メニューへ移動
	void	sceneTitle::ModeChangeScene( void )
	{
		if ( batparam.x >= -1000 )
		{
			batparam.x -= 10;
			batparam.y -= 5;
		}
		else
		{
			MainFrame->ChangeScene(new sceneSelect());
			return;
		}
	}

	//	カーソルが乗ってるかチェック
	bool	sceneTitle::OnCursorCheck( POINT p, ImageParam i_param )
	{
		if ( p.x >= i_param.x - i_param.w / 2 && p.x <= i_param.x + i_param.w / 2 )
		{
			if ( p.y >= i_param.y - i_param.h / 2 && p.y <= i_param.y + i_param.h / 2 )
			{
				return	true;
			}
		}
		
		return	false;
	}