#include	"TimeObject.h"
#include	"../IEX/iextreme.h"
#include	"Utility.h"
#include	"Campus.h"

ImageFactory::~ImageFactory()
{
	for (std::pair<const int, iex2DObj*>& r : imageList)
		SafeDelete(r.second);
}

void ImageFactory::Init()
{
	//	���v
	imageList[CLOCK_BACK] = new iex2DObj("DATA/���֘A/���v.png");
	paramList[CLOCK_BACK] = ImageParam(0, 0, 512, 512);
	imageList[CLOCK_HOUR] = new iex2DObj("DATA/���֘A/���v(�j�j.png");
	paramList[CLOCK_HOUR] = ImageParam(0, 0, 512, 512);
	imageList[CLOCK_MINUTE] = new iex2DObj("DATA/���֘A/���v(�j�j.png");
	paramList[CLOCK_MINUTE] = ImageParam(0, 0, 512, 512);
	//��
	imageList[FRAG_BLACK_CLOSE] = new iex2DObj("DATA/�Q�[�����/jikan-koumori(close).png");
	paramList[FRAG_BLACK_CLOSE] = ImageParam(0, 0, 201, 148);
	imageList[FRAG_BLACK_OPEN] = new iex2DObj("DATA/�Q�[�����/jikan-koumori(Black).png");
	paramList[FRAG_BLACK_OPEN] = ImageParam(0, 0, 230, 144);
	imageList[FRAG_GOLD] = new iex2DObj("DATA/�Q�[�����/jikan-koumori(gold).png");
	paramList[FRAG_GOLD] = ImageParam(0, 0, 230, 144);
	imageList[BUTTON_MENU] = new iex2DObj("DATA/�Q�[�����/���j���[.png");
	paramList[BUTTON_MENU] = ImageParam(0, 0, 123, 58);
	imageList[SPEED_LIST] = new iex2DObj("DATA/�Q�[�����/UI�g.png");
	paramList[SPEED_LIST] = ImageParam(0, 0, 128, 594);
	imageList[SPEED_BLOCK] = new iex2DObj("DATA/�Q�[�����/�X�s�[�h�{�[�h.png");
	paramList[SPEED_BLOCK] = ImageParam(0, 0, 102, 80);
	imageList[GOLD_STOCK] = new iex2DObj("DATA/�Q�[�����/�R�E�����c��.png");
	paramList[GOLD_STOCK] = ImageParam(0, 0, 64, 64);
	imageList[BLOCK_FRAME] = new iex2DObj("DATA/�Q�[�����/blockFrame.png");
	paramList[BLOCK_FRAME] = ImageParam(0, 0, 108, 88);
	///��

	//	�낤����
	imageList[CANDLESTICK] = new iex2DObj("DATA/���֘A/�����^��02.png");
	paramList[CANDLESTICK] = ImageParam(0, 0, 512, 512);
	imageList[CANDLE_BIG] = new iex2DObj("DATA/���֘A/rousokuhutoi.png");
	paramList[CANDLE_BIG] = ImageParam(0, 0, 512, 512);
	imageList[CANDLE_SMALL] = new iex2DObj("DATA/���֘A/rousoku.png");
	paramList[CANDLE_SMALL] = ImageParam(0, 0, 512, 512);
	imageList[CANDLE_FIRE] = new iex2DObj("DATA/���֘A/honoopsd.png");
	paramList[CANDLE_FIRE] = ImageParam(0, 0, 512, 512);
	imageList[CANDLE_FIRE_ANIMATION] = new iex2DObj("DATA/���֘A/honoo.png");
	paramList[CANDLE_FIRE_ANIMATION] = ImageParam(0, 0, 256, 256);
	imageList[CANDLE_SMALL_MELT] = new iex2DObj("DATA/���֘A/�Ƃ��镔��.png");
	paramList[CANDLE_SMALL_MELT] = ImageParam(0, 0, 512, 512);
	imageList[CANDLE_BIG_MELT] = new iex2DObj("DATA/���֘A/�Ƃ��镔���~2.png");
	paramList[CANDLE_BIG_MELT] = ImageParam(0, 0, 512, 512);

	//	���g���m�[��
	imageList[METRONOM_BACK] = new iex2DObj("DATA/���֘A/���g���m�[�����{.png");
	paramList[METRONOM_BACK] = ImageParam(0, 0, 512, 512);
	imageList[METRONOM] = new iex2DObj("DATA/���֘A/���g���m�[��.png");
	paramList[METRONOM] = ImageParam(0, 0, 512, 512);
	imageList[METRONOM_FACE] = new iex2DObj("DATA/���֘A/���g���m�[���@���.png");
	paramList[METRONOM_FACE] = ImageParam(0, 0, 512, 512);
	imageList[METRONOM_NEEDLE] = new iex2DObj("DATA/���֘A/���g���m�[���i�j�j.png");
	paramList[METRONOM_NEEDLE] = ImageParam(0, 0, 512, 512);
	imageList[METRONOM_SPINDLE1] = new iex2DObj("DATA/���֘A/���g���m�[���i���jspeed1.png");
	paramList[METRONOM_SPINDLE1] = ImageParam(0, 0, 512, 512);
	imageList[METRONOM_SPINDLE2] = new iex2DObj("DATA/���֘A/���g���m�[���i���jspeed2.png");
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
	if (s == SUCCESS)SuccessCnt = SUCCESS_EFFECT_TIME;
	state = s;
}
