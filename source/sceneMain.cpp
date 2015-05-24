#include	"iextreme.h"
#include	"system/system.h"

#include	"sceneMain.h"
#include	"DataOwner.h"
#include	"Stage.h"
#include	"ChoicesMgr.h"


sceneMain::sceneMain(void)
{
	stage = nullptr;
}

bool sceneMain::Initialize()
{
	//	ŠÂ‹«İ’è
	iexLight::SetFog(800, 1000, 0);
	//	ŠÂ‹«Œõ
	DataOwner::GetInst()->Init();

	stage = new StageMNG;
	stage->LoadStage(1);
	return true;
}

sceneMain::~sceneMain()
{
	SafeDelete(stage);
}

void sceneMain::Update()
{
	stage->Update();
}

void sceneMain::Render()
{
	//	‰æ–ÊƒNƒŠƒA
	DataOwner::GetInst()->view->Activate();
	DataOwner::GetInst()->view->Clear(0x00000080);

	stage->Render();
}
