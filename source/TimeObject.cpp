#include	"TimeObject.h"
#include	"../IEX/iextreme.h"
#include	"Utility.h"
#include	"Campus.h"
#include	"DataOwner.h"

ImageFactory::~ImageFactory()
{
	for(std::pair<const int, iex2DObj*>& r : imageList)
		SafeDelete(r.second);
}

void ImageFactory::Init()
{
	imageList[CLOCK_BACK] = new iex2DObj("DATA/アナログ時計.png");
	paramList[CLOCK_BACK] = ImageParam(0, 0, 512, 512);
	imageList[CLOCK_HOUR] = new iex2DObj("DATA/アナログ時計(短い針）.png");
	paramList[CLOCK_HOUR] = ImageParam(0, 0, 512, 512);
	imageList[CLOCK_MINUTE] = new iex2DObj("DATA/アナログ時計(長い針）.png");
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
	state = s;
}

Gimmick::Gimmick()
{
	obj = nullptr;
}

Gimmick::Gimmick(TimeObj* obj)
{
	this->obj = obj;
}

Gimmick::~Gimmick()
{

}

MoveGmk::MoveGmk():SPEED(2.0f)
{
	nowNode = node.end();
	ringLoop = false;
}

MoveGmk::MoveGmk(TimeObj* obj): MoveGmk()
{
	this->obj = obj;
}

void MoveGmk::Update()
{
	float moveX = nowNode->x - obj->GetPos().x;
	float moveY = nowNode->y - obj->GetPos().y;
	float len = sqrtf(moveX*moveX + moveY*moveY);
	if(len <= SPEED * 2)// 移動ポイントに到達
	{
		if(ringLoop)// ループ方法：リングループ
		{
			nowNode++;
			if(nowNode == node.end())
				nowNode = node.begin();
		}
		else// ループ方法：折り返しループ
		{
			if(searchOrder)// イテレータ：順方向
			{
				nowNode++;
				if(nowNode == node.end())// 末尾到達
				{
					nowNode--;
					searchOrder = false;
				}
			}
			else// イテレータ：逆方向
			{
				if(nowNode == node.begin())// 先頭到達
				{
					nowNode++;
					searchOrder = true;
				}
				else
					nowNode--;
			}
		}
		// ベクトル再計算
		moveX = nowNode->x - obj->GetPos().x;
		moveY = nowNode->y - obj->GetPos().y;
		len = sqrtf(moveX*moveX + moveY*moveY);
	}
	moveX /= len;
	moveY /= len;
	moveX *= SPEED;
	moveY *= SPEED;

	Vector2 newPos;
	newPos.x = obj->GetPos().x + moveX;
	newPos.y = obj->GetPos().y + moveY;


	obj->SetPos(newPos);
}

void MoveGmk::Render()
{
	// do nothing
}

void MoveGmk::AppendNode(const Vector2& node)
{
	this->node.push_back(node);
	nowNode = this->node.begin();
}

void MoveGmk::SetLoop(bool IsRingLoop)
{
	ringLoop = IsRingLoop;
	searchOrder = true;
}

FlagGmk::FlagGmk(TimeObj* timeObj):Gimmick(timeObj)
{
	back = nullptr;
	number = nullptr;
}

FlagGmk::~FlagGmk() {}

void FlagGmk::Init(iex2DObj* back, ImageParam backParam, Number* number)
{
	this->back = back;
	this->backParam = backParam;
	this->number = number;
	pos = Vector2(0, 0);
	num = 0;
}

void FlagGmk::Update()
{
	pos = obj->GetPos();
}

void FlagGmk::Render()
{
	Campus::GetInst()->Add(back, pos.x - backParam.w / 2 * 0.7f, pos.y - backParam.h * 2 + 30, backParam.w*0.7f, backParam.h*0.7f,
						   backParam.x, backParam.y, backParam.w, backParam.h);
	number->Render(num, pos.x - 10, pos.y - backParam.h * 2 + 70, 1.0f);
}

void FlagGmk::SetNum(int num)
{
	this->num = num;
}

int FlagGmk::GetNum()
{
	return num;
}

void FlagGmk::SetPos(const Vector2& pos)
{
	this->pos = pos;
}

bool FlagGmk::IsChecked()
{
	return checked;
}

void FlagGmk::SetChecked(bool checked)
{
	this->checked = checked;
}

FlagGmk::TYPE FlagGmk::GetType()
{
	return type;
}

void FlagGmk::SetType(FlagGmk::TYPE type)
{
	this->type = type;
}

void FlagGmk::SetBack(iex2DObj* back, ImageParam backParam)
{
	this->back = back;
	this->backParam = backParam;
}

FlagMgr::FlagMgr()
{
	listBack = nullptr;
	blockBack = nullptr;
	goldBat = nullptr;
	MissCount = 0;
}

FlagMgr::~FlagMgr()
{
	SafeDelete(listBack);
	SafeDelete(blockBack);
	for(std::pair<TimeObj* const, FlagGmk*>& r : flagList)
		SafeDelete(r.second);
}

void FlagMgr::Init()
{
	listBack = new iex2DObj("DATA/ゲーム画面/UI枠.png");
	blockBack = new iex2DObj("DATA/ゲーム画面/スピードボード.png");
	goldBat = new iex2DObj("DATA/ゲーム画面/コウモリ残数.png");
}

void FlagMgr::Update()
{
	for(std::pair<TimeObj* const, FlagGmk*>& r : flagList)
		r.second->Update();
}

void FlagMgr::Render()
{
	for(std::pair<TimeObj* const, FlagGmk*>& r : flagList)
		r.second->Render();
	Campus::GetInst()->Draw();
	listBack->Render(1102, 0, 180, 720, 50, 108, 154, 722);
	goldBat->Render(1152, 530, 80, 50, 0, 0, 64, 64);
	int offsety = 85;
	for(std::pair<const int, int>& r : speedList)
	{
		blockBack->Render(1152, offsety, 116, 72, 0, 0, 100, 80);
		DataOwner::GetInst()->number->Render(r.first, 1152 + 60, offsety + 30, 0.3f, false);
		DataOwner::GetInst()->number->Render(r.second, 1152 + 50, offsety + 50, 0.5f, false);
		offsety += 72;
	}

}

void FlagMgr::SetSpeedList(const std::map<int, int>& list)
{
	speedList = list;
}

void FlagMgr::AppendFlag(TimeObj* obj, bool next)
{
	int speed = 0;
	if(flagList.find(obj) != flagList.end())
	{
		if(flagList[obj]->GetType() == FlagGmk::GOLD)return;
		speed = flagList[obj]->GetNum();
		SafeDelete(flagList[obj]);
		flagList.erase(obj);
	}
	if(next)
		speed = NextSpeed(speed);
	else
		speed = BeforeSpeed(speed);
	if(speed == 0)return;

	FlagGmk* flg = new FlagGmk(obj);
	flg->Init(DataOwner::GetInst()->imageFactory->GetImage(ImageFactory::FRAG_BLACK),
			  DataOwner::GetInst()->imageFactory->GetParam(ImageFactory::FRAG_BLACK),
			  DataOwner::GetInst()->number);
	flg->SetNum(speed);
	flg->SetType(FlagGmk::BLACK);
	flagList[obj] = flg;
}

bool FlagMgr::StartCheck()
{
	for(std::pair<TimeObj*const, FlagGmk*>& r : flagList)
	{
		if(r.second->GetType() == FlagGmk::TYPE::GOLD)continue;
		r.first->SetState(TimeObj::State::CHECK);
		r.second->SetChecked(false);
	}
	for(std::pair<TimeObj*const, FlagGmk*>& r : flagList)
	{
		if(r.second->GetType() == FlagGmk::TYPE::GOLD)continue;
		nowObj = r.first;
		return true;
	}
	return false;
}

bool FlagMgr::CheckNext()
{
	for(auto r = flagList.begin(); r != flagList.end(); r++)
	{
		if(r->first == nowObj)
		{
			while(r != flagList.end() && (r->first == nowObj || r->second->GetType() == FlagGmk::TYPE::GOLD))
				r++;
			if(r != flagList.end())
			{
				nowObj = r->first;
				return true;
			}
			else
				return false;
		}
	}
	return false;
}

void FlagMgr::CheckFlag()
{
	if(abs(nowObj->GetOrginSpeed() - flagList[nowObj]->GetNum()) < 0.1f)
	{
		speedList[flagList[nowObj]->GetNum()]--;
		flagList[nowObj]->SetBack(DataOwner::GetInst()->imageFactory->GetImage(ImageFactory::FRAG_GOLD),
								  DataOwner::GetInst()->imageFactory->GetParam(ImageFactory::FRAG_GOLD));
		flagList[nowObj]->SetType(FlagGmk::GOLD);
		nowObj->SetState(TimeObj::State::STOP);
	}
	else
	{
		MissCount++;
	}
	nowObj->SetState(TimeObj::State::MOVE);
	flagList[nowObj]->SetChecked(true);
}

bool FlagMgr::IsFinishEffect()
{
	return true;
}

POINT FlagMgr::GetNowObjPos()
{
	POINT ret;
	ret.x = nowObj->GetPos().x;
	ret.y = nowObj->GetPos().y;
	return ret;
}

inline int FlagMgr::NextSpeed(int nowSpeed)
{
	for(auto r = speedList.begin(); r != speedList.end(); r++)
	{
		if(r->first == nowSpeed || nowSpeed == 0)
		{
			while(r != speedList.end() && (r->first == nowSpeed || r->second == 0))
				r++;
			return r != speedList.end() ? r->first : 0;
		}
	}
	return 0;
}

inline int FlagMgr::BeforeSpeed(int nowSpeed)
{
	for(auto r = speedList.rbegin(); r != speedList.rend(); r++)
	{
		if(r->first == nowSpeed || nowSpeed == 0)
		{
			while(r != speedList.rend() && (r->first == nowSpeed || r->second == 0))
			{
				r++;
			}
			return r != speedList.rend() ? r->first : 0;
		}
	}
	return 0;
}

//TimeObject::TimeObject() :TimeObject(0, 0)
//{
//
//}
//
//TimeObject::TimeObject(int sw, int sh) : SOURCE_W(sw), SOURCE_H(sh)
//{
//
//}
//
//TimeObject::~TimeObject()
//{
//	SafeDelete(image);
//}
//
//
//bool TimeObject::CheckWithin(const POINT& p)
//{
//	if ((pos.x < p.x) && (p.x < pos.x + width))
//	{
//		if ((pos.y < p.y) && (p.y < pos.y + height))
//			return true;
//	}
//	return false;
//}
//
////　ゲーム本編動作関数
//bool	TimeObject::CheckWithin(const POINT& p, const int& startX, const int& startY, const int size)
//{
//
//}
//
//void	TimeObject::Lerp(POINT& out, const POINT p1, const POINT p2, float t)
//{
//	float rate = t * t * (3.0f - 2.0f * t);   // 3次関数補間値に変換
//
//	out.x = (long)(p1.x * (1.0f - rate) + p2.x * rate);
//	out.y = (long)(p1.y * (1.0f - rate) + p2.y * rate);
//}
//
//void	TimeObject::Control(void)
//{
//
//}
//
//void	TimeObject::CalcPos(void)
//{
//
//}
//
//void	TimeObject::Check()
//{
//
//}
//
//void	TimeObject::ClearCheck()
//{
//
//}
//
//void	TimeObject::Shake()
//{
//
//}
//
//void	TimeObject::FlagUpdate()
//{
//
//}
//
//
////-----------------------------------------------------------------
////
////      TimeObjMgr class
////
////-----------------------------------------------------------------
//
//TimeObjMgr::TimeObjMgr()
//{
//
//}
//
//TimeObjMgr::~TimeObjMgr()
//{
//	for (TimeObject*& r : objList)
//		SafeDelete(r);
//	for (iex2DObj*& r : imageList)
//		SafeDelete(r);
//}
//
//void TimeObjMgr::Init()
//{
//	for (TimeObject*& r : objList)
//		SafeDelete(r);
//	for (iex2DObj*& r : imageList)
//		SafeDelete(r);
//
//	// 画像データとかステージデータとかロード
//}
//
//void TimeObjMgr::Update()
//{
//
//}
//
//void TimeObjMgr::Render()
//{
//
//}
//
//void TimeObjMgr::CalcPos()
//{
//	float cx, cy, vx, vy;
//	cx = 640;
//	cy = 360;
//	float	l;
//
//	for (int i = 0; i < OBJ_MAX; i++)
//	{
//		vx = objList[i]->GetPosCC().x - cx;
//		vy = objList[i]->GetPosCC().y - cy;
//
//		l = sqrt(vx * vx + vy *vy);
//
//		vx /= l;
//		vy /= l;
//
//		if (selectNum != i)
//			objList[i]->SetShift({(int)(vx * m_Camera.scale * 2), (int)(vy * m_Camera.scale * 2)});
//	}
//}
//
//void TimeObjMgr::Control()
//{
//	//移動中ならreturn
//	if (t < 1.0f)return;
//	//	エンターで選択
//	if (KEY(KEY_ENTER) == 3)
//	if (obj[selectNum].speed == obj[0].speed && selectNum != 0)
//		clear = true;
//
//	//基準オブジェクト選択判定
//	for (int i = 0; i < OBJ_MAX; i++)
//	{
//		//選択済みならスルー
//		if (i == selectNum)continue;
//		//マウスクリックチェック
//		if (Mouse::Click() && objList[i]->CheckWithin(Mouse::cursor))
//		{
//			selectNum = i;
//			//元の位置を保存
//			for (TimeObject*& r : objList)
//				r->SetStart(r->GetPosCC());
//			t = 0.0f;
//			break;
//		}
//	}
//}