#ifndef _PUMPKIN_H_
#define _PUMPKIN_H_
extern DWORD	ScreenMode;
class Pumpkin{
private:
	Pumpkin() :IsInit(false), IsMoveEndflg(true), IsOpen(false){}
	Pumpkin(Pumpkin&) {}
	Pumpkin& operator = (const Pumpkin&) {}

	EDX::EDX_2DObj* Pumpkinup, *Pumpkindown;

	bool IsMoveEndflg;
	bool IsInit;
	bool IsOpen;

	const float POWER_Y = 0.15f;
	int MAX_MOVE_Y = 200;
	int OFFSET_X = -20;
	int OFFSET_Y = 50;
	float P_SCALE = 1.5;

	float MoveY;
	float PowY;
	float PosY;
public:
	void Init();
	void Release();
	void Update();
	void Render();

	void Reset(bool IsOpen);

	void SetMaxY(int Y = 200){ MAX_MOVE_Y = Y; }

	bool IsMoveEnd();
	void SetOpen(bool IsOpen);

	__forceinline static Pumpkin* GetInst(){
		static Pumpkin c;
		return &c;
	}
};

#endif