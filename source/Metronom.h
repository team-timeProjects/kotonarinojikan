
#ifndef		__METRONOM_H__
#define	__METRONOM_H__
#include "../IEX/iextreme.h"
#include "TimeObject.h"
#include "Campus.h"
//****************************************************************************************
//
//		Metronom�N���X
//
//****************************************************************************************
class Metronom :public TimeObj
{
	//----------- field --------------
public:
	enum Behavior
	{
		ORIGIN = 0, DOUBLE,
	};
	enum Image
	{
		BACK, METRONOM, NEEDLE, SPINDLE1, SPINDLE2, FACE
	};

private:
	const static int IMAGE_WIDTH = 300;
	const static int IMAGE_HEIGHT = 300;
	static const int FPS = 60;			// frame per second
	static const int MPH = 24;			// minute per hour

	float			timeCount = 0.0f;			// �b�J�E���g
	float			frameCount = 0.0f;		// �t���[���J�E���g
	float			melt = 0.0f;					//	�n���
	float			alpha = 0.0f;					//	�����x
	int				animationCounter = 0;
	float			org_speed = D3DX_PI / 180 * 5;
	float			param = 0.0f;
	float			angle = 0.0f;
	int				shakeParam = 0;
	bool			shakeflag = false;
	//---------- method -------------
public:
	Metronom(void);
	void Init(int id, const Vector2& centerPos, int colW, int colH, float scale, float speed, Behavior behavior);
	void AppendImage(int idx, iex2DObj* image, const ImageParam& param)override;
	void Update(void)override;
	void Render(void)override;
	void	RenderSmallCandle(void);
	void	RenderBigCandle(void);
private:
	void Update_Time(float speed = 1.0f);
	void Update_Check(void);
	void	Move(float speed = 1.0f);
};

//****************************************************************************************
#endif // !__METRONOM_H__
