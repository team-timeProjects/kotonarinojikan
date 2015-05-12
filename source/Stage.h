#pragma once
class iex2DObj;

struct Object
{
public:
	Object() {}
	~Object();
	void Setting(int x, int y, float scale, float speed, int another, int childNum);
	void SetChild(int id, int x, int y);
	void Update();
private:
	enum class TYPE{
		BASE, MOVE
	};
	float x = 0, y = 0;				//オブジェクトの中心座標
	float scale = .0f, speed = .0f;
	TYPE type = TYPE::BASE;
	int another = 0;
	int childNum = 0;
	int* childX = nullptr;
	int* childY = nullptr;
	bool isLoop = false;
	int nowPosId = 0;

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
public:
	StageMNG();
	~StageMNG();
	void LoadStage(const int stageNum);
	void Update();
	void Render();
private:
	iex2DObj* tex = nullptr;
	Object* obj = nullptr;
	int objMax = 0;
};