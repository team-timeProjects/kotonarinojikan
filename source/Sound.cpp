#include "Sound.h"


//*****************************************************************************************************************************
//	初期化
//*****************************************************************************************************************************
void Sound::Initialize()
{
	//bufferのMAX 127
	//例：
	//IEX_SetWAV(0, "DATA\\SOUND\\BGM00.wav");	//	タイトル

	//BGM
	IEX_SetWAV(0, "DATA\\SOUND\\BGM\\main.wav");
	IEX_SetWAV(1, "DATA\\SOUND\\BGM\\select.wav");
	IEX_SetWAV(2, "DATA\\SOUND\\BGM\\titleInto.wav");
	IEX_SetWAV(3, "DATA\\SOUND\\BGM\\titleMain.wav");

	//SE
	IEX_SetWAV(4, "DATA\\SOUND\\SE\\akeru1.wav");
	IEX_SetWAV(5, "DATA\\SOUND\\SE\\akeru2.wav");
	IEX_SetWAV(6, "DATA\\SOUND\\SE\\akeru3.wav");
	IEX_SetWAV(7, "DATA\\SOUND\\SE\\basabasa.wav");
	IEX_SetWAV(8, "DATA\\SOUND\\SE\\bat.wav");
	IEX_SetWAV(9, "DATA\\SOUND\\SE\\clock.wav");
	IEX_SetWAV(10, "DATA\\SOUND\\SE\\clock2.wav");
	IEX_SetWAV(11, "DATA\\SOUND\\SE\\clock3.wav");
	IEX_SetWAV(12, "DATA\\SOUND\\SE\\clock4.wav");
	IEX_SetWAV(13, "DATA\\SOUND\\SE\\drumroll.wav");
	IEX_SetWAV(14, "DATA\\SOUND\\SE\\kachako.wav");
	IEX_SetWAV(15, "DATA\\SOUND\\SE\\ufufufu.wav");
	IEX_SetWAV(16, "DATA\\SOUND\\SE\\ジングル\\good.wav");
	IEX_SetWAV(17, "DATA\\SOUND\\SE\\ジングル\\miss.wav");

	IEX_SetWAV(18, "DATA\\SOUND\\newSE\\正解音編集\\正解音1.wav");
	IEX_SetWAV(19, "DATA\\SOUND\\newSE\\正解音編集\\正解音2.wav");
	IEX_SetWAV(20, "DATA\\SOUND\\newSE\\正解音編集\\正解音3.wav");
	IEX_SetWAV(21, "DATA\\SOUND\\newSE\\正解音編集\\正解音4.wav");
	IEX_SetWAV(22, "DATA\\SOUND\\newSE\\正解音編集\\正解音5.wav");


	IEX_SetWAV(23, "DATA\\SOUND\\newSE\\失敗音候補.wav");
	IEX_SetWAV(24, "DATA\\SOUND\\newSE\\決定音候補.wav");
	IEX_SetWAV(25, "DATA\\SOUND\\newSE\\口開く.wav");


	for(int i=0; i<SE_MAX; i++){
		IEX_SetSoundVolume(i, -1000);
	}
}