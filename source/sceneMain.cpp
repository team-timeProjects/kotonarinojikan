#include	"iextreme.h"
#include	"system/system.h"

#include	"sceneMain.h"
#include	"DataOwner.h"
#include	"Stage.h"
#include	"stub.h"
#include	"ChoicesMgr.h"


sceneMain::sceneMain(void)
{

}

bool sceneMain::Initialize()
{
	//	環境設定
	iexLight::SetFog(800, 1000, 0);
	//	環境光
	DataOwner::GetInst()->Init();

	// アップデートメソッド登録
	updateDgt = updateDgt.Cleate(CAMPUS, Campus::Inst(), &Campus::Update);
	// レンダーメソッド登録
	renderDgt.Cleate(BACK, DataOwner::GetInst()->back, &BackHoge::Render);
	renderDgt.Cleate(BIGCLOCK, DataOwner::GetInst()->bigClock, &BigClockHoge::Render);
	renderDgt.Cleate(CLOCK_SELECT, DataOwner::GetInst()->choiceClock, &ChoicesMgr::Render);
	renderDgt.Cleate(CLOCK_MAIN, DataOwner::GetInst()->clock, &ClockHoge::Render);
	//renderDgt.Cleate(STAGE, DataOwner::GetInst()->stage, &StageHoge::Render);
	renderDgt.Cleate(GAME, DataOwner::GetInst()->gameMain, &Clock::Render);

	// メインキュー設定
	mainQueue.push_back(&sceneMain::StageSelect_Intro);
	Campus::Inst()->Zoom(970, 218, 2.5f);
	step = ZOOM_IN;

	return true;
}

sceneMain::~sceneMain()
{
}

void sceneMain::Update()
{
	updateDgt.RetentionRun();
	if (!(this->*mainQueue.front())())
	{
		mainQueue.pop_front();
	}
}

void sceneMain::Render()
{
	//	画面クリア
	DataOwner::GetInst()->view->Activate();
	DataOwner::GetInst()->view->Clear(0x00000080);

	renderDgt();

#ifdef _DEBUG
	char text[128];
	wsprintf(text, "Mode %s\n", str);
	IEX_DrawText(text, 10, 30, 200, 20, 0xFF808020);
#endif
}

//　タイトルからの遷移
bool sceneMain::StageSelect_Intro()
{
	wsprintf(str, "StageSelect_Intro");

	DataOwner::GetInst()->choiceClock->Update();

	renderDgt = BACK;
	renderDgt += BIGCLOCK;
	renderDgt += CLOCK_SELECT;

	if (Campus::Inst()->IsZoomEnd())
	{
		// To AppendStage
		mainQueue.push_back(&sceneMain::AppendStage);
		POINT p = DataOwner::GetInst()->clock->GetPos(DataOwner::GetInst()->clock->GetCount());
		Campus::Inst()->Zoom(p.x, p.y, 7.0f);
		return false;
	}
	return true;
}

//　ステージ追加演出
bool sceneMain::AppendStage()
{
	wsprintf(str, "AppendStage");

	DataOwner::GetInst()->choiceClock->Update();

	renderDgt = BACK;
	renderDgt += BIGCLOCK;
	renderDgt += CLOCK_SELECT;

	if (Campus::Inst()->IsZoomEnd())
	{
		// To StageSelect
		mainQueue.push_back(&sceneMain::StageSelect);
		DataOwner::GetInst()->clock->Append();
		return false;
	}
	return true;
}

//　ステージ選択画面
bool sceneMain::StageSelect()
{
	wsprintf(str, "StageSelect");

	DataOwner::GetInst()->choiceClock->Update();

	renderDgt = BACK;
	renderDgt += BIGCLOCK;
	renderDgt += CLOCK_SELECT;

	if (KEY_Get(KEY_ENTER) == 3)// ステージ選択
	{
		// To GameMain Intro
		mainQueue.push_back(&sceneMain::GameMain_Intro);
		POINT p = DataOwner::GetInst()->clock->GetPos(DataOwner::GetInst()->clock->GetCount() - 1);
		Campus::Inst()->Zoom(p.x, p.y, 25.0f);
		step = ZOOM_IN;
		return false;
	}
	return true;
}

//　ゲームメイン導入演出
bool sceneMain::GameMain_Intro()
{
	wsprintf(str, "GameMain_Intro");


	switch (step)
	{
		case ZOOM_IN:
			DataOwner::GetInst()->choiceClock->Update();
			renderDgt = BIGCLOCK;
			renderDgt += CLOCK_SELECT;
			if (Campus::Inst()->IsZoomEnd())
			{
				// ステージロード
				DataOwner::GetInst()->gameMain->Initialize();
				//DataOwner::GetInst()->gameMain->LoadStage(DataOwner::GetInst()->clock->GetCount());
				POINT p = DataOwner::GetInst()->clock->GetPos(DataOwner::GetInst()->clock->GetCount() - 1);
				Campus::Inst()->Zoom(p.x, p.y, 7.0f);
				step = ZOOM_OUT;
			}
			break;
		case ZOOM_OUT:
			renderDgt = GAME;
			renderDgt += CLOCK_MAIN;
			if (Campus::Inst()->IsZoomEnd())
			{
				// To GameMain
				mainQueue.push_back(&sceneMain::GameMain);
				step = ZOOM_IN;
				return false;
			}
			break;
	}
	return true;

}

//　ゲームメイン画面
bool sceneMain::GameMain()
{
	wsprintf(str, "GameMain");

	renderDgt = GAME;

	// ゲームメイン処理
	DataOwner::GetInst()->gameMain->Update();
	//if (KEY_Get(KEY_D) == 3)//　ゲームをやめる
	//{
	//	mainQueue.push_back(&sceneMain::StageSelect_Intro);
	//}
	if (KEY_Get(KEY_C) == 3)//　ゲームクリア
	{
		// To GameMain Outro
		mainQueue.push_back(&sceneMain::GameMain_Outro);
		POINT p = DataOwner::GetInst()->clock->GetPos(DataOwner::GetInst()->clock->GetCount() - 1);
		Campus::Inst()->Zoom(p.x, p.y, 25.0f);
		step = ZOOM_IN;
		return false;
	}
	return true;

}

//　ゲームメインクリア後の演出
bool sceneMain::GameMain_Outro()
{
	wsprintf(str, "GameMain_Outro");

	switch (step)
	{
		case ZOOM_IN:
			renderDgt = GAME;
			renderDgt += CLOCK_MAIN;

			if (Campus::Inst()->IsZoomEnd())
			{
				if (DataOwner::GetInst()->clock->GetCount() < 12)//時計のステージすべて解放した
				{
					POINT p = DataOwner::GetInst()->clock->GetPos(DataOwner::GetInst()->clock->GetCount());
					Campus::Inst()->Zoom(p.x, p.y, 7.0f);
				}
				else
				{
					Campus::Inst()->Zoom(978, 218, 2.5f);//大時計中央
				}
				step++;
			}
			break;
		case ZOOM_OUT:
			renderDgt = BACK;
			renderDgt += BIGCLOCK;
			renderDgt += CLOCK_SELECT;

			DataOwner::GetInst()->choiceClock->Update();
			// To Append Stage
			mainQueue.push_back(&sceneMain::AppendStage);
			step = ZOOM_IN;
			return false;
			break;
	}
	return true;

}

