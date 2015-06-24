#ifndef	_SOUND_H_
#define	_SOUND_H_

#include "iextreme.h"
//*****************************************************************************************************************************
//
//		�T�E���h�N���X( BGM�ESE )
//
//*****************************************************************************************************************************

enum SOUND{
	MAIN,
	SELECT,
	TITLE_INT,
	TITLE_MAIN,
	AKERU1,
	AKERU2,
	AKERU3,
	BASA,
	BAT,
	CLOCK1,
	CLOCK2,
	CLOCK3,
	CLOCK4,
	DRUMROLL,
	KACHAKO,
	UFUFUFUFU,
	GOOD,
	MISS,

	SUCCESS1,
	SUCCESS2,
	SUCCESS3,
	SUCCESS4,
	SUCCESS5,
	FAILED,

	OK,
	PAKA,
};

class	Sound
{
public:
	//	������
	static	void	Initialize();
	//�@����
	static void BGM_Play(int n)	//BGM�����[�v�Đ�������֐�(�����͋Ȃ�ǂݍ��񂾂Ƃ��ɐU�蓖�Ă�No.)
	{
		if(!IEX_GetSoundStatus(n)){
			IEX_PlaySound(n,true);
		}
	}
	static void BGM_Stop(int n)	//No.n�Ԗڂ�BGM���Đ�����Ă�����~�߂�֐�
	{
		IEX_StopSound(n);
	}
	static void SE_Play(int n)//SE���Đ�(�����͂Ȃ�...(ry)
	{
		IEX_PlaySound(n,false);
	}

	enum{
		SE_MAX
	};
};
//*****************************************************************************************************************************
#endif
