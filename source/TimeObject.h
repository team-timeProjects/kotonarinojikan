#ifndef _TIMEOBJECT_H_
#define _TIMEOBJECT_H_

//#include "Game.h"
#include "../IEX/iextreme.h"
#include <map>
<<<<<<< HEAD
#include <list>
#include <vector>
=======
>>>>>>> remotes/origin/horita


// 画像の描画パラメータ
struct ImageParam
{
	int x, y;
	int w, h;
	ImageParam()
	{
		x = y = w = h = 0;
	}
	ImageParam(int x, int y, int w, int h):x(x), y(y), w(w), h(h) {}
};

class Number
{
	//---------- field -----------
private:
	enum
	{
<<<<<<< HEAD
		MINUS_10 = 0, MINUS_9, MINUS_8, MINUS_7, MINUS_6, MINUS_5, MINUS_4, MINUS_3, MINUS_2, MINUS_1, NUM_0,
		PLUS_1, PLUS_2, PLUS_3, PLUS_4, PLUS_5, PLUS_6, PLUS_7, PLUS_8, PLUS_9, PLUS_10
	};
	iex2DObj*				 image;		//自己管理
	std::map<int, ImageParam> param;

	//---------- method ------------
public:
	~Number();
	void Init();
	void Render(int num, int x, int y, float scale, bool campus = true);
=======
		MINUS=-1
	};
	iex2DObj*	image;		//自己管理
	const int	WIDTH;
	const int	HEIGHT;

	//---------- method ------------
public:
	Number();
	~Number();
	void Init();
	void RenderCC(int num, int x, int y, float scale,float alpha=1.0f, bool campus = true);
private:
	void Render(int n, int x, int y, float scale, float alpha,bool campus);
	inline int idxCalc(int base, int exponent);
>>>>>>> remotes/origin/horita
};

// TimeObjの画像管理
class ImageFactory
{
	//------- field ---------
public:
	enum ImageID
	{
		CLOCK_BACK, CLOCK_HOUR, CLOCK_MINUTE, FRAG_BLACK,FRAG_GOLD
	};
private:
	std::map<int, iex2DObj*> imageList;				// 画像データ
	std::map<int, ImageParam> paramList;			// 画像ソースパラメータ

	//-------- method ----------
public:
	~ImageFactory();
	void Init();
	iex2DObj* GetImage(ImageID id);
	ImageParam GetParam(ImageID id);
};

class TimeObj
{
	//---------- field -------------
public:
	enum State
	{
<<<<<<< HEAD
		MOVE, STOP, CHECK
	}state = MOVE;
	bool IsShuffled = false;

=======
		MOVE, STOP, CHECK,SUCCESS
	}state = MOVE;
	bool IsShuffled = false;
>>>>>>> remotes/origin/horita
protected:
	int id = 0;
	std::map<int, iex2DObj*> imageList;				// 画像データ(外部管理)
	std::map<int, ImageParam> dst;					// 画像描画パラメータ
	std::map<int, ImageParam> src;					// 画像ソースパラメータ
	Vector2 pos;									// 中心のワールド座標
	int colw = 0, colh = 0;							// 当たり判定用幅高さ
	float scale = 0.0f;								// 描画サイズ
	float orginSpeed = 0.0f;						// 時間経過絶対スピード
	float speed = 0.0f;								// 時間経過相対スピード
	int behavior = 0;								// 挙動アルゴリズム
<<<<<<< HEAD
	
=======
	bool haveFlag = false;							// フラッグ所持状態
	static const int SUCCESS_EFFECT_TIME = 30;		// 卍成功演出時間定数
	int SuccessCnt = 0;								// 卍成功演出用変数
	const float CHAIN_MAX_SCALE = 3.0f;				// 卍連鎖演出用最大拡大率
	static const int CHAIN_EFFECT_MAX = 5;			// 卍連鎖演出用最大拡大率到達連鎖数
	static int SuccessChain;						// 卍連鎖カウンタ
	int Mine_SChain;								// 卍自分の連鎖カウンタ
	bool Gold_Effect = false;						// 卍金フラッグ効果発動中
>>>>>>> remotes/origin/horita

	//------------- method ----------------
public:
	TimeObj();
	virtual ~TimeObj();
	virtual void AppendImage(int idx, iex2DObj* image, const ImageParam& source);
	int GetID()const;
	const Vector2& GetPos()const;
	virtual void SetPos(const Vector2& pos);
	void SetOrginSpeed(float speed);
	float GetOrginSpeed();
	// 基準となるスピードをもとに相対スピードを決定
	void SetRelativeSpeed(float orginSpeed);
	// ワールド座標との当たり判定(矩形)
	bool IsCollision(const POINT& p);
	//					描画の中心座標、当たり判定の幅高さ、拡大率、　　　元のスピード、　　挙動ID
	void Init(int id, const Vector2& centerPos, int colW, int colH, float scale, float orginSpeed, int behavior);
	virtual void Update() = 0;
	virtual void Render() = 0;
	void SetState(TimeObj::State s);
<<<<<<< HEAD
};

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
=======
	__forceinline static void ResetChain(){ SuccessChain = 0; }
	__forceinline static void AddChain(){ SuccessChain++; }
	__forceinline static int GetChain(){ return SuccessChain; }
	__forceinline bool GetGold_Effect(){ return Gold_Effect; }
	__forceinline void SetGold_Effect(bool IsGoldEffect){ Gold_Effect = IsGoldEffect; }
	__forceinline int GetSuccessCnt(){ return SuccessCnt; }
	__forceinline void SetMine_SChain(int Chain){ Mine_SChain = Chain; }
>>>>>>> remotes/origin/horita
};

class FlagGmk :public Gimmick
{
	//--------- field --------------
public:
	enum TYPE
	{
		BLACK, GOLD
	};
private:
	iex2DObj*	back;		// 外部管理
	ImageParam  backParam;
	Number*		number;		// 外部管理
	Vector2		pos;
	TYPE		type;
	int			num;
	bool		checked;

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
};

class FlagMgr
{
	//----------- field ---------------
private:
	iex2DObj* listBack;
	iex2DObj* blockBack;
	iex2DObj* goldBat;
	std::map<TimeObj*, FlagGmk*> flagList;
	std::map<int, int> speedList;		// <スピード,個数>
	TimeObj* nowObj;
	int		MissCount;

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
	int		GetMissCount(){ return MissCount; }
	void	SetMissCount(int mc){ MissCount = mc; }
private:
	inline int NextSpeed(int nowSpeed);
	inline int BeforeSpeed(int nowSpeed);
};

//class TimeObjMgr
//{
//protected:
//	const static int OBJ_MAX = 64;
//	const static int IMAGE_MAX = 8;
//	TimeObject* objList[OBJ_MAX] = {nullptr};
//	iex2DObj* imageList[IMAGE_MAX] = {nullptr};
//	int selectNum = 0;
//	CAMERA camera;
//
//public:
//	TimeObjMgr();
//	~TimeObjMgr();
//	void Init();
//	void Update();
//	void Render();
//protected:
//	void CalcPos();
//	void Control();
//
//};

#endif