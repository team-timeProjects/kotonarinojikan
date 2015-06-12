#ifndef _SCENEMAIN_H_
#define _SCENEMAIN_H_

#include	"Game.h"
#include	"../IEX/iextreme.h"
#include	"../EDX/EDXLIB.h"
#include	"Utility.h"

class StageMNG;
class FlagMgr;
<<<<<<< HEAD
=======

class JudgeClock
{
private:
	iex2DObj* image;
	POINT	pos;			//　位置
	float	ShortAngle;		//　短針角度
	float	LongAngle;		//	長針角度
	int		timer;
	int		judgeCycle;
	int		judgeCount;
	bool	clockUp;
	bool	checkPalse;
public:
	JudgeClock()
	{
		image = nullptr;
		pos.x = 0;
		pos.y = 0;
		ShortAngle = 0;
		LongAngle = 0;
		timer = 0;
		judgeCycle = 0;
		judgeCount = 0;
		clockUp = false;
		checkPalse = false;
	}
	~JudgeClock()
	{
		SafeDelete(image);
	}
	void Init(int judgeCount, int judgeCycle)
	{
		image = new iex2DObj("DATA/timer.png");
		pos.x = 1220;
		pos.y = 650;
		this->judgeCount = judgeCount;
		this->judgeCycle = judgeCycle * 60;
		timer = this->judgeCount*this->judgeCycle;
		clockUp = false;
		checkPalse = false;
	}

	void Update()
	{
		timer -= clockUp ? 6 : 1;
		checkPalse = timer%judgeCycle == 0 || (clockUp&&timer%judgeCycle > judgeCycle - 6);
		clockUp = false;

		LongAngle = (timer%judgeCycle)*(2 * PI / judgeCycle);
		ShortAngle = (timer / judgeCycle)*(2 * PI / 12) + (timer%judgeCycle)*(2 * PI / (12 * judgeCycle));
	}

	void Render()
	{
		const int size = 110;
		POINT p;
		p.x = pos.x;
		p.y = pos.y;
		image->Render(pos.x - size / 2, pos.y - size / 2, size, size, 0, 0, 256, 256);
		image->Render(pos.x - size / 2, pos.y - size / 2, size, size, 0, 256, 256, 256, p, ShortAngle);
		image->Render(pos.x - size / 2, pos.y - size / 2, size, size, 256, 0, 256, 256, p, LongAngle);

	}
	bool IsCollision(const POINT& p)
	{
		return (p.x >= pos.x - 110 / 2) && (p.x <= pos.x + 110 / 2) &&
			(p.y >= pos.y - 110 / 2) && (p.y <= pos.y + 110 / 2);
	}

	void TimerClockUp()
	{
		clockUp = true;
	}

	bool CheckPalse()
	{
		return checkPalse;
	}

	int GetTime()
	{
		return timer;
	}
};
>>>>>>> remotes/origin/horita

class sceneMain :public Scene
{
	//-------- field ---------
private:
	enum State
	{
<<<<<<< HEAD
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


=======
		BEGIN, MAIN, PAUSE, CHECK, END
	}state;
	StageMNG* stage;
	FlagMgr* flag;
	JudgeClock* judgeClock;
	EDX::EDX_2DObj* back;
	EDX::EDX_2DObj* back2;
	float back2angle;
	float angleSpeed;

	int stageID;// プロット提供用
>>>>>>> remotes/origin/horita

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
