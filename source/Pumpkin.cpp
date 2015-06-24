#include "../IEX/iextreme.h"
#include	"system/System.h"
#include	"system/Framework.h"
#include "../EDX/EDXLIB.h"
#include "Pumpkin.h"
#include	<random>


void Pumpkin::Init(){
	if (!IsInit){
		Pumpkinup = new EDX::EDX_2DObj("DATA\\ステージ選択画面\\かぼちゃ上.png");
		Pumpkindown = new EDX::EDX_2DObj("DATA\\ステージ選択画面\\pumpkin-syuuseibanEX.png");
		flash = new EDX::EDX_2DObj("DATA\\ステージ選択画面\\flash.png");
		Pumpkinup->SetScale(P_SCALE);
		Pumpkindown->SetScale(P_SCALE);
		IsInit = true;
		MoveY = 0;
		PowY = 0;
		PosY = 0;
		Flashflg = false;
	}
}
void Pumpkin::Release(){
	if (IsInit){
		SAFE_DELETE(Pumpkinup);
		SAFE_DELETE(Pumpkindown);
		SAFE_DELETE(flash);
		IsInit = false;
	}
}
void Pumpkin::Update(){
	IsMoveEndflg = false;

	Shake();

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
				Flashflg = false;
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
				Flashflg = true;
				IsMoveEndflg = true;
			}
		}
	}

	if (Flashflg){
		FlashAlpha += 50;
		if (FlashAlpha > 255){
			FlashAlpha = 255;
		}
	}
	else {
		FlashAlpha -= 50;
		if (FlashAlpha < 0){
			FlashAlpha = 0;
		}
	}

	FlashCol = ARGB(FlashAlpha, 255, 255, 255);
	flash->SetARGB(FlashCol);
}

void Pumpkin::Render(){
	Pumpkinup->Draw(OFFSET_X + ShakeVar.x, OFFSET_Y - PosY + ShakeVar.y);
	Pumpkindown->Draw(OFFSET_X + ShakeVar.x, OFFSET_Y + PosY + ShakeVar.y);
	flash->Draw(ShakeVar.x, -PosY +ShakeVar.y);
}

bool Pumpkin::IsMoveEnd(){
	return IsMoveEndflg;
}

void Pumpkin::SetOpen(bool IsOpen){
	this->IsOpen = IsOpen;
}

void Pumpkin::Reset(bool IsOpen){
	this->IsOpen = IsOpen;
	if (IsOpen){
		PosY = MAX_MOVE_Y;
		PowY = 0;
		MoveY = 0;
		IsMoveEndflg = true;
	}
	else{
		PosY = 0;
		PowY = 0;
		MoveY = 0;
		IsMoveEndflg = true;
	}
}

void Pumpkin::Shake(){
	if (ShakeTime > 0){
		ShakeTime--;
		std::uniform_real_distribution<float> rad(0.0f, 1.0f);

		std::mt19937 mtRand{ std::random_device()() };

		ShakeVar.x = rad(mtRand)*SHAKE_POWER - SHAKE_POWER / 2;
		ShakeVar.y = rad(mtRand)*SHAKE_POWER - SHAKE_POWER / 2;
	}
	else{
		ShakeVar.x = 0;
		ShakeVar.y = 0;
		ShakeTime = 0;
	}
}