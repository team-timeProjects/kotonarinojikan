#include	"iextreme.h"
#include	"system/system.h"

#include	"sceneMain.h"
#include	"DataOwner.h"
#include	"Stage.h"
#include	"ChoicesMgr.h"
#include	"Control.h"
#include	"Campus.h"
#include	"TimeObject.h"

int sceneMain::timelimit = 0;
#define	SIZE 130
sceneMain::sceneMain(void)
{
	stage = nullptr;
	flag = nullptr;
	back = nullptr;
	rCheckClock = nullptr;
	state = BEGIN;

}

bool sceneMain::Initialize()
{
	//	ŠÂ‹«Ý’è
	iexLight::SetFog(800, 1000, 0);
	//	ŠÂ‹«Œõ
	DataOwner::GetInst()->Init();

	back = new iex2DObj("DATA/ƒQ[ƒ€‰æ–Ê/”wŒi‚P.png");
	stage = new StageMNG;
	stageID = 1;
	stage->LoadStage(stageID);
	flag = new FlagMgr;
	flag->Init();
	flag->SetSpeedList(stage->GetSpeedList());
	state = MAIN;

	//‹­§”»’èŽžŒv‰Šú‰»
	judgeNum = stage->GetJudgeNum();		//@‰ñ“]i”»’èj‚·‚é‰ñ”
	judgeTimer = stage->GetJudgeTimer();	//@ˆêŽü‚Ì‰ñ“]•b
	rCheckClock = new iex2DObj("DATA\\timer.png");
	check_obj.LongAngle = 0.0f;
	check_obj.ShortAngle = ((12 - judgeNum) * 30 * PI) / 180;
	check_obj.pos.x = 1200;
	check_obj.pos.y = 580;

	timelimit = judgeNum*judgeTimer * 60;
	
	return true;
}

sceneMain::~sceneMain()
{
	SafeDelete(stage);
	SafeDelete(flag);
	SafeDelete(back);
	SafeDelete(rCheckClock);
}

void sceneMain::Update()
{
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

			if (judgeNum <= 0) judgeNum = 0;

			if (timelimit == 0) state = sceneMain::END;

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
	CLOCK_CHECK:;
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

	int	minute = timelimit / (60 * 60);
	int	second = (timelimit - (60 * 60 * minute)) / 60;
	if (timelimit <= 0)	timelimit = 0;
	
	//‹­§”»’èŽžŒv
	//	ŽžŒv•`‰æ
	rCheckClock->Render(check_obj.pos.x - SIZE/2, check_obj.pos.y, SIZE, SIZE, 0, 0, 256, 256);
	//@’Zj•`‰æ										  	
	rCheckClock->Render(check_obj.pos.x - SIZE/2, check_obj.pos.y, SIZE, SIZE, 0, 256, 256, 256, check_obj.pos, check_obj.ShortAngle, (float)1.0f);
	//	•ªj•`‰æ										   	
	rCheckClock->Render(check_obj.pos.x - SIZE/2, check_obj.pos.y, SIZE, SIZE, 256, 0, 256, 256, check_obj.pos, (float)check_obj.LongAngle, (float)1.0f);

	//sprintf_s(str, "Žc‚èŽžŠÔ%d•ª%d•b", minute, second);

#ifdef _DEBUG
	char	str[64];
	wsprintf(str, "Žc‚èŽžŠÔ%d•ª%d•b", minute, second);
	//wsprintf(str, "Stage: %d\n", stageID);
	IEX_DrawText(str, 10, 30, 200, 20, 0xFF7070FF);
#endif

}
