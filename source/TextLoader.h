#pragma once
#include	<Windows.h>

class TextLoader
{
public:
	//初期化・解放
	TextLoader();
	~TextLoader();
	//処理
	bool Load(char* fileName);		//	ファイルの読み込み
	bool EndCheck();				//	終点確認（trueで終点
	float LoadFloat();				//	数字取得（float値
	int LoadInt();					//	数字取得（int値
	void LoadStr(LPSTR str);		//	文字列取得
	bool Search(LPSTR str);			//	文字列検索
	bool SearchLine(LPSTR str);		//	行内文字列検索
	void nextLine();				//	次の行へ移動
	void Push();					//	現在の位置を保存
	void Pop();						//	保存された位置に移動

private:
	char* fileBuf;		//読み込んだファイルの内容
	long fileSize;		//ファイルのサイズ
	long nowPoint;		//読込位置
	long keepPoint;		//読込位置の一時保存変数

	int	SearchTop();	//次の文字列の先頭へ移動


};