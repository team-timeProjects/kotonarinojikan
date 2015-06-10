#ifndef _GIMMICK_H_
#define _GIMMICK_H_

#include "TimeObject.h"
#include <list>

// ギミック基底クラス
class Gimmick
{
	//--------- field ----------
protected:
	TimeObj* obj = nullptr;					// ギミックを適用するオブジェクト(外部管理)

	//--------- method -----------
public:
	Gimmick();
	Gimmick(TimeObj* obj);
	virtual ~Gimmick();
	virtual void Update() = 0;
	virtual void Render() = 0;
	TimeObj* GetObj();
};

// 平行移動ギミック
class MoveGmk :public Gimmick
{
	//----------- field ------------
private:
	std::list<Vector2> node;				// 移動ポイント
	std::list<Vector2>::iterator nowNode;	// 移動中のノード
	bool ringLoop = false;					// リングループ：折り返しループ
	bool searchOrder = true;				// イテレータの方向(折り返しループ用)
	const float SPEED;						// 移動スピード

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
		BLACK, GOLD
	};
	enum State
	{
		FADE_IN, STAY, FADE_OUT
	};
private:
	iex2DObj*	back;		// 外部管理
	ImageParam  backParam;
	Number*		number;		// 外部管理
	Vector2		pos;
	TYPE		type;
	int			num;
	bool		checked;

	// 演出用
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
		MAIN,CHECK
	}state;
	iex2DObj* listBack;
	iex2DObj* blockBack;
	std::list<FlagGmk*> blackFlag;
	std::list<FlagGmk*> goldFlag;
	std::list<FlagGmk*> releaseFlag;
	std::map<int, int> speedList;		// <スピード,個数>
	std::list<FlagGmk*>::iterator nowFlag;
	

	//------------ method --------------
public:
	FlagMgr();
	~FlagMgr();
	void Init();
	void Update();
	void Render();
	void SetSpeedList(const std::map<int, int>& list);
	void AppendFlag(TimeObj* obj, bool next = true);
	bool StartCheck();// チェックフェーズ開始
	bool CheckNext();// 移動開始、なければfalse
	void CheckFlag();
	bool IsFinishEffect();// 演出終了
	POINT GetNowObjPos();
	bool IsClear();
private:
	inline int NextSpeed(int nowSpeed);
	inline int BeforeSpeed(int nowSpeed);
	inline void ReleaseImage();
	inline void ReleaseList();
};


#endif