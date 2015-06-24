#ifndef _TIMEOBJECT_H_
#define _TIMEOBJECT_H_

//#include "Game.h"
#include "../IEX/iextreme.h"
#include <map>


// �摜�̕`��p�����[�^
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
	iex2DObj*	image;		//���ȊǗ�
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

// TimeObj�̉摜�Ǘ�
class ImageFactory
{
	//------- field ---------
public:
	enum ImageID
	{
		FRAG_BLACK_CLOSE, FRAG_BLACK_OPEN, FRAG_GOLD,
		BUTTON_MENU,
		SPEED_LIST, SPEED_BLOCK, GOLD_STOCK,

		//�@���v�֘A
		CLOCK_BACK, CLOCK_HOUR, CLOCK_MINUTE, 

		//	�낤�����֘A
		CANDLESTICK,
		CANDLE_BACK, CANDLE_SMALL, CANDLE_BIG,
		CANDLE_FIRE, CANDLE_FIRE_ANIMATION, CANDLE_SMALL_MELT,
		CANDLE_BIG_MELT,

		//	���g���m�[���֘A
		METRONOM_BACK,
		METRONOM_NEEDLE, METRONOM_SPINDLE1, METRONOM_SPINDLE2, METRONOM, METRONOM_FACE
	};
private:
	std::map<int, iex2DObj*> imageList;				// �摜�f�[�^
	std::map<int, ImageParam> paramList;			// �摜�\�[�X�p�����[�^

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
	std::map<int, iex2DObj*> imageList;				// �摜�f�[�^(�O���Ǘ�)
	std::map<int, ImageParam> dst;					// �摜�`��p�����[�^
	std::map<int, ImageParam> src;					// �摜�\�[�X�p�����[�^
	Vector2 pos;									// ���S�̃��[���h���W
	int colw = 0, colh = 0;							// �����蔻��p������
	float scale = 0.0f;								// �`��T�C�Y
	float orginSpeed = 0.0f;						// ���Ԍo�ߐ�΃X�s�[�h
	float speed = 0.0f;								// ���Ԍo�ߑ��΃X�s�[�h
	int behavior = 0;								// �����A���S���Y��
	bool haveFlag = false;							// �t���b�O�������
	static const int SUCCESS_EFFECT_TIME = 30;		// �������o���Ԓ萔
	int SuccessCnt = 0;								// �������o�p�ϐ�
	const float CHAIN_MAX_SCALE = 3.5f;				// �A�����o�p�ő�g�嗦
	static const int CHAIN_EFFECT_MAX = 5;			// �A�����o�p�ő�g�嗦���B�A����
	static int SuccessChain;						// �A���J�E���^
	int Mine_SChain;								// �����̘A���J�E���^
	bool Gold_Effect = false;						// ���t���b�O���ʔ�����

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
	// ��ƂȂ�X�s�[�h�����Ƃɑ��΃X�s�[�h������
	void SetRelativeSpeed(float orginSpeed);
	// ���[���h���W�Ƃ̓����蔻��(��`)
	bool IsCollision(const POINT& p);
	//					�`��̒��S���W�A�����蔻��̕������A�g�嗦�A�@�@�@���̃X�s�[�h�A�@�@����ID
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