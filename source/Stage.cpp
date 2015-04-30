#include	"Stage.h"
#include	"iextreme.h"
#include	"TextLoader.h"
#include	<string>
#include	<sstream>
#include	<assert.h>

void Object::Setting(int x, int y, float scale, float speed)
{
	this->x = x;	this->y = y;
	this->scale = scale; this->speed = speed;
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
	std::string fileName = "DATA/BG/stage"+ buf.str() +".sdt";
	TextLoader loader;
	if( !loader.Load((char*)fileName.c_str()) ){
		assert(!"ステージファイルが開けません（存在しません）");
		return;
	}

	//個数
	objMax = loader.LoadInt();
	obj = new Object[objMax];
	//設定用変数
	int x, y;
	float scale, speed;
	//設定
	for(int i=0; i<objMax; i++){
		x = loader.LoadInt(); y = loader.LoadInt();
		scale = loader.LoadFloat(); speed = loader.LoadFloat();

		obj[i].Setting(x, y, scale, speed);
	}
}

void StageMNG::Update()
{

}

void StageMNG::Render()
{

}