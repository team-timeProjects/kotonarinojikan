#ifndef _TIMEOBJECT_H_
#define _TIMEOBJECT_H_

//#include "Game.h"
#include "../IEX/iextreme.h"
#include <map>
#include <list>

// 時計・蝋燭・メトロノームの基底クラス
//class TimeObject
//{
//protected:
//	iex2DObj*	image = nullptr;		//　画像
//	const int	SOURCE_W = 0;			//　画像ソースのクリップ幅
//	const int	SOURCE_H = 0;			//	画像ソースのクリップ高さ
//	int			width = 0;				//　画像描画幅
//	int			height = 0;				//　画像描画高さ
//	POINT		pos = {0, 0};			//	位置
//	POINT		start = {0, 0};			//	元の位置
//	POINT		end = {0, 0};			//	移動後の位置
//	POINT		shift = {0, 0};			//	カメラ用ずれ幅
//	float		speed = .0f;			//	速度
//
//	int			flag = 0;				//　フラグ※Ｅ
//	int			state = 0;				//　判定状態
//	int			fallpow = 0;			//　失敗演出用
//	bool		SuccessFlg = false;		//　成功演出用
//	int			SuccessCnt = 0;			//　成功演出用
//
//	//フラグ種類※Ｅ
//	enum
//	{
//		FLAG_NOT,
//		FLAG_RED,
//		FLAG_GREEN,
//		FLAG_BLUE,
//	};
//	//時計判定状態※Ｅ
//	enum
//	{
//		STATE_WAIT,
//		STATE_SUCCESS,
//		STATE_BERAK,
//	};
//
//	POINT		length;		//	距離
//	float		org_speed;	//	元のスピード
//	float		cul_speed;	//	現在選択中のそもそものスピード
//	int			selectNum;	//	選択中の番号
//	bool		clear;		//	クリア
//	float		t;			//	割合
//	int			step;		//	ステップ
//	bool		CheckFlg;	//　チェック開始※Ｅ
//
//	//　判定失敗時用
//	static const int SHAKE_TIME_MAX = 30;
//	const float SHAKE_POWER = 10.0f;
//	bool		ShakeFlg;
//	int			ShakeCnt;
//	Vector2		ShakeVar;
//
//	//　判定成功時用
//	static const int SUCCESS_TIME_MAX = 30;
//	const float SUCCESS_SCALE_MAX = 2.0f;
//
//	//所持フラッグ
//	int DefaultHaveFlag[4];
//	int HaveFlag[4];
//
//public:
//	TimeObject();
//	TimeObject(int sw, int sh);
//	virtual ~TimeObject();
//
//	virtual void Update() = 0;
//	virtual void Render() = 0;
//	virtual bool CheckWithin(const POINT& p) { return false; };
//	virtual POINT& GetPosCC() = 0;
//	void SetPosCC(const POINT& pos);
//	void SetShift(const POINT& shift);
//	void SetStart(const POINT& pos);
//
//	//　ゲーム本編動作関数
//	bool	CheckWithin(const POINT& p, const int& startX, const int& startY, const int size);	//	範囲内かどうか調べる
//	void	Lerp(POINT& out, const POINT p1, const POINT p2, float t);	//	POINT補間
//	void	Control(void);	//	操作
//	void	CalcPos(void);	//	位置関係計算
//
//	void	Check();			//　判定※Ｅ
//	void	ClearCheck();		//　判定※Ｅ
//	void	Shake();
//	void	FlagUpdate();
//};

// 画像の描画パラメータ
struct ImageParam
{
	int offsetx, offsety;// 描画座標(左上)からのオフセット
	int dw, dh;
	int sx, sy;
	int sw, sh;
	ImageParam()
	{
		offsetx = offsety = dw = dh = 0;
		sx = sy = sw = sh = 0;
	}
	ImageParam(int offsetx, int offsety, int dw, int dh, int sx, int sy, int sw, int sh) :
		offsetx(offsetx), offsety(offsety), dw(dw), dh(dh), sx(sx), sy(sy), sw(sw), sh(sh)
	{
	}
	ImageParam(int sx, int sy, int sw, int sh) :sx(sx), sy(sy), sw(sw), sh(sh)
	{
		offsetx = offsety = 0;
		dw = sw;
		dh = sh;
	}
};

// TimeObjの画像管理
class ImageFactory
{
	//------- field ---------
public:
	enum ImageID
	{
		CLOCK_BACK, CLOCK_HOUR, CLOCK_MINUTE
	};
private:
	std::map<int, iex2DObj*> imageList;				// 画像データ
	std::map<int, ImageParam> paramList;			// 描画パラメータ

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
protected:
	std::map<int, iex2DObj*> imageList;				// 画像データ(外部管理)
	std::map<int, ImageParam> paramList;			// 画像描画パラメータ
	Vector2 pos;									// 中心のワールド座標
	int w = 0, h = 0;								// 当たり判定用幅高さ
	float scale = 0.0f;								// 描画サイズ
	float orginSpeed = 0.0f;						// 時間経過絶対スピード
	float speed = 0.0f;								// 時間経過相対スピード
	int behavior = 0;								// 挙動アルゴリズム

	//------------- method ----------------
public:
	TimeObj();
	virtual ~TimeObj();
	void AppendImage(int idx, iex2DObj* image, const ImageParam& param);
	const Vector2& GetPos();
	virtual void SetPos(const Vector2& pos);
	float GetOrginSpeed();
	// 基準となるスピードをもとに相対スピードを決定
	void SetRelativeSpeed(float orginSpeed);
	// ワールド座標との当たり判定(矩形)
	bool IsCollision(const POINT& p);
	//					描画の中心座標、当たり判定の幅高さ、拡大率、　　　元のスピード、　　挙動ID
	void Init(const Vector2& centerPos, int colW, int colH, float scale, float orginSpeed, int behavior);
	virtual void Update() = 0;
	virtual void Render() = 0;
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