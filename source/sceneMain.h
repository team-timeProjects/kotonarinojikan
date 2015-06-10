#ifndef _SCENEMAIN_H_
#define _SCENEMAIN_H_

#include	"Game.h"
class StageMNG;
class FlagMgr;

class sceneMain :public Scene
{
	//-------- field ---------
private:
	enum State
	{
		BEGIN,MAIN,PAUSE,CHECK,END
	}state;
	StageMNG* stage;
	FlagMgr* flag;
	iex2DObj* back;

	int stageID;// プロット提供用

	//-------- method ---------
public:
	sceneMain();
	~sceneMain();
	bool Initialize()override;
	void Update()override;
	void Render()override;

};

#endif //#ifndef _SCENEMAIN_H_
