#ifndef _DATAOWNER_H_
#define _DATAOWNER_H_

#include "../IEX/iextreme.h"
#include "Utility.h"
#include "Stage.h"
#include "ChoicesMgr.h"
#include "TimeObject.h"

// 全シーンでデータ共有
class DataOwner // singleton
{
	//---------- field ------------
public:
	int				stageNo=0;
	iexView*		view;
	ImageFactory*   imageFactory;
	Number*			number;

	//---------- method -----------
public:
	~DataOwner()
	{
		SafeDelete(view);
		SafeDelete(imageFactory);
		SafeDelete(number);
	}

	static DataOwner* GetInst()
	{
		static DataOwner Instance;

		return &Instance;
	}

	bool Init()
	{
		SafeDelete(view);
		SafeDelete(imageFactory);
		SafeDelete(number);

		stageNo = 0;
		view = new iexView();
		imageFactory = new ImageFactory();
		number = new Number();

		imageFactory->Init();
		number->Init();
		return true;
	}

private:
	DataOwner() 
	{
		view = nullptr;
		imageFactory = nullptr;
		number = nullptr;
	}

	DataOwner(const DataOwner& r) = delete;
	DataOwner& operator=(const DataOwner& r) = delete;
};

#endif //#ifndef _DATAOWNER_H_
