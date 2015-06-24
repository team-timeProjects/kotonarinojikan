#include	"iextreme.h"
#include	"system/system.h"
#include	"system/Framework.h"

#include	"../EDX/EDXLIB.h"

#include	"DataOwner.h"
#include	"GameDirection.h"


void GameDirection::Init(){
	m_GameClear = new EDX::EDX_2DObj("DATA\\ゲーム画面\\ゲームクリア.png");
	m_GameOver = new EDX::EDX_2DObj("DATA\\ゲーム画面\\ゲームオーバー.png");

	m_state = State::WAIT;
	m_IsDirectionEnd = false;
	
	ClearScale = 0.0f;
	OverAlpha = 0;
	OverOffset = 0;
}

void GameDirection::Update(){
	m_IsDirectionEnd = false;
	switch (m_state){
	case State::WAIT:
		
		break;
	case State::CLEAR:
		ClearScale += 1.0f / (float)MAX_FRAME;
		if (ClearScale > 1.0f)ClearScale = 1.0;
		m_GameClear->SetScale(ClearScale);
		break;
	case State::OVER:
		OverAlpha += 255.0f / (float)MAX_FRAME;
		if (OverAlpha > 255)OverAlpha = 255;
		m_GameOver->SetARGB(ARGB((int)OverAlpha, 255, 255, 255));

		OverOffset++;
		if (OverOffset > OVER_OFFSET_MAX)OverOffset = OVER_OFFSET_MAX;
		break;
	}
}

void GameDirection::Draw(){
	DWORD boxcol;
	switch (m_state){
	case State::WAIT:

		break;
	case State::CLEAR:
		m_GameClear->Draw(1280 / 2 - (m_GameClear->info.Width / 2), 720 / 2 - (m_GameClear->info.Height / 2));
		break;
	case State::OVER:
		boxcol = ARGB((int)OverAlpha / 3, 0, 0, 0);
		EDX::DrawBox(0, 0, boxcol, 1280, 0, boxcol, 0, 720, boxcol, 1280, 720,boxcol);
		m_GameOver->Draw(1280 / 2 - (m_GameOver->info.Width / 2), 720 / 2 - (m_GameOver->info.Height / 2 )- OverOffset);

		break;
	}
}

void GameDirection::Finalize(){
	delete m_GameClear;
	delete m_GameOver;
}

void GameDirection::Reset(){
	m_state = State::WAIT;
	m_IsDirectionEnd = false;

	ClearScale = 0.0f;
	OverAlpha = 0;
	OverOffset = 0;

	m_GameClear->SetScale(ClearScale);
	m_GameOver->SetARGB(ARGB((int)OverAlpha, 255, 255, 255));
}