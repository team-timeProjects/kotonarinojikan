#ifndef _GIMMICK_H_
#define _GIMMICK_H_

#include "TimeObject.h"
#include <list>

// �M�~�b�N���N���X
class Gimmick
{
	//--------- field ----------
protected:
	TimeObj* obj = nullptr;					// �M�~�b�N��K�p����I�u�W�F�N�g(�O���Ǘ�)

	//--------- method -----------
public:
	Gimmick();
	Gimmick(TimeObj* obj);
	virtual ~Gimmick();
	virtual void Update() = 0;
	virtual void Render() = 0;
	TimeObj* GetObj();
};

// ���s�ړ��M�~�b�N
class MoveGmk :public Gimmick
{
	//----------- field ------------
private:
	std::list<Vector2> node;				// �ړ��|�C���g
	std::list<Vector2>::iterator nowNode;	// �ړ����̃m�[�h
	bool ringLoop = false;					// �����O���[�v�F�܂�Ԃ����[�v
	bool searchOrder = true;				// �C�e���[�^�̕���(�܂�Ԃ����[�v�p)
	const float SPEED;						// �ړ��X�s�[�h

	//----------- method -----------
public:
	MoveGmk();
	MoveGmk(TimeObj* obj);
	void Update()override;
	void Render()override;
	void AppendNode(const Vector2& node);
	void SetLoop(bool IsRingLoop);
};

class FlagGmk :public Gimmick
{
	//--------- field --------------
public:
	enum TYPE
	{
		FREE, BLACK, GOLD
	};
	enum State
	{
		FADE_IN, STAY, FADE_OUT
	};
private:
	iex2DObj*	back;		// �O���Ǘ�
	ImageParam  backParam;
	Number*		number;		// �O���Ǘ�
	Vector2		pos;
	TYPE		type;
	int			num;
	bool		checked;

	// ���o�p
	float		alpha;
	Vector2		fromOffset;
	Vector2		targetOffset;
	float		effectTime;
	State		state;

	//----------- method ------------
public:
	FlagGmk(TimeObj* timeObj);
	~FlagGmk();
	void Init(iex2DObj* back, ImageParam backParam, Number* number);
	void Update()override;
	void Render()override;
	void SetNum(int num);
	int GetNum();
	void SetPos(const Vector2& pos);
	bool IsChecked();
	void SetChecked(bool checked);
	TYPE GetType();
	void SetType(TYPE type);
	void SetBack(iex2DObj* back, ImageParam backParam);
	void SetState(FlagGmk::State state);
	State GetState();
	bool IsEnd();
};

class FlagMgr
{
	//----------- field ---------------
private:
	enum State
	{
		MAIN, MOVE_NEXT, CHECK, EFFECT
	}state;
	iex2DObj* listBack;
	iex2DObj* blockBack;
	iex2DObj* goldBack;
	ImageParam listParam;
	ImageParam blackParam;
	ImageParam goldParam;
	std::list<FlagGmk*> blackFlag;
	std::list<FlagGmk*> goldFlag;
	std::list<FlagGmk*> releaseFlag;
	std::map<int, int> speedList;		// <�X�s�[�h,��>
	std::list<FlagGmk*>::iterator nowFlag;
	int haveGold;
	int effectTimer;
	int	missCount;

	//------------ method --------------
public:
	FlagMgr();
	~FlagMgr();
	void Init();
	void Update();
	void Render();
	void SetSpeedList(const std::map<int, int>& list);
	void AddSpeedCount(int speed);
	void AppendFlag(TimeObj* obj, bool next = true);
	void AppendPlainFlag(TimeObj* obj);
	void AppendGoldFlag(TimeObj* obj);
	void ReleaseFlag(TimeObj* obj);
	void StartCheck();// �`�F�b�N�t�F�[�Y�J�n
	bool CheckNext();// �ړ��J�n�A�Ȃ����false
	void CheckFlag();
	bool IsFinishEffect();// ���o�I��
	POINT GetNowObjPos();
	bool IsCheckEnd();
	bool IsClear();
	void SetHaveGoldFlag(int num);
	Vector2 GetSpeedBlockPos(int speed);
	int	GetMissCount();
	void SetMissCount(int mc);

private:
	inline int NextSpeed(int nowSpeed);
	inline int BeforeSpeed(int nowSpeed);
	//��
	inline void ReleaseImage();
	///��
	inline void ReleaseList();
};


#endif