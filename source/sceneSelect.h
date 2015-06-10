
#ifndef		_SCENESELECT_H_
#define		_SCENESELECT_H_

//*****************************************************************************
//
//	sceneSelectクラス
//
//*****************************************************************************
bool PushCheck(int px, int py, int x, int y, int w, int h);

class Bat{
private:
	const float BAT_SCALE = 0.75;
	static const int BAT_CHECK_X = 50;
	static const int BAT_CHECK_Y = 130;
	static const int BAT_CHECK_W = 410;
	static const int BAT_CHECK_H = 250;

	static const int MOVE_INTER = 40;
	const float MOVE_POW = 0.01f;
	EDX::EDX_2DObj* pic;
	EDX::Vector pos;
	EDX::Vector up;
	EDX::Vector down;
	float time;
	int offset_Y;
	bool udflg;
public:
	void Init(char* filepath, int x, int offsety);
	void Update();
	void Render();
	EDX::Vector GetPos(){ return EDX::GetV(pos.x, pos.y + offset_Y, 0); }
	bool ClickCheck( int mx, int my);
	void Release();
};


class sceneSelect :public Scene
{
private:
	static const int STAGEPIC_MAX = 30;
	static const int WAIT_TIME = 60;

	static const int OPEN_STAGE = 3;

	static const int BAT_X_SPACE = 350;
	static const int BAT_PIC_WH = 512;
	static const int CAM_OFFSET_Y = -130;

	static const int SWITCH_TITLE_WIDTH = 200;
	static const int SWITCH_GAME_WIDTH = 230;
	static const int SWITCH_HEIGHT = 140;
	static const int SWITCH_TITLE_X = 115;
	static const int SWITCH_GAME_X = 985;
	static const int SWITCH_Y = 610;
	EDX::EDX_2DObj* Back1;
	EDX::EDX_2DObj* Back2;
	Bat bat[STAGEPIC_MAX];

	int Next_Alpha = 0;
	EDX::EDX_2DObj* Next_Title;
	EDX::EDX_2DObj* Next_Game;

	bool NextTitle;

	enum{
		SCENE_FADEIN,
		SCENE_SELECT,
		SCENE_NEXT,
	};
	int SceneState = SCENE_FADEIN;
	int SelectStage = 0;
	int save_SelectStage = 0;
	int WaitTime = 0;
public:
	//	初期化・解放
	sceneSelect(void){}
	~sceneSelect(void);
	bool	Initialize(void);

	//	更新・描画
	void	Update(void);
	void	Render(void);

	void	S_FADEIN();
	void	S_SELECT();
	void	S_NEXT();
};

//*****************************************************************************
#endif //#ifndef _SCENETITLE_H_
