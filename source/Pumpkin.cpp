#include "../IEX/iextreme.h"
#include	"system/System.h"
#include	"system/Framework.h"
#include "../EDX/EDXLIB.h"
#include "Pumpkin.h"


void Pumpkin::Init(){
	if (!IsInit){
		Pumpkinup = new EDX::EDX_2DObj("DATA\\ステージ選択画面\\pumpkin up.png");
		Pumpkindown = new EDX::EDX_2DObj("DATA\\ステージ選択画面\\pumpkin down.png");
		Pumpkinup->SetScale(P_SCALE);
		Pumpkindown->SetScale(P_SCALE);
		IsInit = true;
		MoveY = 0;
		PowY = 0;
		PosY = 0;
	}
}
void Pumpkin::Release(){
	if (IsInit){
		SAFE_DELETE(Pumpkinup);
		SAFE_DELETE(Pumpkindown);
		IsInit = false;
	}
}
void Pumpkin::Update(){
	IsMoveEndflg = false;
	if (IsOpen){
		if (!IsMoveEndflg){
			PowY += POWER_Y;
			MoveY += PowY;
			PosY += MoveY;
			if (MAX_MOVE_Y < PosY){
				PosY = MAX_MOVE_Y;
				PowY = 0;
				MoveY = 0;
				IsMoveEndflg = true;
			}
		}
	}
	else{
		if (!IsMoveEndflg){
			PowY += POWER_Y;
			MoveY += PowY;
			PosY -= MoveY;
			if (0 > PosY){
				PosY = 0;
				PowY = 0;
				MoveY = 0;
				IsMoveEndflg = true;
			}
		}
	}
}

void Pumpkin::Render(){
	Pumpkinup->Draw(OFFSET_X, OFFSET_Y - PosY);
	Pumpkindown->Draw(OFFSET_X, OFFSET_Y + PosY);
}

bool Pumpkin::IsMoveEnd(){
	return IsMoveEndflg;
}

void Pumpkin::SetOpen(bool IsOpen){
	this->IsOpen = IsOpen;
}