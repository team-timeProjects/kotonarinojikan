#include	"Stage.h"
#include	"TextLoader.h"
#include	<string>
#include	<sstream>
#include	"Candle.h"
#include	"Metronom.h"
#include	"Clock.h"
#include	"DataOwner.h"
#include	"TimeObject.h"
#include	"Gimmick.h"

namespace
{
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

void Object::Setting(int x, int y, float scale, float speed, int behaviorId, int gimmickId, bool shuffle, int childNum)
{
	this->x = (float)x; this->y = (float)y;
	this->scale = scale; this->speed = speed;
	this->behaviorId = behaviorId;
	this->gimmickId = gimmickId;
	this->canShuffle = shuffle;
	this->childNum = childNum + 1;
	if (childNum > 1)
	{
		type = TYPE::MOVE;
		childX = new int[childNum];
		childY = new int[childNum];

		childX[0] = x;
		childY[0] = y;
	}
}

void Object::SetChild(int id, int x, int y)
{
	childX[id + 1] = x;
	childY[id + 1] = y;
}

void Object::Update()
{
	if (childNum <= 1) return;

	int nextPosId = nowPosId + 1;
	if (nextPosId > childNum)
	{
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
	if (length < OK_LEN*OK_LEN)
	{
		nowPosId++;
	}
}

//*********************************************************************

StageMNG::StageMNG()
{
	Campus::GetInst()->Init();
}

StageMNG::~StageMNG()
{
	for (TimeObj*& r : objList)
		SafeDelete(r);
	for (Gimmick*& r : gimmickList)
		SafeDelete(r);
}

bool StageMNG::LoadStage(const int stageNum)
{

	//stageNum����t�@�C�����̐ݒ�
	std::stringstream buf;
	buf << stageNum;
	//�t�@�C�������ʕ����̐ݒ�
	std::string fileName = "DATA/BG/stage" + buf.str() + ".sdt";
	TextLoader loader;
	if (!loader.Load((char*)fileName.c_str()))return false;
	if (loader.LoadInt()<5)return false;	//�o�[�W�������̋�ǂ�

	//���̔j��
	objMax = 0;
	for (TimeObj*& r : objList)
		SafeDelete(r);
	for (Gimmick*& r : gimmickList)
		SafeDelete(r);
	objList.clear();
	gimmickList.clear();
	shuffleList.clear();
	speedList.clear();
	//�X�e�[�W�^�C�v ��������� 1�����x(frame) ��
	stageType = (TYPE)loader.LoadInt();
	judgeNum = loader.LoadInt();
	judgeTimer = loader.LoadInt();
	goldenFlagNum = loader.LoadInt();

	//��
	objMax = loader.LoadInt();
	//�ݒ�p�ϐ�
	Vector2 pos;
	float scale, speed;
	int childNum;
	//�ݒ�
	for (int i = 0; i < objMax; i++)
	{//x y scale speed ���̑� �q�� ���[�v ���W...
		pos.x = loader.LoadInt();
		pos.y = loader.LoadInt();
		scale = loader.LoadFloat();
		speed = loader.LoadFloat();
		int behavior = loader.LoadInt();
		int gimmick = loader.LoadInt();
		bool shuffle = (bool)loader.LoadInt();
		childNum = loader.LoadInt();
		if (speedList.find(speed) == speedList.end())
			speedList[speed] = 1;
		else
			speedList[speed]++;

		TimeObj* obj = MakeObj(i, pos, scale, speed, behavior);
		if (obj != nullptr)
		{
			objList.push_back(obj);
			if (shuffle)
				shuffleList.push_back(obj);
		}
		if (childNum > 0)
		{
			MoveGmk* gmk = new MoveGmk(obj);
			gmk->SetLoop(loader.LoadInt() != 0);
			gmk->AppendNode(pos);
			for (int j = 0; j < childNum; j++)
			{
				pos.x = loader.LoadInt();
				pos.y = loader.LoadInt();
				gmk->AppendNode(pos);
			}
			gimmickList.push_back(gmk);
		}
	}
	SpeedShuffle();
	float startSpeed = objList.front()->GetOrginSpeed();
	for (TimeObj*& r : objList)
	{
		r->SetRelativeSpeed(startSpeed);
	}
	nowID = 0;
	DefaultGoldFlagSum = HaveGoldFlag = goldenFlagNum;
	return true;
}

void StageMNG::Update()
{
	HaveGoldFlag = DefaultGoldFlagSum;
	for (TimeObj*& r : objList)
	{
		r->Update();
		if (r->GetID() != 0 && r->GetGold_Effect())
		{
			HaveGoldFlag--;
		}
	}
	if (IsChecking)return;
	for (Gimmick*& r : gimmickList)
		r->Update();
}

void StageMNG::Render()
{
	for (TimeObj*& r : objList)
		r->Render();
	for (Gimmick*& r : gimmickList)
		r->Render();
	Campus::GetInst()->Draw();
}

POINT StageMNG::GetPos(int objID)const
{
	POINT ret;
	TimeObj* r = SearchObj(objID);
	if (r != nullptr)
	{
		ret.x = (LONG)r->GetPos().x;
		ret.y = (LONG)r->GetPos().y;
	}
	else
	{
		ret.x = 0;
		ret.y = 0;
	}
	return ret;
}

int StageMNG::GetNowObj()const
{
	return nowID;
}

int StageMNG::IsCollision(const POINT& p)const
{
	for (TimeObj* r : objList)
	{
		if (r->IsCollision(p))
			return r->GetID();
	}
	return -1;
}

void StageMNG::Activate(int objID)
{
	nowID = objID;
	TimeObj* r = SearchObj(objID);
	if (r == nullptr)return;

	float current = r->GetOrginSpeed();
	for (TimeObj*& i : objList)
	{
		i->SetRelativeSpeed(current);
	}
}

void StageMNG::SpeedShuffle()
{
	std::vector<float> work;
	for (TimeObj*& r : shuffleList)
	{
		work.push_back(r->GetOrginSpeed());
		r->IsShuffled = false;
	}
	for (float r : work)
	{
		int n = rand() % shuffleList.size();
		while (shuffleList.at(n)->IsShuffled)
			n = rand() % shuffleList.size();
		shuffleList.at(n)->SetOrginSpeed(r);
		shuffleList.at(n)->IsShuffled = true;
	}
}

TimeObj* StageMNG::GetObj(int objID)
{
	return SearchObj(objID);
}

std::map<int, int> StageMNG::GetSpeedList()
{
	return speedList;
}

int StageMNG::GetjudgeTimer()
{
	return judgeTimer;
}

int StageMNG::GetJudgeNum()
{
	return judgeNum;
}

inline TimeObj* StageMNG::SearchObj(int ID)const
{
	for (TimeObj* r : objList)
	{
		if (r->GetID() == ID)
			return r;
	}
	return nullptr;
}

inline TimeObj* StageMNG::MakeObj(int ID, const Vector2& pos, float scale, float speed, int behavior)
{
	TimeObj* ret = nullptr;
	auto Append = [&ret](int idx, ImageFactory::ImageID id)
	{
		ret->AppendImage(idx,
			DataOwner::GetInst()->imageFactory->GetImage(id),
			DataOwner::GetInst()->imageFactory->GetParam(id));
	};

	switch (stageType)
	{
	case StageMNG::CLOCK:
		ret = new Clock;
		Append(Clock::BACK, ImageFactory::CLOCK_BACK);
		Append(Clock::HOUR, ImageFactory::CLOCK_HOUR);
		Append(Clock::MINUTE, ImageFactory::CLOCK_MINUTE);
		ret->Init(ID, pos, 300, 300, scale, speed, behavior);
		break;
	case StageMNG::CANDOL:
		ret = new Candle;
		Append(Clock::BACK, ImageFactory::CLOCK_BACK);
		Append(Candle::CANDLESTICK, ImageFactory::CANDLESTICK);
		Append(Candle::BIG_CANDLE, ImageFactory::CANDLE_BIG);
		Append(Candle::FIRE, ImageFactory::CANDLE_FIRE);
		Append(Candle::ANIMATION_FIRE, ImageFactory::CANDLE_FIRE_ANIMATION);
		Append(Candle::BIG_MELT, ImageFactory::CANDLE_BIG_MELT);
		Append(Candle::SMALL_CANDLE, ImageFactory::CANDLE_SMALL);
		Append(Candle::SMALL_MELT, ImageFactory::CANDLE_SMALL_MELT);
		ret->Init(ID, pos, 300, 300, scale, speed, behavior);
		break;
	case StageMNG::METRO:
		ret = new Metronom;
		Append(Metronom::BACK, ImageFactory::METRONOM_BACK);
		Append(Metronom::NEEDLE, ImageFactory::METRONOM_NEEDLE);
		Append(Metronom::SPINDLE1, ImageFactory::METRONOM_SPINDLE1);
		Append(Metronom::SPINDLE2, ImageFactory::METRONOM_SPINDLE2);
		Append(Metronom::METRONOM, ImageFactory::METRONOM);
		Append(Metronom::FACE, ImageFactory::METRONOM_FACE);
		ret->Init(ID, pos, 300, 300, scale, speed, behavior);
		break;
	default:
		break;
	}
	return ret;
}
