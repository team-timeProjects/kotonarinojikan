#ifndef _SCENEMAIN_H_
#define _SCENEMAIN_H_

#include	"Game.h"
#include	"../IEX/iextreme.h"
#include	"../EDX/EDXLIB.h"
#include	"Utility.h"
#include	<list>

class tomALManager;

class StageMNG;
class FlagMgr;

class JudgeClock
{
private:
	iex2DObj* image;
	POINT	pos;			//@ˆÊ’u
	float	ShortAngle;		//@’ZjŠp“x
	float	LongAngle;		//	’·jŠp“x
	int		timer;			//@§ŒÀŽžŠÔ
	int		judgeCycle;		//@ˆêŽü‚Å‰ñ‚éŽžŠÔiframej
	int		judgeCount;		//@”»’è‰ñ”
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
		image = new iex2DObj("DATA/ƒQ[ƒ€‰æ–Ê/hanteitokei.png");
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
		if (judgeCycle != 0)
		{
			timer -= clockUp ? 6 : 1;
			checkPalse = timer%judgeCycle == 0 || (clockUp&&timer%judgeCycle > judgeCycle - 6);
			clockUp = false;

			LongAngle = -(timer%judgeCycle)*(2 * PI / judgeCycle);
			ShortAngle = -(timer / judgeCycle)*(2 * PI / 12) - (timer%judgeCycle)*(2 * PI / (12 * judgeCycle));
		}
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

class sceneMain :public Scene
{
	//-------- field ---------
private:
	enum State
	{
		BEGIN, START_EFFECT, MAIN, PAUSE, CHECK, END
	}state;
	StageMNG* stage;
	FlagMgr* flag;
	JudgeClock* judgeClock;
	EDX::EDX_2DObj* back;
	EDX::EDX_2DObj* back2;
	float back2angle;
	float angleSpeed;
	int stageID;


	tomALManager* armgr;

	class StartEffect
	{
	private:
		enum StartStep
		{
			BEGIN, ESCAPE, STORES, SET, END
		}step;
		int effectTimer;
		iex2DObj* batImage;
		ImageParam batParam;
		struct BatEffect
		{
			Vector2 pos;
			Vector2 target;
			int speedLabel;
			float scale;
			float alpha;
			float time;
		};
		std::list<BatEffect> batList;
		int idx;
		sceneMain* scene;

	public:
		StartEffect();
		void Init(sceneMain* ref);
		void Update();
		void Render();
		bool IsFinish();
	private:
		void BatUpdate(BatEffect* bat);
	}*startEffect;

	//-------- method ---------
public:
	sceneMain();
	~sceneMain();
	bool Initialize()override;
	void Update()override;
	void Render()override;

};

#endif //#ifndef _SCENEMAIN_H_
