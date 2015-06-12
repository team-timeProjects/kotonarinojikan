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
<<<<<<< HEAD

int sceneMain::timelimit = 0;
#define	SIZE 130
=======
#include	"Gimmick.h"
#include	"Pumpkin.h"
#include	"sceneTitle.h"

using namespace EDX;

>>>>>>> remotes/origin/horita
sceneMain::sceneMain(void)
{
	stage = nullptr;
	flag = nullptr;
	back = nullptr;
<<<<<<< HEAD
	rCheckClock = nullptr;
	state = BEGIN;

=======
	back2 = nullptr;
	back2angle = 0.0f;
	angleSpeed = 0.0f;
	state = BEGIN;
	judgeClock = nullptr;
>>>>>>> remotes/origin/horita
}

bool sceneMain::Initialize()
{
	//	ŠÂ‹«İ’è
	iexLight::SetFog(800, 1000, 0);
	//	ŠÂ‹«Œõ
<<<<<<< HEAD
	DataOwner::GetInst()->Init();

	back = new iex2DObj("DATA/ƒQ[ƒ€‰æ–Ê/”wŒi‚P.png");
	stage = new StageMNG;
	stageID = 1;
=======
	//DataOwner::GetInst()->Init();
	back = new EDX::EDX_2DObj("DATA/ƒQ[ƒ€‰æ–Ê/”wŒi‚P.png");
	back2 = new EDX::EDX_2DObj("DATA/ƒQ[ƒ€‰æ–Ê/”wŒi2.png");
	back2angle = 0.0f;
	angleSpeed = 0.01f;
	stage = new StageMNG;
	stageID = DataOwner::GetInst()->stageNo;
>>>>>>> remotes/origin/horita
	stage->LoadStage(stageID);
	flag = new FlagMgr;
	flag->Init();
	flag->SetSpeedList(stage->GetSpeedList());
<<<<<<< HEAD
	state = MAIN;

	//‹­§”»’èŒv‰Šú‰»
	judgeNum = stage->GetJudgeNum();		//@‰ñ“]i”»’èj‚·‚é‰ñ”
	judgeTimer = stage->GetJudgeTimer();	//@ˆêü‚Ì‰ñ“]•b
	rCheckClock = new iex2DObj("DATA\\timer.png");
	check_obj.LongAngle = 0.0f;
	check_obj.ShortAngle = ((12 - judgeNum) * 30 * PI) / 180;
	check_obj.pos.x = 1200;
	check_obj.pos.y = 580;

	timelimit = judgeNum*judgeTimer * 60;
	
=======
	judgeClock = new JudgeClock;
	judgeClock->Init(stage->GetJudgeNum(), stage->GetjudgeTimer());
	state = MAIN;
	Pumpkin::GetInst()->Init();
	Pumpkin::GetInst()->SetOpen(true);
	Pumpkin::GetInst()->SetMaxY(1000);
>>>>>>> remotes/origin/horita
	return true;
}

sceneMain::~sceneMain()
{
	SafeDelete(stage);
	SafeDelete(flag);
	SafeDelete(back);
<<<<<<< HEAD
	SafeDelete(rCheckClock);
=======
	SafeDelete(back2);
	SafeDelete(judgeClock);
	Pumpkin::GetInst()->Release();
>>>>>>> remotes/origin/horita
}

void sceneMain::Update()
{
<<<<<<< HEAD
	if(KEY_Get(KEY_A) == 3)
	{
		stageID--;
		if(!stage->LoadStage(stageID))
			stageID++;
	}
	if(KEY_Get(KEY_B) == 3)
	{
		stageID++;
		if(!stage->LoadStage(stageID))
			stageID--;
	}


	if(MouseGet(EDX_CLICK_L))
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
=======
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
>>>>>>> remotes/origin/horita
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
<<<<<<< HEAD
			stage->Update();
			flag->Update();

			//‹­§”»’è
			
			//check_obj.LongAngle = flag->GetMissCount() * 10 * 60 * (2 * PI*judgeNum * 60) / (timelimit * 60);
			
			check_obj.LongAngle += (2 * PI*judgeNum * 60) / (timelimit * 60);
			check_obj.ShortAngle = ((12 - judgeNum) * 30 * PI) / 180 + check_obj.LongAngle / 12;
			if (check_obj.LongAngle > PI * 2)
			{
				check_obj.LongAngle = 0.0f;
				judgeNum--;
				goto CLOCK_CHECK;
			}
			timelimit--;
			if (timelimit == 0) state = sceneMain::END;

=======
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
>>>>>>> remotes/origin/horita
			Campus::GetInst()->SetNextPos(stage->GetPos(stage->GetNowObj()));
			if(Campus::GetInst()->IsMoveEnd())
			{
				Campus::GetInst()->SetPos(stage->GetPos(stage->GetNowObj()));
			}
			Campus::GetInst()->Update();
<<<<<<< HEAD
=======
			if(judgeClock->CheckPalse())
			{
				flag->StartCheck();
				state = CHECK;
			}
			else if(judgeClock->GetTime() <= 0)
			{
				state = END;
			}
>>>>>>> remotes/origin/horita
			break;
		case sceneMain::PAUSE:
			break;
		case sceneMain::CHECK:
<<<<<<< HEAD
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
=======
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
>>>>>>> remotes/origin/horita
				}
			}
			stage->Update();
			flag->Update();
			Campus::GetInst()->Update();
<<<<<<< HEAD

			if (judgeNum <= 0) judgeNum = 0;

			if (timelimit == 0) state = sceneMain::END;

=======
>>>>>>> remotes/origin/horita
			break;
		case sceneMain::END:
			stage->Update();
			flag->Update();
<<<<<<< HEAD
=======
			MainFrame->ChangeScene(new sceneTitle);
			return;
>>>>>>> remotes/origin/horita
			break;
		default:
			break;
	}
<<<<<<< HEAD
	if(KEY_Get(KEY_LEFT) == 3)
	{
	CLOCK_CHECK:;
		if(flag->StartCheck())
		{
			Campus::GetInst()->SetNextPos(flag->GetNowObjPos());
			Campus::GetInst()->TimeReset();
			state = CHECK;
		}
	}

=======
	//if(KEY_Get(KEY_LEFT) == 3)
	//{
	//	flag->StartCheck();
	//	TimeObj::ResetChain();
	//	state = CHECK;
	//}
	flag->SetHaveGoldFlag(stage->GetHaveGoldFlag());
>>>>>>> remotes/origin/horita
}

void sceneMain::Render()
{
	//	‰æ–ÊƒNƒŠƒA
	DataOwner::GetInst()->view->Activate();
	DataOwner::GetInst()->view->Clear(0x00000080);

<<<<<<< HEAD
	Campus::GetInst()->Add(back, -10000, -10000, 20000, 20000, 0, 0, 756, 512);

	stage->Render();
	flag->Render();

	int	minute = timelimit / (60 * 60);
	int	second = (timelimit - (60 * 60 * minute)) / 60;
	if (timelimit <= 0)	timelimit = 0;
	
	//‹­§”»’èŒv
	//	Œv•`‰æ
	rCheckClock->Render(check_obj.pos.x - SIZE/2, check_obj.pos.y, SIZE, SIZE, 0, 0, 256, 256);
	//@’Zj•`‰æ										  	
	rCheckClock->Render(check_obj.pos.x - SIZE/2, check_obj.pos.y, SIZE, SIZE, 0, 256, 256, 256, check_obj.pos, check_obj.ShortAngle, (float)1.0f);
	//	•ªj•`‰æ										   	
	rCheckClock->Render(check_obj.pos.x - SIZE/2, check_obj.pos.y, SIZE, SIZE, 256, 0, 256, 256, check_obj.pos, (float)check_obj.LongAngle, (float)1.0f);

	//sprintf_s(str, "c‚èŠÔ%d•ª%d•b", minute, second);

#ifdef _DEBUG
	char	str[64];
	wsprintf(str, "c‚èŠÔ%d•ª%d•b", minute, second);
	//wsprintf(str, "Stage: %d\n", stageID);
	IEX_DrawText(str, 10, 30, 200, 20, 0xFF7070FF);
=======
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
>>>>>>> remotes/origin/horita
#endif

}
