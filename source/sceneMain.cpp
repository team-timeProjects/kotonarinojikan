#include	"iextreme.h"
#include	"system/system.h"
#include	"system/Framework.h"

#include	"sceneMain.h"
#include	"DataOwner.h"
#include	"Stage.h"
#include	"ChoicesMgr.h"
#include	"Control.h"
#include	"Campus.h"
#include	"TimeObject.h"
#include	"Gimmick.h"
#include	"Pumpkin.h"
#include	"sceneTitle.h"

using namespace EDX;

sceneMain::sceneMain(void)
{
	stage = nullptr;
	flag = nullptr;
	back = nullptr;
	back2 = nullptr;
	back2angle = 0.0f;
	angleSpeed = 0.0f;
	state = BEGIN;
	judgeClock = nullptr;
}

bool sceneMain::Initialize()
{
	//	ŠÂ‹«Ý’è
	iexLight::SetFog(800, 1000, 0);
	//	ŠÂ‹«Œõ
	//DataOwner::GetInst()->Init();
	back = new EDX::EDX_2DObj("DATA/ƒQ[ƒ€‰æ–Ê/”wŒi‚P.png");
	back2 = new EDX::EDX_2DObj("DATA/ƒQ[ƒ€‰æ–Ê/”wŒi2.png");
	back2angle = 0.0f;
	angleSpeed = 0.01f;
	stage = new StageMNG;
	stageID = DataOwner::GetInst()->stageNo;
	stage->LoadStage(stageID);
	flag = new FlagMgr;
	flag->Init();
	flag->SetSpeedList(stage->GetSpeedList());
	judgeClock = new JudgeClock;
	judgeClock->Init(stage->GetJudgeNum(), stage->GetjudgeTimer());
	state = MAIN;
	Pumpkin::GetInst()->Init();
	Pumpkin::GetInst()->SetOpen(true);
	Pumpkin::GetInst()->SetMaxY(1000);
	return true;
}

sceneMain::~sceneMain()
{
	SafeDelete(stage);
	SafeDelete(flag);
	SafeDelete(back);
	SafeDelete(back2);
	SafeDelete(judgeClock);
	Pumpkin::GetInst()->Release();
}

void sceneMain::Update()
{
	back2angle += angleSpeed;
	back2->SetAngle(back2angle);

	Pumpkin::GetInst()->Update();
	if(MouseGet(EDX_CLICK_L) == 1 && state == MAIN)
	{
		int objID = -1;
		POINT p;
		p = AddPoint(Mouse::cursor, Campus::GetInst()->GetPos());
		if((objID = stage->IsCollision(p)) != -1)
		{
			stage->Activate(objID);
			Campus::GetInst()->TimeReset();
		}
	}
	if(MouseGet(EDX_CLICK_L) && state == MAIN)
	{
		POINT p;
		p = Mouse::cursor;
		if(judgeClock->IsCollision(p))
		{
			judgeClock->TimerClockUp();
		}
	}
	//‹àƒtƒ‰ƒbƒO
	if (state == MAIN &&MouseGet(EDX_CLICK_R)==1)
	{
		int objID = -1;
		TimeObj::ResetChain();
		TimeObj::AddChain();
		POINT p = AddPoint(Mouse::cursor, Campus::GetInst()->GetPos());
		if ((objID = stage->IsCollision(p)) != -1)
		{
			flag->AppendGoldFlag(stage->GetObj(objID));
		}
	}
	if(state == MAIN && (KEY_Get(KEY_UP) == 3 || KEY_Get(KEY_DOWN) == 3))
	{
		int objID = -1;
		POINT p = AddPoint(Mouse::cursor, Campus::GetInst()->GetPos());
		if((objID = stage->IsCollision(p)) != -1)
		{
			flag->AppendFlag(stage->GetObj(objID), KEY_Get(KEY_UP) == 3);
			Campus::GetInst()->TimeReset();
		}
	}
	switch(state)
	{
		case sceneMain::BEGIN:
			stage->Update();
			flag->Update();
			break;
		case sceneMain::MAIN:
			//if(KEY_Get(KEY_A) == 3)
			//{
			//	stageID--;
			//	if(stage->LoadStage(stageID))
			//	{
			//		flag->Init();
			//		flag->SetSpeedList(stage->GetSpeedList());
			//	}
			//	else
			//		stageID++;
			//}
			//if(KEY_Get(KEY_B) == 3)
			//{
			//	stageID++;
			//	if(stage->LoadStage(stageID))
			//	{
			//		flag->Init();
			//		flag->SetSpeedList(stage->GetSpeedList());
			//	}
			//	else
			//		stageID--;
			//}


			stage->Update();
			flag->Update();
			judgeClock->Update();
			Campus::GetInst()->SetNextPos(stage->GetPos(stage->GetNowObj()));
			if(Campus::GetInst()->IsMoveEnd())
			{
				Campus::GetInst()->SetPos(stage->GetPos(stage->GetNowObj()));
			}
			Campus::GetInst()->Update();
			if(judgeClock->CheckPalse())
			{
				flag->StartCheck();
				state = CHECK;
			}
			else if(judgeClock->GetTime() <= 0)
			{
				state = END;
			}
			break;
		case sceneMain::PAUSE:
			break;
		case sceneMain::CHECK:
			if(flag->IsCheckEnd())
			{
				if(flag->IsClear())
				{
					state = END;
				}
				else
				{
					Campus::GetInst()->SetNextPos(stage->GetPos(stage->GetNowObj()));
					Campus::GetInst()->TimeReset();
					state = MAIN;
				}
			}
			stage->Update();
			flag->Update();
			Campus::GetInst()->Update();
			break;
		case sceneMain::END:
			stage->Update();
			flag->Update();
			MainFrame->ChangeScene(new sceneTitle);
			return;
			break;
		default:
			break;
	}
	//if(KEY_Get(KEY_LEFT) == 3)
	//{
	//	flag->StartCheck();
	//	TimeObj::ResetChain();
	//	state = CHECK;
	//}
	flag->SetHaveGoldFlag(stage->GetHaveGoldFlag());
}

void sceneMain::Render()
{
	//	‰æ–ÊƒNƒŠƒA
	DataOwner::GetInst()->view->Activate();
	DataOwner::GetInst()->view->Clear(0x00000080);

	back->Draw(0, 0);
	back2->Draw(1280 / 2 - 720 / 2, 720 / 2 - 720 / 2);
	//	Campus::GetInst()->Add(back, 0, 0, 1280, 720, 0, 0, 3508, 2480);

	stage->Render();
	flag->Render();
	judgeClock->Render();
	Pumpkin::GetInst()->Render();

#ifdef _DEBUG
	char	str[64];
	wsprintf(str, "Stage: %d\n", stageID);
	IEX_DrawText(str, 10, 30, 200, 20, 0xFF7070FF);
	wsprintf(str, "timelimit = %d", judgeClock->GetTime() / 60);
	IEX_DrawText(str, 10, 70, 200, 20, 0xFF7070FF);
#endif

}
