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
#include	"sceneSelect.h"
#include	"TransitionBat.h"


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

	menubutton = DataOwner::GetInst()->imageFactory->GetImage(ImageFactory::ImageID::BUTTON_MENU);
	menuParam = DataOwner::GetInst()->imageFactory->GetParam(ImageFactory::ImageID::BUTTON_MENU);
	
	//　卍
	menu = new EDX::EDX_2DObj("DATA/ゲーム画面/メニュー.png");
	MenuButton[0] = new EDX::EDX_2DObj("DATA/メニュー画面/ゲームに戻る.png");
	MenuButton[1] = new EDX::EDX_2DObj("DATA/メニュー画面/再チャレンジ.png");
	MenuButton[2] = new EDX::EDX_2DObj("DATA/メニュー画面/ゲームをやめる.png");

	back2angle = 0.0f;
	angleSpeed = 0.01f;
	stage = new StageMNG;
	stageID = DataOwner::GetInst()->stageNo;
	if (!stage->LoadStage(stageID))
	{
		MainFrame->ChangeScene(new sceneTitle());
		return false;
	}
	flag = new FlagMgr;
	flag->Init();
	flag->SetSpeedList(stage->GetSpeedList());
	judgeClock = new JudgeClock;
	judgeClock->Init(stage->GetJudgeNum(), stage->GetjudgeTimer());
	state = BEGIN;
	NextSceneTime = 0;
	Pumpkin::GetInst()->Init();
	Pumpkin::GetInst()->SetOpen(true);
	Pumpkin::GetInst()->SetMaxY(1000);
	direction.Init();
	TransitionBat::GetInst()->SetNextStep(TransitionBat::TBAT_STATE::UP);
	TransitionBat::GetInst()->TimeReset();

	startEffect = new StartEffect;
	startEffect->Init(this);
	PauseBlack = 0;
	PauseZoom = 1.0f;
	PauseCol = 0x00000000;

	Sound::BGM_Play(SOUND::MAIN);

	return true;
}

sceneMain::~sceneMain()
{
	Sound::BGM_Stop(SOUND::MAIN);
	SafeDelete(stage);
	SafeDelete(menu);
	SafeDelete(flag);
	SafeDelete(back);
	SafeDelete(back2);
	SafeDelete(judgeClock);
	SafeDelete(startEffect);
	for (int i = 0; i < 3; i++){
		SafeDelete(MenuButton[i]);
	}
	direction.Finalize();
	Pumpkin::GetInst()->Release();
}

void sceneMain::Update()
{
	TransitionBat::GetInst()->Update();

	back2angle += angleSpeed;
	back2->SetAngle(back2angle);

	Pumpkin::GetInst()->Update();

	//ポーズ
	if (MouseGet(EDX::EDX_CLICK_L) == 1){
		if ((Mouse::cursor.x >= 1160) && (Mouse::cursor.x <= 1160 + 123) &&
			(Mouse::cursor.y >= 530) && (Mouse::cursor.y <= 530 + 58)){
			if (state == MAIN){
				Campus::GetInst()->SetNextPos(GetPoint(0, 0));
				Campus::GetInst()->TimeReset();
				state = PAUSE;
				return;
			}
		}
	}

	//　オブジェクト選択
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

	//　ホイールによるフラグ選択
	if (state == MAIN && (EDX::MouseGet(EDX::EDX_WHEEL)>0 || EDX::MouseGet(EDX::EDX_WHEEL)<0))
	{
		int objID = -1;
		POINT p = AddPoint(Mouse::cursor, Campus::GetInst()->GetPos());
		if ((objID = stage->IsCollision(p)) != -1)
		{
			flag->AppendFlag(stage->GetObj(objID), (EDX::MouseGet(EDX::EDX_WHEEL)<0));
			Campus::GetInst()->TimeReset();
		}
	}

	//　強制判定時計の長押し
	if (MouseGet(EDX_CLICK_L) && state == MAIN)
	{
		POINT p;
		p = Mouse::cursor;
		if (judgeClock->IsCollision(p))
		{
			judgeClock->TimerClockUp();
			if (judgeClock->GetTime() == 0)
				state = CHECK;
		}
	}

	//　金フラッグ
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

	switch(state)
	{
		case sceneMain::BEGIN:
			//stage->Update();
			flag->Update();
			//break;
			if (Pumpkin::GetInst()->IsMoveEnd())
				state = START_EFFECT;
			Campus::GetInst()->SetPos(stage->GetPos(stage->GetNowObj()));
			Campus::GetInst()->Update();
			break;

		case sceneMain::START_EFFECT:
			if (startEffect->IsFinish())
			{
				state = MAIN;
				SafeDelete(startEffect);
			}
			else
				startEffect->Update();
			//stage->Update();
			flag->Update();
			break;

		case sceneMain::MAIN:

			PauseBlack -= 10;
			if (PauseBlack< 0)PauseBlack = 0;
			PauseCol = ARGB(PauseBlack, 0, 0, 0);
			for (int i = 0; i < 3; i++){
				MenuButton[i]->SetARGB(PauseBlack * 2, 255, 255, 255);
			}
			PauseZoom += 0.1f;
			if (PauseZoom>1.0f)PauseZoom = 1.0f;
			Campus::GetInst()->SetZoom(PauseZoom);

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
				stage->SetChecking(true);
				flag->StartCheck();
				state = CHECK;
			}
			else if(judgeClock->GetTime() <= 0)
			{
				Sound::BGM_Stop(SOUND::MAIN);
				Sound::SE_Play(SOUND::MISS);

				state = OVER;
				direction.SetState(GameDirection::State::OVER);
				OverScene = new sceneMain;
			}
			break;

		case sceneMain::PAUSE:
			PauseBlack += 10;
			if (PauseBlack > 127)PauseBlack = 127;
			PauseCol = ARGB(PauseBlack, 0, 0, 0);

			for (int i = 0; i < 3; i++){
				MenuButton[i]->SetARGB(PauseBlack * 2, 255, 255, 255);
			}

			//左
			if (MouseGet(EDX::EDX_CLICK_L) == 1){
				if ((Mouse::cursor.x >= 230) && (Mouse::cursor.x <= 230 + 256) &&
					(Mouse::cursor.y >= 500) && (Mouse::cursor.y <= 500 + 256)){
					Campus::GetInst()->SetNextPos(GetPoint(0, 0));
					Campus::GetInst()->TimeReset();
					state = MAIN;
				}
				//真ん中
				if ((Mouse::cursor.x >= 230 + 256) && (Mouse::cursor.x <= 230 + 256 + 256) &&
					(Mouse::cursor.y >= 500) && (Mouse::cursor.y <= 500 + 256)){
					NextSceneTime = 110;
					state = OVER;
					OverScene = new sceneMain;
				}
				//右(やめる)
				if ((Mouse::cursor.x >= 230 + 512) && (Mouse::cursor.x <= 230 + 512 + 256) &&
					(Mouse::cursor.y >= 500) && (Mouse::cursor.y <= 500 + 256)){

					Sound::BGM_Stop(SOUND::MAIN);
					Sound::SE_Play(SOUND::MISS);

					state = OVER;
					direction.SetState(GameDirection::State::OVER);
					OverScene = new sceneSelect(stageID - 1);
				}
			}

			PauseZoom -= 0.1f;
			if (PauseZoom < 0.3f)PauseZoom = 0.3f;

			Campus::GetInst()->SetZoom(PauseZoom);
			Campus::GetInst()->Update();
			Campus::GetInst()->Update();
			Campus::GetInst()->Update();

			break;
		
		case sceneMain::CHECK:
			if(flag->IsCheckEnd())
			{
				if(flag->IsClear())
				{
					//　ステージ解放
					if (stageID == DataOwner::GetInst()->OpenStage)
					DataOwner::GetInst()->OpenStage++;
					DataOwner::GetInst()->ofs.open("DATA/SaveData.txt");
					DataOwner::GetInst()->ofs << DataOwner::GetInst()->OpenStage;

					Sound::BGM_Stop(SOUND::MAIN);
					Sound::SE_Play(SOUND::GOOD);
					state = CLEAR;	
					direction.SetState(GameDirection::State::CLEAR);
				}
				else
				{
					Campus::GetInst()->SetNextPos(stage->GetPos(stage->GetNowObj()));
					Campus::GetInst()->TimeReset();
					stage->SetChecking(false);
					//　ミスった数×１０秒マイナス
					judgeClock->DecreaseTimer(10 * flag->GetMissCount());
					state = MAIN;
				}
			}
			stage->Update();
			flag->Update();
			Campus::GetInst()->Update();
			break;
		
		case sceneMain::CLEAR:
			NextSceneTime++;
			if (NextSceneTime == 120){
				TransitionBat::GetInst()->SetStep(TransitionBat::TBAT_STATE::DOWN);
				TransitionBat::GetInst()->SetNextStep(TransitionBat::TBAT_STATE::CENTER);
				TransitionBat::GetInst()->TimeReset();
			}
			if (NextSceneTime > 180){
				if (stageID >= DataOwner::GetInst()->STAGE_MAX - 1){
					MainFrame->ChangeScene(new sceneMain);
					return;
				}
				else{
					DataOwner::GetInst()->stageNo++;
					MainFrame->ChangeScene(new sceneMain);
					return;
				}
			}
			break;
		
		case sceneMain::OVER:
		{static bool isShaked = false;
		NextSceneTime++;
		if (NextSceneTime == 120){
			Pumpkin::GetInst()->SetOpen(false);
			Pumpkin::GetInst()->SetMaxY();
			TransitionBat::GetInst()->SetStep(TransitionBat::TBAT_STATE::UP);
			//TransitionBat::GetInst()->SetNextStep(TransitionBat::TBAT_STATE::CENTER);
			//TransitionBat::GetInst()->TimeReset();
			isShaked = false;
		}
		if (!isShaked&&Pumpkin::GetInst()->IsMoveEnd()){ Pumpkin::GetInst()->ShakeStart(); isShaked = true; }

		if (NextSceneTime > 180){
			MainFrame->ChangeScene(OverScene);
			return;
		}
		}	break;
		default:
			break;
	}
	flag->SetHaveGoldFlag(stage->GetHaveGoldFlag());
	//卍
	TimeObj::ResetChain();
	direction.Update();

}

void sceneMain::Render()
{
	//	画面クリア
	DataOwner::GetInst()->view->Activate();
	DataOwner::GetInst()->view->Clear(0x00000080);

	back->Draw(0, 0);
	back2->Draw(0, 0);
	
	stage->Render();
	flag->Render();
	judgeClock->Render();
	menu->Draw(1160, 530);
	direction.Draw();
	menubutton->Render(1155, 530, menuParam.w, menuParam.h, menuParam.x, menuParam.y, menuParam.w, menuParam.h);
	Pumpkin::GetInst()->Render();
	TransitionBat::GetInst()->Render();
	if (state == State::START_EFFECT)
		startEffect->Render();

	
	if (state == State::PAUSE){
		DrawBox(0, 0, PauseCol, 1280, 0, PauseCol, 0, 720, PauseCol, 1280, 720, PauseCol);

		for (int i = 0; i < 3; i++){
			MenuButton[i]->Draw(i * 256 + 230, 500);
		}
	}


#ifdef _DEBUG
	/*char	str[64];
	wsprintf(str, "Stage: %d\n", stageID);
	IEX_DrawText(str, 10, 30, 200, 20, 0xFF7070FF);
	wsprintf(str, "state = %d", state);
	IEX_DrawText(str, 10, 50, 200, 20, 0xFF7070FF);
	wsprintf(str, "timelimit = %d", judgeClock->GetTime() / 60);
	IEX_DrawText(str, 10, 70, 200, 20, 0xFF7070FF);
	wsprintf(str, "missCount = %d", flag->GetMissCount());
	IEX_DrawText(str, 10, 90, 200, 20, 0xFFFFFFFF);*/
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
	batImage = DataOwner::GetInst()->imageFactory->GetImage(ImageFactory::FRAG_BLACK_OPEN);
	batParam = DataOwner::GetInst()->imageFactory->GetParam(ImageFactory::FRAG_BLACK_OPEN);
	std::map<int, int> speed = scene->stage->GetSpeedList();
	for (auto& r : speed)
		r.second = 0;
	scene->flag->SetSpeedList(speed);
	TimeObj* obj = nullptr;
	int id = 0;
	while ((obj = scene->stage->GetObj(id)) != nullptr)
	{
		scene->flag->AppendPlainFlag(obj);
		id++;
	}
	idx = 0;
	ref->PauseZoom = 0.3f;
	Campus::GetInst()->SetZoom(ref->PauseZoom);
}

void sceneMain::StartEffect::Update()
{
	TimeObj* obj = nullptr;
	int id;
	switch (step)
	{
	case StartStep::BEGIN:
		scene->PauseZoom = 0.3f;
		if (effectTimer <= 0)
		{
			obj = nullptr;
			id = 0;
			while ((obj = scene->stage->GetObj(id)) != nullptr)
			{
				scene->flag->ReleaseFlag(obj);
				id++;
			}
			step = StartStep::ESCAPE;
			effectTimer = 60;
		}
		else
			effectTimer--;
		break;
	case StartStep::ESCAPE:
		if (effectTimer <= 0)// コウモリ逃げ待ち
		{
			step = StartStep::STORES;
			idx = 1;// 0は基準時計(演出いらない)
			effectTimer = 0;
		}
		else
			effectTimer--;
		scene->PauseZoom += 0.01f;
		if (scene->PauseZoom > 1.0f)
		{
			scene->PauseZoom = 1.0f;
		}
		Campus::GetInst()->SetZoom(scene->PauseZoom);
		Campus::GetInst()->Update();
		break;
	case StartStep::STORES:

		if (effectTimer <= 0 && (obj = scene->stage->GetObj(idx)) != nullptr)
		{
			batList.push_back({ Vector2(600 + rand() % 1000 - 500, 350 + rand() % 600 - 300), scene->flag->GetSpeedBlockPos(obj->GetOrginSpeed()),
				obj->GetOrginSpeed(), 1.0f, 0.0f, 0.0f });
			idx++;
			effectTimer = 10;
		}
		else
			effectTimer--;
		if (batList.empty())
		{
			step = StartStep::SET;
			effectTimer = 120;
			break;
		}
		for (auto it = batList.begin(); it != batList.end();)
		{
			if (it->time >= 1.0f)
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
		scene->PauseZoom += 0.01f;
		if (scene->PauseZoom > 1.0f)
		{
			scene->PauseZoom = 1.0f;
		}
		Campus::GetInst()->SetZoom(scene->PauseZoom);
		Campus::GetInst()->Update();
		break;
	case StartStep::SET:
		if (effectTimer <= 0)
		{
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
	for (auto& bat : batList)
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
	if (bat->time < 1.0f)
	{
		bat->time += 0.01f;
		if (bat->time > 1.0f)
			bat->time = 1.0f;
	}
	bat->scale -= 0.01f;
	bat->alpha = sinf(bat->time*(PI / 1.0f));
}