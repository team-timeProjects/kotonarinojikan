#include	"../IEX/iextreme.h"
#include	"system/System.h"
#include	"system/Framework.h"
#include	"../EDX/EDXLIB.h"
#include	"DataOwner.h"
#include	"Campus.h"
#include	"sceneMain.h"
#include	"sceneSelect.h"

#include	"sceneTitle.h"
#include	"TransitionBat.h"

//******************************************************************************
//
//	sceneTitleクラス
//
//******************************************************************************
bool sceneTitle::IsReturnTitle = false;
//----------------------------------------------------------------------
//	初期化・解放
//----------------------------------------------------------------------

	//	コンストラクタ
	sceneTitle::sceneTitle(void) : m_Back1(NULL), m_Back2(NULL), m_Star(NULL), m_Logo(NULL), m_Start(NULL), m_End(NULL)
	{

	}

	//	デストラクタ
	sceneTitle::~sceneTitle( void )
	{
		if (mode == MODE_EXIT)TransitionBat::GetInst()->Release();
		SafeDelete(m_Back1);
		SafeDelete(m_Back2);
		SafeDelete(m_Star);
		SafeDelete(m_Logo);
		SafeDelete(m_Start);
		SafeDelete(m_End);

	}

	//	初期化
	bool sceneTitle::Initialize( void )
	{
		//	環境設定
		iexLight::SetFog( 800, 1000, 0 );

		//	環境光
		DataOwner::GetInst()->Init();

		//	画像初期化
		m_Back1 = new EDX::EDX_2DObj("DATA/タイトル画面/Title3.png");
		m_Back2 = new EDX::EDX_2DObj("DATA/タイトル画面/Title1.png");
		m_Star = new EDX::EDX_2DObj("DATA/タイトル画面/Title2.png");

		m_Logo = new iex2DObj( "DATA/タイトル画面/title logo.png" );
		logoparam = { 670, 300, 500, 500 };
		m_Start = new iex2DObj( "DATA/タイトル画面/gaem start.png" );
		startparam = { 400, 550, 250, 150 };
		startAngle = 0.0f;
		m_End = new iex2DObj( "DATA/タイトル画面/game exit.png" );
		endparam = { 900, 550, 250, 150 };
		endAngle = 0.0f;
		TransitionBat::GetInst()->Init();
		if (IsReturnTitle){
			TransitionBat::GetInst()->SetStep(TransitionBat::TBAT_STATE::CENTER);
			TransitionBat::GetInst()->SetNextStep(TransitionBat::TBAT_STATE::UP);
			TransitionBat::GetInst()->TimeReset();
			IsReturnTitle = false;
		}
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
		TransitionBat::GetInst()->Update();
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
			m_Back1->Draw(0, 0);
			m_Back2->Draw(0, 0);
			static float a = 0;
			a -= EDX_PI_F / 360;
			m_Star->SetAngle(a);
			m_Star->Draw(-640, -640);
			RenderObject( m_Logo, logoparam, 0, 0, 512, 512, 0.0f );
			RenderObject( m_Start, startparam, 0, 0, 300, 170, PI / 180 * 30 * sinf( startAngle ) );
			RenderObject( m_End, endparam, 0, 0, 300, 170, PI / 180 * 30 * sinf( endAngle ) );
		}
		
		TransitionBat::GetInst()->Render();
		//RenderObject( m_Bat, batparam, 0, 0, 4096, 4096, 0.0f );
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
			if (EDX::MouseGet(EDX::EDX_CLICK_L)){
				TransitionBat::GetInst()->SetStep(TransitionBat::TBAT_STATE::DOWN);
				TransitionBat::GetInst()->TimeReset();
				TransitionBat::GetInst()->SetNextStep(TransitionBat::TBAT_STATE::CENTER);
				mode = MODE_MOVE_BAT;
			}

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
			if (EDX::MouseGet(EDX::EDX_CLICK_L)){
				mode = MODE_EXIT;
				exit(0);
			}

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
	void	sceneTitle::ModeMoveMenu(void)
	{
		startAngle = 0.0f;
		if (TransitionBat::GetInst()->IsMoveEnd()){
			mode = MODE_SCENE_CHANGE;
		}
	}

	//	メニューへ移動
	void	sceneTitle::ModeChangeScene( void )
	{
		MainFrame->ChangeScene(new sceneSelect());
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