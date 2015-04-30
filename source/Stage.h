#pragma once
class iex2DObj;

struct Object
{
public:
	Object(){}
	~Object(){}
	void Setting(int x, int y, float scale, float speed);

private:
	int x, y;				//オブジェクトの中心座標
	float scale = .0f, speed = .0f;
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