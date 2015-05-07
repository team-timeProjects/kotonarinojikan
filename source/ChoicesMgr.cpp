#include "ChoicesMgr.h"
#include "../IEX/iextreme.h"
#include "TimeObject.h"
#include "Utility.h"

ChoicesMgr::ChoicesMgr()
{

}
	
ChoicesMgr::~ChoicesMgr()
{
	for (TimeObject*& r : choicesList)
	{
		SafeDelete(r);
	}
}

void ChoicesMgr::Update()
{
	for (TimeObject*& r : choicesList)
	{
		r->Update();
	}
	
}

void ChoicesMgr::Render()
{

	for (TimeObject*& r : choicesList)
	{
		r->Render();
	}
}
