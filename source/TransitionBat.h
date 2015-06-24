#ifndef _TRANSITION_BAT_H_
#define _TRANSITION_BAT_H_

class TransitionBat{
private:
	TransitionBat() :IsLoad(false){}
	TransitionBat(TransitionBat&) {}
	TransitionBat& operator = (const TransitionBat&) {}

	EDX::EDX_2DObj* Bat;
	bool IsLoad;
	float time = 0;
	const float TIME_POWER = 0.01f;
public:
	enum class TBAT_STATE{
		DOWN, CENTER, UP
	};
private:
	TBAT_STATE step;
	TBAT_STATE nextstep;
	bool IsMoveEnd_Var;
	bool Sound_Played;
	EDX::Vector pos;
	EDX::Vector down = EDX::GetV(800, 1100, 0);
	EDX::Vector center = EDX::GetV(-1900, -1900, 0);
	EDX::Vector up = EDX::GetV(-800 * 4, -1100 * 4, 0);
public:
	void Init();
	void Release();
	void Update();
	void Render();


	void SetNextStep(TBAT_STATE next);
	void SetStep(TBAT_STATE step){
		IsMoveEnd_Var = true;
		time = 1.0f;
		this->step = nextstep = step;
		switch (step){
		case TBAT_STATE::DOWN:
			pos = down;
			break;
		case TBAT_STATE::CENTER:
			pos = center;
			break;
		case TBAT_STATE::UP:
			pos = up;
			break;
		}
	}
	void TimeReset();
	bool IsMoveEnd(){ return IsMoveEnd_Var; }

	__forceinline static TransitionBat* GetInst(){
		static TransitionBat c;
		return &c;
	}

};

#endif