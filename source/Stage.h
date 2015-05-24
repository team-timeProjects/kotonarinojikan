#pragma once

#include "TimeObject.h"

class iex2DObj;

struct Object
{
public:
	Object() {}
	~Object();
	void Setting(int x, int y, float scale, float speed, int behavior, int gimmick, bool shuffle, int childNum);
	void SetChild(int id, int x, int y);
	void Update();
private:
	enum class TYPE{
		BASE, MOVE
	};
	float x = 0, y = 0;				//オブジェクトの中心座標
	float scale = .0f, speed = .0f;
	TYPE type = TYPE::BASE;
	int childNum = 0;
	int* childX = nullptr;
	int* childY = nullptr;
	bool isLoop = false;
	int nowPosId = 0;
	int behaviorId = 0;			//挙動ID
	int gimmickId = 0;			//ギミックID
	bool canShuffle = false;	//シャッフルフラグ

public:
	void SetLoop(int flg){
		if(flg == 0){
			isLoop = false;
		}else{
			isLoop = true;
		}
	}
};

//*********************************************************************

class StageMNG
{
private:
	enum GimmickID
	{
		NONE,MOVE
	};
	std::list<TimeObj*> objList;
	std::list<Gimmick*> gimmickList;

public:
	StageMNG();
	~StageMNG();
	void LoadStage(const int stageNum);
	void Update();
	void Render();
private:
	int objMax = 0;
	enum TYPE{
		//時計、ろうそく、メトロノーム
		CLOCK, CANDOL, METRO
	};
	TYPE stageType = TYPE::CLOCK;	//ステージタイプ

	int judgeTimer = 0; //強制判定までの時間（秒)
	int judgeNum = 0;	//強制判定回数
};