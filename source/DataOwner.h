#ifndef _DATAOWNER_H_
#define _DATAOWNER_H_

#include "../IEX/iextreme.h"
#include "Utility.h"
#include "stub.h"

// 全シーンでデータ共有
class DataOwner // singleton
{
	//---------- field ------------
public:
	iexView*		view;
	BackHoge*		back;
	BigClockHoge*	bigClock;
	ClockHoge*		clock;
	StageHoge*		stage;

	//---------- method -----------
public:
	~DataOwner()
	{
		SafeDelete(view);
		SafeDelete(back);
		SafeDelete(bigClock);
		SafeDelete(clock);
		SafeDelete(stage);
	}

	static DataOwner* GetInst()
	{
		static DataOwner Instance;

		return &Instance;
	}

	bool Init()
	{
		view = new iexView();
		back = new BackHoge();
		bigClock = new BigClockHoge();
		clock = new ClockHoge();
		stage = new StageHoge();

		Campus::Inst()->x = 0;
		Campus::Inst()->y = 0;
		Campus::Inst()->scale= 1;
		return true;
	}

private:
	DataOwner() 
	{
		view = nullptr;
	}

	DataOwner(const DataOwner& r) = delete;
	DataOwner& operator=(const DataOwner& r) = delete;
};

#endif //#ifndef _DATAOWNER_H_
