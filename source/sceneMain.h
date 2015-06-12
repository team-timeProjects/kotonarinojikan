#ifndef _SCENEMAIN_H_
#define _SCENEMAIN_H_

#include	"Game.h"
#include	"../EDX/EDXLIB.h"

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
	EDX::EDX_2DObj* back;
	EDX::EDX_2DObj* back2;
	float back2angle;
	float angleSpeed;

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
