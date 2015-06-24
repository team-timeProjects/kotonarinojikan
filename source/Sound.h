#ifndef	_SOUND_H_
#define	_SOUND_H_

#include "iextreme.h"
//*****************************************************************************************************************************
//
//		サウンドクラス( BGM・SE )
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
	//	初期化
	static	void	Initialize();
	//　処理
	static void BGM_Play(int n)	//BGMをループ再生させる関数(引数は曲を読み込んだときに振り当てたNo.)
	{
		if(!IEX_GetSoundStatus(n)){
			IEX_PlaySound(n,true);
		}
	}
	static void BGM_Stop(int n)	//No.n番目のBGMが再生されていたら止める関数
	{
		IEX_StopSound(n);
	}
	static void SE_Play(int n)//SEを再生(引数はなが...(ry)
	{
		IEX_PlaySound(n,false);
	}

	enum{
		SE_MAX
	};
};
//*****************************************************************************************************************************
#endif
