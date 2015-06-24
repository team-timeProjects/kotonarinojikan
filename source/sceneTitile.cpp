#include	"../IEX/iextreme.h"
#include	"system/System.h"
#include	"system/Framework.h"
#include	"../EDX/EDXLIB.h"
#include	"DataOwner.h"
#include	"Campus.h"
#include	"sceneMain.h"
#include	"sceneSelect.h"

#include	"sceneTitle.h"
#include	"TransitionBat.h"

//******************************************************************************
//
//	sceneTitle�N���X
//
//******************************************************************************
bool sceneTitle::IsReturnTitle = false;
//----------------------------------------------------------------------
//	�������E���
//----------------------------------------------------------------------

//	�R���X�g���N�^
sceneTitle::sceneTitle(void) : m_Back1(NULL), m_Back2(NULL), m_Star(NULL), m_Logo(NULL), m_Start(NULL), m_End(NULL)
{

}

//	�f�X�g���N�^
sceneTitle::~sceneTitle(void)
{
	Sound::BGM_Stop(SOUND::TITLE_MAIN);
	SafeDelete(m_Back1);
	SafeDelete(m_Back2);
	SafeDelete(m_Star);
	SafeDelete(m_Logo);
	SafeDelete(m_Start);
	SafeDelete(m_End);

}

//	������
bool sceneTitle::Initialize(void)
{
	//	���ݒ�
	iexLight::SetFog(800, 1000, 0);

	//	����
	DataOwner::GetInst()->Init();

	//	�摜������
	m_Back1 = new EDX::EDX_2DObj("DATA/�^�C�g�����/Title3.png");
	m_Back2 = new EDX::EDX_2DObj("DATA/�^�C�g�����/Title1.png");
	m_Star = new EDX::EDX_2DObj("DATA/�^�C�g�����/Title2.png");

	m_Logo = new iex2DObj("DATA/�^�C�g�����/�^�C�g�����S.png");
	logoparam = { 670, 300, 500, 500 };
	m_Start = new iex2DObj("DATA/�^�C�g�����/�Q�[�����n�߂�.png");
	startparam = { 400, 550, ButtonSize, ButtonSize };
	startAngle = 0.0f;
	m_End = new iex2DObj("DATA/�^�C�g�����/�Q�[�����I���.png");
	endparam = { 900, 550, ButtonSize, ButtonSize };
	endAngle = 0.0f;
	if (IsReturnTitle){
		TransitionBat::GetInst()->SetStep(TransitionBat::TBAT_STATE::CENTER);
		TransitionBat::GetInst()->SetNextStep(TransitionBat::TBAT_STATE::UP);
		TransitionBat::GetInst()->TimeReset();
		IsReturnTitle = false;
	}
	//	�ϐ�
	mode = MODE_SELECT;

	DataOwner::GetInst()->ifs.open("DATA/SaveData.txt");
	DataOwner::GetInst()->ifs >> DataOwner::GetInst()->OpenStage;
	
	Sound::BGM_Play(SOUND::TITLE_MAIN);
	return true;
}

//----------------------------------------------------------------------
//	�X�V�E�`��
//----------------------------------------------------------------------

//	�X�V
void sceneTitle::Update(void)
{
	TransitionBat::GetInst()->Update();
	switch (mode)
	{
	case MODE_SELECT:				ModeSelect();			break;
	case MODE_MOVE_BAT:			ModeMoveMenu();		break;
	case MODE_SCENE_CHANGE:	ModeChangeScene();	break;
	}
}

//	�`��
void sceneTitle::Render(void)
{
	//	��ʃN���A
	DataOwner::GetInst()->view->Activate();
	DataOwner::GetInst()->view->Clear();

	//	�摜�`��
	if (mode != MODE_SCENE_CHANGE)
	{
		m_Back1->Draw(0, 0);
		m_Back2->Draw(0, 0);
		static float angle = 0;
		angle -= EDX_PI_F / 720;
		m_Star->SetAngle(angle);
		m_Star->Draw(-640, -640);
		RenderObject(m_Logo, logoparam, 0, 0, 512, 512, 0.0f);
		RenderObject(m_Start, startparam, 0, 0, 256, 256, PI / 180 * 30 * sinf(startAngle));
		RenderObject(m_End, endparam, 0, 0, 256, 256, PI / 180 * 30 * sinf(endAngle));
	}

	TransitionBat::GetInst()->Render();
}

//	�I�u�W�F�N�g�`��
void	sceneTitle::RenderObject(iex2DObj* obj, ImageParam param, int sx, int sy, int sw, int sh, float angle)
{
	POINT	p;
	p.x = param.x;
	p.y = param.y;
	obj->Render(param.x - param.w / 2, param.y - param.h / 2, param.w, param.h, sx, sy, sw, sh, p, angle);
}

//----------------------------------------------------------------------
//	����֐�
//----------------------------------------------------------------------

//	�I��
void	sceneTitle::ModeSelect(void)
{
	//	�J�[�\���ʒu�擾
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(iexSystem::Window, &p);

	//	�J�[�\��������Ă���
	if (OnCursorCheck(p, startparam))
	{
		//	�N���b�N
		if (EDX::MouseGet(EDX::EDX_CLICK_L)){
			TransitionBat::GetInst()->SetStep(TransitionBat::TBAT_STATE::DOWN);
			TransitionBat::GetInst()->TimeReset();
			TransitionBat::GetInst()->SetNextStep(TransitionBat::TBAT_STATE::CENTER);
			mode = MODE_MOVE_BAT;
		}

		startAngle += 0.05f;
		startparam.w = ButtonSize;
		startparam.h = ButtonSize;
	}
	else
	{
		startAngle = 0.0f;
		startparam.w = ButtonSize;
		startparam.h = ButtonSize;
	}

	if (OnCursorCheck(p, endparam))
	{
		//	�N���b�N
		if (EDX::MouseGet(EDX::EDX_CLICK_L)){
			mode = MODE_EXIT;
			exit(0);
		}

		endAngle += 0.05f;
		endparam.w = ButtonSize;
		endparam.h = ButtonSize;
	}
	else
	{
		endAngle = 0.0f;
		endparam.w = ButtonSize;
		endparam.h = ButtonSize;
	}
}

//	�R�E������((((���i�L�́M��)��))))��Կޫ�I
void	sceneTitle::ModeMoveMenu(void)
{
	startAngle = 0.0f;
	if (TransitionBat::GetInst()->IsMoveEnd()){
		mode = MODE_SCENE_CHANGE;
	}
}

//	���j���[�ֈړ�
void	sceneTitle::ModeChangeScene(void)
{
	MainFrame->ChangeScene(new sceneSelect(DataOwner::GetInst()->OpenStage - 1));
}

//	�J�[�\��������Ă邩�`�F�b�N
bool	sceneTitle::OnCursorCheck(POINT p, ImageParam i_param)
{
	if (p.x >= i_param.x - i_param.w / 2 && p.x <= i_param.x + i_param.w / 2)
	{
		if (p.y >= i_param.y - i_param.h / 2 && p.y <= i_param.y + i_param.h / 2)
		{
			return	true;
		}
	}

	return	false;
}