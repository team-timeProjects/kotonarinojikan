#include "../IEX/iextreme.h"
#include	"system/System.h"
#include	"system/Framework.h"
#include "../EDX/EDXLIB.h"
#include "TransitionBat.h"

EDX::Vector	Lerp(const EDX::Vector p1, const EDX::Vector p2, float t)
{
	EDX::Vector Out;
	float rate = t * t * (3.0f - 2.0f * t);   // 3ŽŸŠÖ”•âŠÔ’l‚É•ÏŠ·

	Out.x = (long)(p1.x * (1.0f - rate) + p2.x * rate);
	Out.y = (long)(p1.y * (1.0f - rate) + p2.y * rate);
	Out.z = (long)(p1.z * (1.0f - rate) + p2.z * rate);
	return Out;
}

void TransitionBat::Init(){
	if (!IsLoad){
		Bat = new EDX::EDX_2DObj("DATA\\ˆÚs‰‰o\\BatTaigun.png");
		IsLoad = true;
	time = 0.0f;
	pos = down;
	nextstep = step = TBAT_STATE::DOWN;
	}
}

void TransitionBat::Release(){
	if (IsLoad){
		delete Bat;
		IsLoad = false;
	}
}

void TransitionBat::Update(){
	IsMoveEnd_Var = false;
	time += TIME_POWER;
	if (time >= 1.0f){
		IsMoveEnd_Var = true;
		time = 1.0f;
		step = nextstep;
		return;
	}
	switch (step){
	case TBAT_STATE::DOWN:
		if (nextstep == TBAT_STATE::CENTER)pos = Lerp(down, center, time);
		else if (nextstep == TBAT_STATE::UP)	pos = Lerp(down, up, time);
		break;
	case TBAT_STATE::CENTER:
		if (nextstep == TBAT_STATE::DOWN)pos = Lerp(center, down, time);
		else if(nextstep == TBAT_STATE::UP)pos = Lerp(center, up, time);
		break;
	case TBAT_STATE::UP:
		if (nextstep == TBAT_STATE::DOWN)pos = Lerp(up, down, time);
		else if(nextstep == TBAT_STATE::CENTER)pos = Lerp(up, center, time);
		break;
	}
}

void TransitionBat::Render(){
	Bat->Draw(pos.x, pos.y);
}
