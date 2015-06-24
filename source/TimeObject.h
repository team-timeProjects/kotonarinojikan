#ifndef _TIMEOBJECT_H_
#define _TIMEOBJECT_H_

//#include "Game.h"
#include "../IEX/iextreme.h"
#include <map>


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
};

// TimeObjの画像管理
class ImageFactory
{
	//------- field ---------
public:
	enum ImageID
	{
		FRAG_BLACK_CLOSE, FRAG_BLACK_OPEN, FRAG_GOLD,
		BUTTON_MENU,
		SPEED_LIST, SPEED_BLOCK, GOLD_STOCK,

		//　時計関連
		CLOCK_BACK, CLOCK_HOUR, CLOCK_MINUTE, 

		//	ろうそく関連
		CANDLESTICK,
		CANDLE_BACK, CANDLE_SMALL, CANDLE_BIG,
		CANDLE_FIRE, CANDLE_FIRE_ANIMATION, CANDLE_SMALL_MELT,
		CANDLE_BIG_MELT,

		//	メトロノーム関連
		METRONOM_BACK,
		METRONOM_NEEDLE, METRONOM_SPINDLE1, METRONOM_SPINDLE2, METRONOM, METRONOM_FACE
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
		MOVE, STOP, CHECK,SUCCESS
	}state = MOVE;
	bool IsShuffled = false;
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
	bool haveFlag = false;							// フラッグ所持状態
	static const int SUCCESS_EFFECT_TIME = 30;		// 卍成功演出時間定数
	int SuccessCnt = 0;								// 卍成功演出用変数
	const float CHAIN_MAX_SCALE = 3.5f;				// 卍連鎖演出用最大拡大率
	static const int CHAIN_EFFECT_MAX = 5;			// 卍連鎖演出用最大拡大率到達連鎖数
	static int SuccessChain;						// 卍連鎖カウンタ
	int Mine_SChain;								// 卍自分の連鎖カウンタ
	bool Gold_Effect = false;						// 卍金フラッグ効果発動中

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
	virtual void Update(void) = 0;
	virtual void Render(void) = 0;
	void SetState(TimeObj::State s);
	__forceinline static void ResetChain(){ SuccessChain = 0; }
	__forceinline static void AddChain(){ SuccessChain++; }
	__forceinline static int GetChain(){ return SuccessChain; }
	__forceinline bool GetGold_Effect(){ return Gold_Effect; }
	__forceinline void SetGold_Effect(bool IsGoldEffect){ Gold_Effect = IsGoldEffect; }
	__forceinline int GetSuccessCnt(){ return SuccessCnt; }
	__forceinline void SetMine_SChain(int Chain){ Mine_SChain = Chain; }
};

#endif