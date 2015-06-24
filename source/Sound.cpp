#include "Sound.h"


//*****************************************************************************************************************************
//	初期化
//*****************************************************************************************************************************
void Sound::Initialize()
{
	//bufferのMAX 127
	//例：
	//IEX_SetWAV(0, "DATA\\Sound\\BGM00.wav");	//	タイトル

	//BGM
	IEX_SetWAV(0, "DATA\\Sound\\BGM\\main.wav");
	IEX_SetWAV(1, "DATA\\Sound\\BGM\\select.wav");
	IEX_SetWAV(2, "DATA\\Sound\\BGM\\titleInto.wav");
	IEX_SetWAV(3, "DATA\\Sound\\BGM\\titleMain.wav");

	//SE
	IEX_SetWAV(4, "DATA\\Sound\\SE\\akeru1.wav");
	IEX_SetWAV(5, "DATA\\Sound\\SE\\akeru2.wav");
	IEX_SetWAV(6, "DATA\\Sound\\SE\\akeru3.wav");
	IEX_SetWAV(7, "DATA\\Sound\\SE\\basabasa.wav");
	IEX_SetWAV(8, "DATA\\Sound\\SE\\bat.wav");
	IEX_SetWAV(9, "DATA\\Sound\\SE\\clock.wav");
	IEX_SetWAV(10, "DATA\\Sound\\SE\\clock2.wav");
	IEX_SetWAV(11, "DATA\\Sound\\SE\\clock3.wav");
	IEX_SetWAV(12, "DATA\\Sound\\SE\\clock4.wav");
	IEX_SetWAV(13, "DATA\\Sound\\SE\\drumroll.wav");
	IEX_SetWAV(14, "DATA\\Sound\\SE\\kachako.wav");
	IEX_SetWAV(15, "DATA\\Sound\\SE\\ufufufu.wav");
	IEX_SetWAV(16, "DATA\\Sound\\SE\\ジングル\\good.wav");
	IEX_SetWAV(17, "DATA\\Sound\\SE\\ジングル\\miss.wav");

	IEX_SetWAV(18, "DATA\\Sound\\newSE\\正解音編集\\正解音1.wav");
	IEX_SetWAV(19, "DATA\\Sound\\newSE\\正解音編集\\正解音2.wav");
	IEX_SetWAV(20, "DATA\\Sound\\newSE\\正解音編集\\正解音3.wav");
	IEX_SetWAV(21, "DATA\\Sound\\newSE\\正解音編集\\正解音4.wav");
	IEX_SetWAV(22, "DATA\\Sound\\newSE\\正解音編集\\正解音5.wav");


	IEX_SetWAV(23, "DATA\\Sound\\newSE\\失敗音候補.wav");
	IEX_SetWAV(24, "DATA\\Sound\\newSE\\決定音候補.wav");
	IEX_SetWAV(25, "DATA\\Sound\\newSE\\口開く.wav");


	for(int i=0; i<SE_MAX; i++){
		IEX_SetSoundVolume(i, -1000);
	}
}