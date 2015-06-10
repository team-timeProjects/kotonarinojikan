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

	struct CheckClock
	{
		POINT	pos;			//　位置
		float	ShortAngle;		//　短針角度
		float	LongAngle;		//	長針角度
	};
	iex2DObj*	rCheckClock;
	
	StageMNG* stage;
	FlagMgr* flag;
	CheckClock	check_obj;	//　判定時計構造体
	int judgeNum = 0;
	int judgeTimer = 0;
	int stageID;// プロット提供用
	iex2DObj* back;// プロット提供用

	static int timelimit;



	//-------- method ---------
public:
	sceneMain();
	~sceneMain();
	bool Initialize()override;
	void Update()override;
	void Render()override;

	int GetTimelimit(){ return timelimit; }
	static void SetTimelimit(int tl){ timelimit = tl; }
	static void DecreaseTimit(int minus){ SetTimelimit(timelimit - minus); }
};

#endif //#ifndef _SCENEMAIN_H_
