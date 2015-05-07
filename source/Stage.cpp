#include	"Stage.h"
#include	"TextLoader.h"
#include	<string>
#include	<sstream>

namespace{
	const float SPEED = 0.01f;
	const float OK_LEN = 3.0f;
}

Object::~Object()
{
	delete[] childX;
	childX = nullptr;
	delete[] childY;
	childY = nullptr;
}

void Object::Setting(int x, int y, float scale, float speed, int another, int childNum)
{
	this->x = (float)x; this->y = (float)y;
	this->scale = scale; this->speed = speed;
	this->another = another; this->childNum = childNum+1;
	if(childNum > 1){
		type = TYPE::MOVE;
		childX = new int[childNum];
		childY = new int[childNum];

		childX[0] = x;
		childY[0] = y;
	}
}

void Object::SetChild(int id, int x, int y)
{
	childX[id+1] = x;
	childY[id+1] = y;
}

void Object::Update()
{
	if(childNum <= 1) return;

	int nextPosId = nowPosId+1;
	if(nextPosId > childNum){
		nextPosId = 0;
	}
	float moveX = childX[nextPosId] - childX[nowPosId];
	float moveY = childY[nextPosId] - childY[nowPosId];

	float length = sqrtf(moveX*moveX + moveY*moveY);
	moveX /= length;
	moveY /= length;

	moveX *= SPEED;
	moveY *= SPEED;

	x += moveX;
	y += moveY;

	float diffX = childX[nextPosId];
	float diffY = childY[nextPosId];
	length = diffX*diffX + diffY*diffY;
	if(length < OK_LEN*OK_LEN){
		nowPosId++;
	}
}

//*********************************************************************

StageMNG::StageMNG()
{
}

StageMNG::~StageMNG()
{
	delete tex;
	tex = nullptr;
	delete[] obj;
	obj = nullptr;
}

void StageMNG::LoadStage(const int stageNum)
{
	//情報の破棄
	objMax = 0;
	delete[] obj;
	obj = nullptr;

	//stageNumからファイル名の設定
	std::stringstream buf;
	buf << stageNum;
	//ファイル名共通部分の設定
	std::string fileName = "DATA/BG/stage"+ buf.str();
	TextLoader loader;
	loader.Load((char*)fileName.c_str());

	//個数
	objMax = loader.LoadInt();
	obj = new Object[objMax];
	//設定用変数
	int x, y;
	float scale, speed;
	int another, childNum;
	//設定
	for(int i=0; i<objMax; i++){//x y scale speed その他 子数 ループ 座標...
		x = loader.LoadInt();
		y = loader.LoadInt();
		scale = loader.LoadFloat();
		speed = loader.LoadFloat();
		another = loader.LoadInt();
		childNum = loader.LoadInt();
		obj[i].Setting(x, y, scale, speed, another, childNum);

		if(childNum > 0){
			obj[i].SetLoop(loader.LoadInt());
			for(int j=0; j<childNum; j++){
				x = loader.LoadInt();
				y = loader.LoadInt();
				obj[i].SetChild(j, x, y);
			}
		}
	}
}

void StageMNG::Update()
{
	for(int i=0; i<objMax; i++){
		obj[i].Update();
	}
}

void StageMNG::Render()
{

}