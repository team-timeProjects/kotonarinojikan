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
	iexView*		view;
	ImageFactory*   imageFactory;

	//---------- method -----------
public:
	~DataOwner()
	{
		SafeDelete(view);
		SafeDelete(imageFactory);
	}

	static DataOwner* GetInst()
	{
		static DataOwner Instance;

		return &Instance;
	}

	bool Init()
	{
		view = new iexView();
		imageFactory = new ImageFactory;

		imageFactory->Init();
		return true;
	}

private:
	DataOwner() 
	{
		view = nullptr;
		imageFactory = nullptr;
	}

	DataOwner(const DataOwner& r) = delete;
	DataOwner& operator=(const DataOwner& r) = delete;
};

#endif //#ifndef _DATAOWNER_H_
