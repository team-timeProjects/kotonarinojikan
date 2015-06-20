#include	"TimeObject.h"
#include	"../IEX/iextreme.h"
#include	"Utility.h"
#include	"Campus.h"

ImageFactory::~ImageFactory()
{
	for(std::pair<const int, iex2DObj*>& r : imageList)
		SafeDelete(r.second);
}

void ImageFactory::Init()
{
	//	時計
	imageList[CLOCK_BACK] = new iex2DObj("DATA/物関連/時計.png");
	paramList[CLOCK_BACK] = ImageParam(0, 0, 512, 512);
	imageList[CLOCK_HOUR] = new iex2DObj("DATA/物関連/時計(針）.png");
	paramList[CLOCK_HOUR] = ImageParam(0, 0, 512, 512);
	imageList[CLOCK_MINUTE] = new iex2DObj("DATA/物関連/時計(針）.png");
	paramList[CLOCK_MINUTE] = ImageParam(0, 0, 512, 512);
	//Δ
	imageList[FRAG_BLACK_CLOSE] = new iex2DObj("DATA/ゲーム画面/jikan-koumori(close).png");
	paramList[FRAG_BLACK_CLOSE] = ImageParam(0, 0, 256, 128);
	imageList[FRAG_BLACK_OPEN] = new iex2DObj("DATA/ゲーム画面/jikan-koumori(Black).png");
	paramList[FRAG_BLACK_OPEN] = ImageParam(0, 0, 256, 128);
	imageList[FRAG_GOLD] = new iex2DObj("DATA/ゲーム画面/jikan-koumori(gold).png");
	paramList[FRAG_GOLD] = ImageParam(0, 0, 256, 128);
	imageList[BUTTON_MENU] = new iex2DObj("DATA/ゲーム画面/メニュー.png");
	paramList[BUTTON_MENU] = ImageParam(0, 0, 128, 64);
	imageList[SPEED_LIST] = new iex2DObj("DATA/ゲーム画面/UI枠.png");
	paramList[SPEED_LIST] = ImageParam(0, 0, 512, 1024);
	imageList[SPEED_BLOCK] = new iex2DObj("DATA/ゲーム画面/スピードボード.png");
	paramList[SPEED_BLOCK] = ImageParam(0, 0, 100, 80);
	imageList[GOLD_STOCK] = new iex2DObj("DATA/ゲーム画面/コウモリ残数.png");
	paramList[GOLD_STOCK] = ImageParam(0, 0, 64, 64);
	///Δ

	//	ろうそく
	imageList[CANDLE_BACK] = new iex2DObj("DATA/和紙.png");
	paramList[CANDLE_BACK] = ImageParam(0, 0, 2048, 720);
	imageList[CANDLE_BIG] = new iex2DObj("DATA/ろうそく(大).png");
	paramList[CANDLE_BIG] = ImageParam(0, 120, 512, 402);
	imageList[CANDLE_SMALL] = new iex2DObj("DATA/ろうそく.png");
	paramList[CANDLE_SMALL] = ImageParam(0, 110, 512, 402);
	imageList[CANDLE_SMALL_FIRE] = new iex2DObj("DATA/ろうそく(火).png");
	paramList[CANDLE_SMALL_FIRE] = ImageParam(0, 0, 512, 512);
	imageList[CANDLE_SMALL_MELT] = new iex2DObj("DATA/ろうそく(溶け).png");
	paramList[CANDLE_SMALL_MELT] = ImageParam(0, 0, 512, 512);
	imageList[CANDLE_SMALL_LIGHT] = new iex2DObj("DATA/ろうそく（明かり）.png");
	paramList[CANDLE_SMALL_LIGHT] = ImageParam(0, 0, 512, 512);
	imageList[CANDLE_BIG_FIRE] = new iex2DObj("DATA/ろうそく(火大).png");
	paramList[CANDLE_BIG_FIRE] = ImageParam(0, 0, 512, 512);
	imageList[CANDLE_BIG_LIGHT] = new iex2DObj("DATA/ろうそく（明かり色違い）.png");
	paramList[CANDLE_BIG_LIGHT] = ImageParam(0, 0, 512, 512);
	imageList[CANDLE_BIG_MELT] = new iex2DObj("DATA/ろうそく(溶け大).png");
	paramList[CANDLE_BIG_MELT] = ImageParam(0, 0, 512, 512);

	//	メトロノーム
	imageList[METRONOM] = new iex2DObj("DATA/物関連/メトロノーム.png");
	paramList[METRONOM] = ImageParam(0, 0, 512, 512);
	imageList[METRONOM_FACE] = new iex2DObj("DATA/物関連/メトロノーム　顔面.png");
	paramList[METRONOM_FACE] = ImageParam(0, 0, 512, 512);
	imageList[METRONOM_NEEDLE] = new iex2DObj("DATA/物関連/メトロノーム（針）.png");
	paramList[METRONOM_NEEDLE] = ImageParam(0, 0, 512, 512);
	imageList[METRONOM_SPINDLE1] = new iex2DObj("DATA/物関連/メトロノーム（錘）speed1.png");
	paramList[METRONOM_SPINDLE1] = ImageParam(0, 0, 512, 512);
	imageList[METRONOM_SPINDLE2] = new iex2DObj("DATA/物関連/メトロノーム（錘）speed2.png");
	paramList[METRONOM_SPINDLE2] = ImageParam(0, 0, 512, 512);
}

iex2DObj* ImageFactory::GetImage(ImageID id)
{
	return imageList[id];
}

ImageParam ImageFactory::GetParam(ImageID id)
{
	return paramList[id];
}

int TimeObj::SuccessChain = 0;
TimeObj::TimeObj()
{}

TimeObj::~TimeObj()
{}

void TimeObj::AppendImage(int idx, iex2DObj* image, const ImageParam& param)
{
	imageList[idx] = image;
	dst[idx] = ImageParam();
	src[idx] = param;
}

int TimeObj::GetID()const
{
	return id;
}

const Vector2& TimeObj::GetPos()const
{
	return pos;
}

void TimeObj::SetPos(const Vector2& pos)
{
	this->pos = pos;
}

void TimeObj::SetOrginSpeed(float speed)
{
	orginSpeed = speed;
}

float TimeObj::GetOrginSpeed()
{
	return orginSpeed;
}

void TimeObj::SetRelativeSpeed(float orginSpeed)
{
	speed = this->orginSpeed / orginSpeed;
}

bool TimeObj::IsCollision(const POINT& p)
{
	return (p.x >= pos.x - colw*scale / 2) && (p.x <= pos.x + colw*scale / 2) &&
		(p.y >= pos.y - colh*scale / 2) && (p.y <= pos.y + colh*scale / 2);
}

void TimeObj::Init(int id, const Vector2& centerPos, int colW, int colH, float scale, float orginSpeed, int behavior)
{
	this->id = id;
	pos = centerPos;
	colw = colW;
	colh = colH;
	orginSpeed = orginSpeed;
	this->scale = scale;
	this->orginSpeed = orginSpeed;
	this->behavior = behavior;
	state = State::MOVE;
}

void TimeObj::SetState(TimeObj::State s)
{
	if(s == SUCCESS)SuccessCnt = SUCCESS_EFFECT_TIME;
	state = s;
}
