#include "../IEX/iextreme.h"
#include	"system/System.h"
#include	"system/Framework.h"
#include "../EDX/EDXLIB.h"
#include "DataOwner.h"
#include "sceneSelect.h"
#include "sceneTitle.h"
#include "sceneMain.h"
#include "Control.h"
#include "Campus.h"
#include "Pumpkin.h"

void Bat::Init(char* filepath, int x, int offsety){
	pic = new EDX::EDX_2DObj(filepath);
	pic->SetScale(BAT_SCALE);
	pos = EDX::GetV(x, 0, 0);
	up = EDX::GetV(0, MOVE_INTER*-0.5f, 0);
	down = EDX::GetV(0, MOVE_INTER*0.5f, 0);
	udflg = false;
	offset_Y = offsety;
	time = (float)rand() / RAND_MAX;
}

void Bat::Update(){
	if (udflg){
		time += MOVE_POW;
		if (time >= 1.0f)udflg = false;
	}
	else{
		time -= MOVE_POW;
		if (time <= 0.0f)udflg = true;
	}

	float rate = time * time * (3.0f - 2.0f * time);
	pos.y = (long)(up.y * (1.0f - rate) + down.y * rate);
}

void Bat::Render(){
	Campus::GetInst()->Add(pic, pos.x, pos.y + offset_Y);
}

bool Bat::ClickCheck(int mx, int my){
	if (PushCheck(mx, my,
		pos.x + BAT_CHECK_X*(BAT_SCALE + 1), pos.y + offset_Y + BAT_CHECK_Y*(BAT_SCALE + 0.5f),
		BAT_CHECK_W*(BAT_SCALE + 1), BAT_CHECK_H * (BAT_SCALE)))return true;
	return false;
}

void Bat::Release(){
	delete pic;
}