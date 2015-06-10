
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
	iex2DObj*		m_Back;
	iex2DObj*		m_Logo;
	iex2DObj*		m_Start;
	iex2DObj*		m_End;
	iex2DObj*		m_Bat;

	//	画像パラメータ
	ImageParam	backparam;
	ImageParam	logoparam;
	ImageParam	startparam;
	ImageParam	endparam;
	ImageParam	batparam;
	float				startAngle;
	float				endAngle;

	//	モード
	int					mode;
	enum MODE
	{
		MODE_SELECT,
		MODE_MOVE_BAT,
		MODE_SCENE_CHANGE,
	};

	//----------- method ----------
public:
	//	初期化・解放
	sceneTitle( void );
	~sceneTitle( void );
	bool	Initialize( void )override;

	//	更新・描画
	void	Update( void )override;
	void	Render( void )override;
	void	RenderObject( iex2DObj* obj, ImageParam param, int sx, int sy, int sw, int sh, float angle );

	//	動作関数
	void	ModeSelect( void );
	void	ModeMoveMenu( void );
	void	ModeChangeScene( void );
	bool	OnCursorCheck( POINT p, ImageParam i_param );
};

//*****************************************************************************
#endif //#ifndef _SCENETITLE_H_
