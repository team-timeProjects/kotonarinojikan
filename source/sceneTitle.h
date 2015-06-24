
#ifndef		_SCENETITLE_H_
#define		_SCENETITLE_H_

//*****************************************************************************
//
//	sceneTitleクラス
//
//*****************************************************************************


class sceneTitle :public Scene
{
	//----------- field -----------
private:
	//	画像
	EDX::EDX_2DObj*		m_Back1;
	EDX::EDX_2DObj*		m_Back2;
	EDX::EDX_2DObj*		m_Star;

	iex2DObj*		m_Logo;
	iex2DObj*		m_Start;
	iex2DObj*		m_End;

	//	画像パラメータ
	const int	ButtonSize = 250;
	ImageParam	logoparam;
	ImageParam	startparam;
	ImageParam	endparam;
	float				startAngle;
	float				endAngle;

	//	モード
	int					mode;
	enum MODE
	{
		MODE_SELECT,
		MODE_MOVE_BAT,
		MODE_SCENE_CHANGE,
		MODE_EXIT,
	};

	//----------- method ----------
public:
	//	初期化・解放
	static bool IsReturnTitle;
	sceneTitle(void);
	~sceneTitle(void);
	bool	Initialize(void)override;

	//	更新・描画
	void	Update(void)override;
	void	Render(void)override;
	void	RenderObject(iex2DObj* obj, ImageParam param, int sx, int sy, int sw, int sh, float angle);

	//	動作関数
	void	ModeSelect(void);
	void	ModeMoveMenu(void);
	void	ModeChangeScene(void);
	bool	OnCursorCheck(POINT p, ImageParam i_param);
};

//*****************************************************************************
#endif //#ifndef _SCENETITLE_H_
