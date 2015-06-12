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
	imageList[CLOCK_BACK] = new iex2DObj("DATA/物関連/時計.png");
	paramList[CLOCK_BACK] = ImageParam(0, 0, 512, 512);
	imageList[CLOCK_HOUR] = new iex2DObj("DATA/物関連/時計(針）.png");
	paramList[CLOCK_HOUR] = ImageParam(0, 0, 512, 512);
	imageList[CLOCK_MINUTE] = new iex2DObj("DATA/物関連/時計(針）.png");
	paramList[CLOCK_MINUTE] = ImageParam(0, 0, 512, 512);
	imageList[FRAG_BLACK] = new iex2DObj("DATA/ゲーム画面/-10～10のコウモリたち/koumori.png");
	paramList[FRAG_BLACK] = ImageParam(0, 0, 256, 128);
	imageList[FRAG_GOLD] = new iex2DObj("DATA/ゲーム画面/-10～10のコウモリたち/koumori_kin.png");
	paramList[FRAG_GOLD] = ImageParam(0, 0, 256, 128);
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
	if (s == SUCCESS)SuccessCnt = SUCCESS_EFFECT_TIME;
	state = s;
}
