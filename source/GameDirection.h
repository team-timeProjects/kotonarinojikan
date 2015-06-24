#ifndef _GAME_DIRECTION_H_
#define _GAME_DIRECTION_H_

#pragma once

class GameDirection{
public:
	enum class State{
		WAIT, CLEAR, OVER
	};
private:
	EDX::EDX_2DObj* m_GameClear;
	EDX::EDX_2DObj* m_GameOver;

	State m_state;
	bool m_IsDirectionEnd;

	float ClearScale;

	static const int OVER_OFFSET_MAX = 50;
	static const int MAX_FRAME = 40;
	float OverAlpha;
	float OverOffset;

public:
	void Init();
	void Update();
	void Draw();
	void Finalize();

	void Reset();

	__forceinline bool GetEndFlg()const{ return m_IsDirectionEnd; }
	__forceinline void SetState(State NewState){ m_state = NewState; }
};

#endif