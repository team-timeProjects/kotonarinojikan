#ifndef _DATAOWNER_H_
#define _DATAOWNER_H_

#include "../IEX/iextreme.h"
#include "Utility.h"
#include "Stage.h"
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
	Game*			gameMain;

	//---------- method -----------
public:
	~DataOwner()
	{
		SafeDelete(view);
		SafeDelete(back);
		SafeDelete(bigClock);
		SafeDelete(clock);
		SafeDelete(stage);
		SafeDelete(gameMain);
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
		gameMain = new Game();

		gameMain->Initialize();

		Campus::Inst()->pos.x = 0;
		Campus::Inst()->pos.y = 0;
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
