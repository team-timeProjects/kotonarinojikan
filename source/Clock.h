
#ifndef	__CLOCK_H__
#define	__CLOCK_H__
#include "../IEX/iextreme.h"
#include "TimeObject.h"
#include "Campus.h"

class Clock :public TimeObj
{
	//----------- field --------------
public:
	enum Behavior
	{
		SMOOTH = 0, STEPING, GO3_BACK2_SMOOTH, GO3_BACK2_STEPING, STANDSTILL
	};
	enum Image
	{
		BACK, HOUR, MINUTE
	};
private:
	const static int IMAGE_WIDTH = 300;
	const static int IMAGE_HEIGHT = 300;
	static const int FPS = 60;			// frame per second
	static const int MPH = 24;			// minute per hour
	static const int HOUR_CYCLE = 12;	// ����̎��Ԑ�
	static const float ANGLE_1MINUTE;	// �ꕪ�̊p�x
	static const float ANGLE_1HOUR;		// �ꎞ�Ԃ̊p�x
	int timeCount = 0;					// �A���O���p�b�J�E���g
	int frameCount = 0;					// �A���O���p�t���[���J�E���g
	int frameTotal = 0;					// �t���[���J�E���g
	int timeTotal = 0;
	int timeOffset = rand() % (MPH*HOUR_CYCLE);
	float hourAngle = 0;				// �Z�j�̊p�x
	float minuteAngle = 0;				// ���j�̊p�x
	bool	vecflag = true;
	int		stepTimer = 0;
	int		stepCount = 0;


	//---------- method -------------
public:
	Clock();
	void Init(int id, const Vector2& centerPos, int colW, int colH, float scale, float speed, Behavior behavior);
	void AppendImage(int idx, iex2DObj* image, const ImageParam& param)override;
	void Update()override;
	void Render()override;
private:
	void Update_Time(float speed = 1.0f);
	void Update_Check();
	void Update_Smooth();
	void Update_Steping();
	void Update_Time_Go3Back2_Steping(float speed = 1.0f);
	void Update_Time_Go3Back2_Smooth(float speed = 1.0f);
	void Update_Time_Standstill(float speed = 1.0f);
};


//****************************************************************************************
#endif // !__CLOCK_H__
