#ifndef _PUMPKIN_H_
#define _PUMPKIN_H_
extern DWORD	ScreenMode;
class Pumpkin{
private:
	Pumpkin() :IsInit(false), IsMoveEndflg(true), IsOpen(false){}
	Pumpkin(Pumpkin&) {}
	Pumpkin& operator = (const Pumpkin&) {}

	EDX::EDX_2DObj* Pumpkinup, *Pumpkindown, *flash;


	bool IsMoveEndflg;

	bool IsInit;
	bool IsOpen;

	const float POWER_Y = 0.15f;
	int MAX_MOVE_Y = 300;
	int OFFSET_X = -256;
	int OFFSET_Y = -250;
	float P_SCALE = 1;

	bool Flashflg;
	int FlashAlpha;
	DWORD FlashCol;


	float MoveY;
	float PowY;
	float PosY;

	static const int SHAKE_TIME_MAX = 50;
	const float SHAKE_POWER = 20.0f;
	EDX::Vector ShakeVar;
	int ShakeTime;
	void Shake();
public:
	void Init();
	void Release();
	void Update();
	void Render();

	void Reset(bool IsOpen);

	void SetMaxY(int Y = 200){ MAX_MOVE_Y = Y; }

	bool IsMoveEnd();
	void SetOpen(bool IsOpen);
	void ShakeStart(){ if (ShakeTime <= 0) ShakeTime = SHAKE_TIME_MAX; }

	__forceinline static Pumpkin* GetInst(){
		static Pumpkin c;
		return &c;
	}
};

#endif