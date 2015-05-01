#include "ChoicesMgr.h"
#include "../IEX/iextreme.h"
#include "Utility.h"

ChoicesMgr::ChoicesMgr()
{

}
	
ChoicesMgr::~ChoicesMgr()
{
	for (SuperHoge*& r : choicesList)
	{
		SafeDelete(r);
	}
}

void ChoicesMgr::Update()
{
	for (SuperHoge*& r : choicesList)
	{
		r;
	}
	
}

void ChoicesMgr::Render()
{

	for (SuperHoge*& r : choicesList)
	{
		SafeDelete(r);
	}
}
