#include	"iextreme.h"
#include	"system/system.h"

#include	"sceneMain.h"
#include	"DataOwner.h"
#include	"Stage.h"
#include	"ChoicesMgr.h"
#include	"Control.h"
#include	"Campus.h"
#include	"TimeObject.h"
#include	"Gimmick.h"
#include	"Pumpkin.h"

using namespace EDX;

sceneMain::sceneMain(void)
{
	stage = nullptr;
	flag = nullptr;
	back = nullptr;
	state = BEGIN;
}

bool sceneMain::Initialize()
{
	//	ŠÂ‹«Ý’è
	iexLight::SetFog(800, 1000, 0);
	//	ŠÂ‹«Œõ
	//DataOwner::GetInst()->Init();

	back = new iex2DObj("DATA/‰¼”wŒi.png");
	stage = new StageMNG;
	stageID =DataOwner::GetInst()->stageNo;
	stage->LoadStage(stageID);
	flag = new FlagMgr;
	flag->Init();
	flag->SetSpeedList(stage->GetSpeedList());
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
	Pumpkin::GetInst()->Release();
}

void sceneMain::Update()
{
	Pumpkin::GetInst()->Update();
	if(MouseGet(EDX_CLICK_L) == 1)
	{
		int objID = -1;
		POINT p;
		switch(state)
		{
			case sceneMain::BEGIN:
				break;
			case sceneMain::MAIN:
				p = AddPoint(Mouse::cursor, Campus::GetInst()->GetPos());
				if((objID = stage->IsCollision(p)) != -1)
				{
					stage->Activate(objID);
					Campus::GetInst()->TimeReset();
				}
				break;
			case sceneMain::PAUSE:
				break;
			case sceneMain::CHECK:
				break;
			case sceneMain::END:
				break;
			default:
				break;
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
			if(KEY_Get(KEY_A) == 3)
			{
				stageID--;
				if(stage->LoadStage(stageID))
				{
					flag->Init();
					flag->SetSpeedList(stage->GetSpeedList());
				}
				else
					stageID++;
			}
			if(KEY_Get(KEY_B) == 3)
			{
				stageID++;
				if(stage->LoadStage(stageID))
				{
					flag->Init();
					flag->SetSpeedList(stage->GetSpeedList());
				}
				else
					stageID--;
			}


			stage->Update();
			flag->Update();
			Campus::GetInst()->SetNextPos(stage->GetPos(stage->GetNowObj()));
			if(Campus::GetInst()->IsMoveEnd())
			{
				Campus::GetInst()->SetPos(stage->GetPos(stage->GetNowObj()));
			}
			Campus::GetInst()->Update();
			break;
		case sceneMain::PAUSE:
			break;
		case sceneMain::CHECK:
			if(Campus::GetInst()->IsMoveEnd())
			{
				flag->CheckFlag();
				if(flag->IsFinishEffect())
				{
					if(flag->CheckNext())
						Campus::GetInst()->SetNextPos(flag->GetNowObjPos());
					else
					{
						Campus::GetInst()->SetNextPos(stage->GetPos(stage->GetNowObj()));
						state = MAIN;
					}
					Campus::GetInst()->TimeReset();
				}
			}
			stage->Update();
			flag->Update();
			Campus::GetInst()->Update();
			break;
		case sceneMain::END:
			stage->Update();
			flag->Update();
			break;
		default:
			break;
	}
	if(KEY_Get(KEY_LEFT) == 3)
	{
		if(flag->StartCheck())
		{
			Campus::GetInst()->SetNextPos(flag->GetNowObjPos());
			Campus::GetInst()->TimeReset();
			state = CHECK;
		}
	}

}

void sceneMain::Render()
{
	//	‰æ–ÊƒNƒŠƒA
	DataOwner::GetInst()->view->Activate();
	DataOwner::GetInst()->view->Clear(0x00000080);

	Campus::GetInst()->Add(back, -10000, -10000, 20000, 20000, 0, 0, 756, 512);

	stage->Render();
	flag->Render();

	Pumpkin::GetInst()->Render();

#ifdef _DEBUG
	char	str[64];
	wsprintf(str, "Stage: %d\n", stageID);
	IEX_DrawText(str, 10, 30, 200, 20, 0xFF7070FF);
#endif

}
