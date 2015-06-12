#ifndef _CAMPUS_H_
#define _CAMPUS_H_

#include <list>

extern DWORD	ScreenMode;
POINT GetPoint(int x, int y);
POINT MulPoint(POINT p, float mul);
POINT AddPoint(POINT p1, POINT p2);
POINT SubPoint(POINT p1, POINT p2);

struct RenderObject{
	iex2DObj* obj;
	EDX::EDX_2DObj* e_obj;
	s32 x;
	s32 y;
	s32 w;
	s32 h;
	s32 sx;
	s32 sy;
	s32 sw;
	s32 sh;
	POINT p = GetPoint(0, 0);
	float angle = 0.0f;
	float scale = 1.0f;
	u32 dwFlags = RS_COPY;
	IEXCOLOR color = 0xFFFFFFFF;
	float z = 0.0f;
};

class Campus
{
private:
	//âÊñ ÇÃíÜêS
	POINT cpos;
	POINT nextcpos;
	POINT pastcpos;
	float MoveSpeed;
	float time;
	float Zoom;

	Campus() :cpos(GetPoint(0, 0)), nextcpos(GetPoint(0, 0)), pastcpos(GetPoint(0, 0)), MoveSpeed(1.0f), time(0.0f) {}
	Campus(Campus&) {}
	Campus& operator = (const Campus&) {}

	std::list<RenderObject> rolist;
public:
	void Init();
	void Update();
	void Draw();

	void Add(iex2DObj* obj, s32 x, s32 y, s32 w, s32 h, s32 sx, s32 sy, s32 sw, s32 sh, POINT p = GetPoint(0, 0),
		float angle = 0, float scale = 1, u32 dwFlags = RS_COPY, IEXCOLOR color = 0xFFFFFFFF, float z = 0.0f);
	void Add(EDX::EDX_2DObj* obj, s32 x, s32 y);
	void Add(EDX::EDX_2DObj* obj, s32 x, s32 y, POINT p, float angle = 0, float scale = 1, IEXCOLOR color = 0xFFFFFFFF);
	void Add(EDX::EDX_2DObj* obj, s32 x, s32 y, s32 w, s32 h, s32 sx, s32 sy, s32 sw, s32 sh);
	void Add(EDX::EDX_2DObj* obj, s32 x, s32 y, s32 w, s32 h, s32 sx, s32 sy, s32 sw, s32 sh,
		POINT p,float angle = 0, float scale = 1, IEXCOLOR color = 0xFFFFFFFF);

	void SetPos(POINT pos);
	void SetNextPos(POINT nextpos);
	void SetMoveSpeed(float Speed);
	bool IsMoveEnd();

	void SetZoom(float Zoom);
	void TimeReset();

	POINT GetPos(){
	POINT out;
	//RECT WindowSize;
	//iexSystem::GetScreenRect(ScreenMode, WindowSize);
	out.x = cpos.x;// +WindowSize.right;
	out.y = cpos.y;// +WindowSize.bottom;
	return out;
	}

	static Campus* GetInst(){
		static Campus c;
		return &c;
	}
};

#endif