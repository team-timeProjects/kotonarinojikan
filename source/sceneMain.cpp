#include	"iextreme.h"
#include	"system/system.h"
#include	"system/Framework.h"

#include	"tomAL\tomAL.h"
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
	//	環境設定
	iexLight::SetFog(800, 1000, 0);
	//	環境光
	//DataOwner::GetInst()->Init();
	back = new EDX::EDX_2DObj("DATA/ゲーム画面/背景１.png");
	back2 = new EDX::EDX_2DObj("DATA/ゲーム画面/背景2.png");
	//Δ
	menubutton = DataOwner::GetInst()->imageFactory->GetImage(ImageFactory::ImageID::BUTTON_MENU);
	menuParam = DataOwner::GetInst()->imageFactory->GetParam(ImageFactory::ImageID::BUTTON_MENU);
	///Δ
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
	state = BEGIN;
	Pumpkin::GetInst()->Init();
	Pumpkin::GetInst()->SetOpen(true);
	Pumpkin::GetInst()->SetMaxY(1000);
	armgr = new tomALManager();
	armgr->Init();

	armgr->SetInmemory(0, "DATA\\sound\\basabasa.wav");

	startEffect = new StartEffect;
	startEffect->Init(this);

	return true;
}

sceneMain::~sceneMain()
{
	SafeDelete(stage);
	SafeDelete(flag);
	SafeDelete(back);
	SafeDelete(back2);
	SafeDelete(judgeClock);
	SafeDelete(startEffect);
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

	//　ホイール
	if(state == MAIN && (EDX::MouseGet(EDX::EDX_WHEEL) > 0 || EDX::MouseGet(EDX::EDX_WHEEL) < 0))
	{
		int objID = -1;
		POINT p = AddPoint(Mouse::cursor, Campus::GetInst()->GetPos());
		if((objID = stage->IsCollision(p)) != -1)
		{
			flag->AppendFlag(stage->GetObj(objID), (EDX::MouseGet(EDX::EDX_WHEEL) > 0));
			Campus::GetInst()->TimeReset();
		}
	}

	//　強制判定時計の長押し
	if(MouseGet(EDX_CLICK_L) && state == MAIN)
	{
		POINT p;
		p = Mouse::cursor;
		if(judgeClock->IsCollision(p))
		{
			judgeClock->TimerClockUp();

		}
	}

	//　金フラッグ
	if(state == MAIN &&MouseGet(EDX_CLICK_R) == 1)
	{
		armgr->PlayInmemory(0);
		int objID = -1;
		TimeObj::ResetChain();
		TimeObj::AddChain();
		POINT p = AddPoint(Mouse::cursor, Campus::GetInst()->GetPos());
		if((objID = stage->IsCollision(p)) != -1)
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
			flag->Update();
			if(Pumpkin::GetInst()->IsMoveEnd())
				state = START_EFFECT;
			Campus::GetInst()->SetPos(stage->GetPos(stage->GetNowObj()));
			Campus::GetInst()->Update();
			break;
		case sceneMain::START_EFFECT:
			if(startEffect->IsFinish())
			{
				state = MAIN;
				SafeDelete(startEffect);
			}
			else
				startEffect->Update();
			flag->Update();
			break;

		case sceneMain::MAIN:
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
	//卍

	TimeObj::ResetChain();
	//state = CHECK;

}

void sceneMain::Render()
{
	//	画面クリア
	DataOwner::GetInst()->view->Activate();
	DataOwner::GetInst()->view->Clear(0x00000080);

	back->Draw(0, 0);
	back2->Draw(1280 / 2 - 720 / 2, 720 / 2 - 720 / 2);
	//	Campus::GetInst()->Add(back, 0, 0, 1280, 720, 0, 0, 3508, 2480);

	stage->Render();
	flag->Render();
	judgeClock->Render();
	if(state == State::START_EFFECT)
		startEffect->Render();
	//Δ
	menubutton->Render(1155, 530, menuParam.w, menuParam.h, menuParam.x, menuParam.y, menuParam.w, menuParam.h);
	///Δ
	Pumpkin::GetInst()->Render();

#ifdef _DEBUG
	char	str[64];
	wsprintf(str, "Stage: %d\n", stageID);
	IEX_DrawText(str, 10, 30, 200, 20, 0xFF7070FF);
	wsprintf(str, "state = %d", state);
	IEX_DrawText(str, 10, 50, 200, 20, 0xFF7070FF);
	wsprintf(str, "timelimit = %d", judgeClock->GetTime() / 60);
	IEX_DrawText(str, 10, 70, 200, 20, 0xFF7070FF);
#endif

}


sceneMain::StartEffect::StartEffect()
{
	step = BEGIN;
	effectTimer = 0;
	idx = 0;
}

void sceneMain::StartEffect::Init(sceneMain* ref)
{
	step = BEGIN;
	effectTimer = 60;
	scene = ref;
	//Δ
	batImage = DataOwner::GetInst()->imageFactory->GetImage(ImageFactory::FRAG_BLACK_OPEN);
	batParam = DataOwner::GetInst()->imageFactory->GetParam(ImageFactory::FRAG_BLACK_OPEN);
	///Δ
	std::map<int, int> speed = scene->stage->GetSpeedList();
	for(auto& r : speed)
		r.second = 0;
	scene->flag->SetSpeedList(speed);
	TimeObj* obj = nullptr;
	int id = 0;
	while((obj = scene->stage->GetObj(id)) != nullptr)
	{
		scene->flag->AppendPlainFlag(obj);
		id++;
	}
	idx = 0;
}

void sceneMain::StartEffect::Update()
{
	TimeObj* obj = nullptr;
	int id;
	switch(step)
	{
		case StartStep::BEGIN:
			if(effectTimer <= 0)
			{
				obj = nullptr;
				id = 0;
				while((obj = scene->stage->GetObj(id)) != nullptr)
				{
					scene->flag->ReleaseFlag(obj);
					id++;
				}
				step = StartStep::ESCAPE;
				effectTimer = 1;
			}
			else
				effectTimer--;
			break;
		case StartStep::ESCAPE:
			if(effectTimer <= 0)// コウモリ逃げ待ち
			{
				step = StartStep::STORES;
				idx = 1;// 0は基準時計(演出いらない)
				effectTimer = 0;
			}
			else
				effectTimer--;
			break;
		case StartStep::STORES:

			if(effectTimer <= 0 && (obj = scene->stage->GetObj(idx)) != nullptr)
			{
				batList.push_back({Vector2(600 + rand() % 1000 - 500, 350 + rand() % 600 - 300), scene->flag->GetSpeedBlockPos(obj->GetOrginSpeed()),
								  obj->GetOrginSpeed(), 1.0f, 0.0f, 0.0f});
				idx++;
				effectTimer = 10;
			}
			else
				effectTimer--;
			if(batList.empty())
			{
				step = StartStep::SET;
				effectTimer = 1;
				break;
			}
			for(auto it = batList.begin(); it != batList.end();)
			{
				if(it->time >= 1.0f)
				{
					scene->flag->AddSpeedCount(it->speedLabel);
					it = batList.erase(it);
				}
				else
				{
					BatUpdate(&*it);
					it++;
				}
			}
			break;
		case StartStep::SET:
			if(effectTimer <= 0)
			{
				scene->stage->GetObj(0)->SetState(TimeObj::State::SUCCESS);
				scene->flag->AppendGoldFlag(scene->stage->GetObj(0));//基準に金フラッグ
				step = StartStep::END;
			}
			else
				effectTimer--;
			break;
		case StartStep::END:
			// do nothing
			break;
	}
}

void sceneMain::StartEffect::Render()
{
	for(auto& bat : batList)
	{
		float rate = bat.time * bat.time * (3.0f - 2.0f * bat.time);   // 3次関数補間値に変換
		Vector2 offset = Vector2(
			bat.pos.x * (1.0f - rate) + bat.target.x * rate,
			bat.pos.y * (1.0f - rate) + bat.target.y * rate);
		batImage->Render(offset.x, offset.y, batParam.w*bat.scale, batParam.h*bat.scale,
						 batParam.x, batParam.y, batParam.w, batParam.h,
						 0UL, ARGB((int)(bat.alpha * 0xFF), 0xFF, 0xFF, 0xFF));
	}
}

bool sceneMain::StartEffect::IsFinish()
{
	return step == StartStep::END;
}

void sceneMain::StartEffect::BatUpdate(sceneMain::StartEffect::BatEffect* bat)
{
	if(bat->time < 1.0f)
	{
		bat->time += 0.01f;
		if(bat->time > 1.0f)
			bat->time = 1.0f;
	}
	bat->scale -= 0.01f;
	bat->alpha = sinf(bat->time*(PI / 1.0f));
}
