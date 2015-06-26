#ifndef _DATAOWNER_H_
#define _DATAOWNER_H_

#include "../IEX/iextreme.h"
#include "Utility.h"
#include "Stage.h"
#include "ChoicesMgr.h"
#include "TimeObject.h"
#include <fstream>
#include <iostream>

// �S�V�[���Ńf�[�^���L
class DataOwner // singleton
{
	//---------- field ------------
public:
	static const int STAGE_MAX = 60;
	int				stageNo=0;
	iexView*		view;
	ImageFactory*   imageFactory;
	Number*			number;

	int		OpenStage;		//�@�������Ă�X�e�[�W��
	std::ofstream	ofs;	//�@�t�@�C���f�[�^�������ݗp
	std::ifstream	ifs;	//�@�t�@�C���f�[�^�ǂݍ��ݗp
		
	//---------- method -----------
public:
	~DataOwner()
	{
		SafeDelete(view);
		SafeDelete(imageFactory);
		SafeDelete(number);
		if (ofs) ofs.close();
		if (ifs) ifs.close();
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

		stageNo = 1;
		view = new iexView();
		imageFactory = new ImageFactory();
		number = new Number();

		imageFactory->Init();
		number->Init();

		//OpenStage = 1;

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
