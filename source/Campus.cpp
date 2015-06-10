#include "../IEX/iextreme.h"
#include "Campus.h"
POINT GetPoint(int x, int y){
	POINT p;
	p.x = x;
	p.y = y;
	return p;
}
POINT MulPoint(POINT p, float mul){
	p.x *= mul;
	p.y *= mul;
	return p;
}
POINT AddPoint(POINT p1, POINT p2){
	POINT out;
	out.x = p1.x + p2.x;
	out.y = p1.y + p2.y;
	return out;
}

POINT SubPoint(POINT p1, POINT p2)
{
	POINT out;
	out.x = p1.x - p2.x;
	out.y = p1.y - p2.y;
	return out;
}

void Campus::Init(){
	RECT WindowSize;
	iexSystem::GetScreenRect(ScreenMode, WindowSize);
	cpos.x -= WindowSize.right*0.5f;
	cpos.y -= WindowSize.bottom*0.5f;
	pastcpos = nextcpos = cpos;
	time = 1.0f;
	Zoom = 1.0f;
	MoveSpeed = 0.02f;

	rolist.clear();
}

void Campus::Update(){

	time += MoveSpeed;
	if (time <= 1.0f){
		float rate = time * time * (3.0f - 2.0f * time);

		cpos.x = (long)(pastcpos.x * (1.0f - rate) + nextcpos.x * rate);
		cpos.y = (long)(pastcpos.y * (1.0f - rate) + nextcpos.y * rate);

	}
}

void Campus::Draw(){
	for (auto it : rolist){
		int x = it.x - cpos.x;
		int y = it.y - cpos.y;
		float w = it.w;
		float h = it.h;
		if (Zoom != 1.0f){
			RECT WindowSize;
			iexSystem::GetScreenRect(ScreenMode, WindowSize);
			x = (x - WindowSize.right*0.5f)*Zoom;
			y = (y - WindowSize.bottom*0.5f)*Zoom;
			x += WindowSize.right*0.5f;
			y += WindowSize.bottom*0.5f;
			w *= Zoom;
			h *= Zoom;
		}

		if (it.obj)it.obj->Render(x, y, w, h,
			it.sx, it.sy, it.sw, it.sh,
			it.p, it.angle, it.scale, (unsigned long)it.dwFlags, (DWORD)it.color, (float)it.z);
		else if (it.e_obj){
			it.e_obj->SetAngle(it.angle);
			it.e_obj->SetARGB(it.color);
			it.e_obj->SetCenter(it.p.x, it.p.y);
			it.e_obj->Draw(x, y, w*it.scale, h*it.scale, it.sx, it.sy, it.sw, it.sh);
		}
	}

	rolist.clear();
}

void Campus::Add(iex2DObj* obj, s32 x, s32 y, s32 w, s32 h, s32 sx, s32 sy, s32 sw, s32 sh, POINT p,
	float angle, float scale, u32 dwFlags, IEXCOLOR color, float z){
	RenderObject ro;
	ro.obj = obj;
	ro.e_obj = nullptr;
	ro.x = x;
	ro.y = y;
	ro.w = w;
	ro.h = h;
	ro.sx = sx;
	ro.sy = sy;
	ro.sw = sw;
	ro.sh = sh;
	ro.p = p;
	ro.angle = angle;
	ro.scale = scale;
	ro.dwFlags = dwFlags;
	ro.color = color;
	ro.z = z;

	rolist.push_back(ro);
}

void Campus::Add(EDX::EDX_2DObj* obj, s32 x, s32 y){
	RenderObject ro;
	ro.e_obj = obj;
	ro.obj = nullptr;
	ro.x = x;
	ro.y = y;
	ro.w = obj->info.Width;
	ro.h = obj->info.Height;
	ro.sx = 0;
	ro.sy = 0;
	ro.sw = obj->info.Width;
	ro.sh = obj->info.Height;
	ro.p = GetPoint(0, 0);
	ro.angle = 0;
	ro.scale = 1;
	ro.dwFlags = RS_COPY;
	ro.color = 0xffffffff;
	ro.z = 1.0f;

	rolist.push_back(ro);
}
void Campus::Add(EDX::EDX_2DObj* obj, s32 x, s32 y, POINT p, float angle, float scale, IEXCOLOR color){
	RenderObject ro;
	ro.e_obj = obj;
	ro.obj = nullptr;
	ro.x = x;
	ro.y = y;
	ro.w = obj->info.Width;
	ro.h = obj->info.Height;
	ro.sx = 0;
	ro.sy = 0;
	ro.sw = obj->info.Width;
	ro.sh = obj->info.Height;
	ro.p = p;
	ro.angle = angle;
	ro.scale = scale;
	ro.dwFlags = RS_COPY;
	ro.color = color;
	ro.z = 1.0f;

	rolist.push_back(ro);
}

void Campus::Add(EDX::EDX_2DObj* obj, s32 x, s32 y, s32 w, s32 h, s32 sx, s32 sy, s32 sw, s32 sh){
	RenderObject ro;
	ro.e_obj = obj;
	ro.obj = nullptr;
	ro.x = x;
	ro.y = y;
	ro.w = w;
	ro.h = h;
	ro.sx = sx;
	ro.sy = sy;
	ro.sw = sw;
	ro.sh = sh;
	ro.p = GetPoint(0,0);
	ro.angle = 0;
	ro.scale = 1;
	ro.dwFlags = RS_COPY;
	ro.color = 0xffffffff;
	ro.z = 1.0f;

	rolist.push_back(ro);
}

void Campus::Add(EDX::EDX_2DObj* obj, s32 x, s32 y, s32 w, s32 h, s32 sx, s32 sy, s32 sw, s32 sh,
	POINT p, float angle, float scale, IEXCOLOR color){
	RenderObject ro;
	ro.e_obj = obj;
	ro.obj = nullptr;
	ro.x = x;
	ro.y = y;
	ro.w = w;
	ro.h = h;
	ro.sx = sx;
	ro.sy = sy;
	ro.sw = sw;
	ro.sh = sh;
	ro.p = p;
	ro.angle = angle;
	ro.scale = scale;
	ro.dwFlags = RS_COPY;
	ro.color = color;
	ro.z = 1.0f;

	rolist.push_back(ro);
}

void Campus::SetPos(POINT pos){
	RECT WindowSize;
	iexSystem::GetScreenRect(ScreenMode, WindowSize);
	pos.x -= WindowSize.right*0.5f;
	pos.y -= WindowSize.bottom*0.5f;
	pastcpos = nextcpos = cpos = pos;
	time = 1.0f;
}
void Campus::TimeReset()
{
	time = 0.0f;
}
void Campus::SetNextPos(POINT next){
	RECT WindowSize;
	iexSystem::GetScreenRect(ScreenMode, WindowSize);
	next.x -= WindowSize.right*0.5f;
	next.y -= WindowSize.bottom*0.5f;
	pastcpos = cpos;
	nextcpos = next;
	//time = 0.0f;
}

void Campus::SetMoveSpeed(float Speed){
	if (Speed < 0.0f || Speed > 1.0f)return;
	MoveSpeed = Speed;
}

void Campus::SetZoom(float Zoom){
	this->Zoom = Zoom;
}

bool Campus::IsMoveEnd(){
	if (time >= 1.0f){
		return true;
	}
	return false;
}