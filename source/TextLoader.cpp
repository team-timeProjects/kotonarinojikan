#include	"TextLoader.h"
#include	<stdio.h>

//*****************************************
//	初期化・解放
//*****************************************
TextLoader::TextLoader()	: fileBuf(NULL), fileSize(0), nowPoint(0), keepPoint(0)
{
}

TextLoader::~TextLoader()
{
	if( fileBuf ) delete[] fileBuf;
	fileBuf = NULL;
}


//*****************************************
//	初期化・解放
//*****************************************

//引数　：[in]ファイル名
//戻り値：正常に開けたかどうか
//処理　：指定されたファイルをテキスト形式で読み込み、内容をバッファに確保する。
bool TextLoader::Load(char* fileName)
{
	FILE* fp;
	fopen_s(&fp, fileName, "r");
	if( fp == NULL ) return false;

	fseek( fp, 0, SEEK_END );
	fileSize = ftell( fp );

	if( fileBuf ){
		delete[] fileBuf;
		fileBuf = NULL;
	}
	fileBuf = new char[fileSize+1];

	fseek( fp, 0, SEEK_SET);
	fread(fileBuf, sizeof(char), fileSize, fp);
	fileBuf[fileSize] = '\n';

	fclose(fp);

	return true;
}

//引数　：なし
//戻り値：終点かどうか
//処理　：読み込み位置が終点かどうかを返す。
bool	TextLoader::EndCheck()
{
	if( nowPoint > fileSize ) return true;
	return false;
}

//引数　：なし
//戻り値：float型の値
//処理　：読み込んだ値をfloat型として返す。
float TextLoader::LoadFloat()
{
	char	temp[20];
	LoadStr(temp);

	return (float)atof(temp);
}

//引数　：なし
//戻り値：int型の値
//処理　：読み込んだ値をint型として返す。
int TextLoader::LoadInt()
{
	char	temp[20];
	LoadStr(temp);

	return atoi(temp);
}

//引数　：[out]文字列
//戻り値：なし
//処理　：区切りまで文字列を抜き出して引数の変数に格納する。
void	TextLoader::LoadStr(LPSTR str)
{
	int		len = 0;
	
	int mode = SearchTop();
	if( nowPoint >= fileSize ) return;
	
	//	名前読み込みモード
	if( mode == 1 ){
//		str[len] = '"';
//		len   ++;
		nowPoint ++;
		while(true){
			if( IsDBCSLeadByte(fileBuf[nowPoint]) ){ 
				str[len] = fileBuf[nowPoint];
				len   ++;
				nowPoint ++;
			} else {
				if( fileBuf[nowPoint] == '"' ) break;
			}
			if( nowPoint >= fileSize ) break;

			str[len] = fileBuf[nowPoint];
			len   ++;
			nowPoint ++;
		}
//		str[len] = '"';
//		len   ++;
		str[len] = '\0';
		nowPoint ++;
		return;
	}


	while(true){
		if( nowPoint>0 && !IsDBCSLeadByte(fileBuf[nowPoint-1]) ){ 
			if( fileBuf[nowPoint] == ';' ) break;
			if( fileBuf[nowPoint] == ')' ) break;
			if( fileBuf[nowPoint] == '(' ) break;
			if( fileBuf[nowPoint] == ',' ) break;
			if( fileBuf[nowPoint] == ' ' ) break;
			if( fileBuf[nowPoint] == '\t' ) break;
			if( fileBuf[nowPoint] == 0x0d ) break;
			if( fileBuf[nowPoint] == 0x0a ) break;
			if( nowPoint >= fileSize ) break;
		}
		str[len] = fileBuf[nowPoint];
		len   ++;
		nowPoint ++;
	}
	str[len] = '\0';
	nowPoint ++;
}

//引数　：なし
//戻り値：正常に処理が行えた場合は0、終点の場合は-1、"の場合は1を返す。
//処理　：次の文字列の先頭へ移動
int TextLoader::SearchTop()
{
	while(true){
		if( nowPoint >= fileSize ) return -1;

		if( IsCharAlphaNumeric(fileBuf[nowPoint]) ) break;
		if( IsDBCSLeadByte(fileBuf[nowPoint]) ) break;

		if( fileBuf[nowPoint] == '{' ) break;
		if( fileBuf[nowPoint] == '}' ) break;
		if( fileBuf[nowPoint] == '"' ) return 1;
		if( fileBuf[nowPoint] == '.' ) break;
		if( fileBuf[nowPoint] == '-' ) break;
		if( fileBuf[nowPoint] == '=' ) break;
		nowPoint ++;
		if( nowPoint >= fileSize ) return -1;
	}
	return	0;
}

//引数　：[in]検索する文字列
//戻り値：文字列が存在したかどうか
//処理　：与えられた文字列を現在の地点から終点まで検索する。
bool	TextLoader::Search( LPSTR str )
{
	char	temp[128];

	while(!EndCheck()){
		LoadStr(temp);
		if( lstrcmp( temp, str ) == 0 ) return true;
	}
	return false;
}

//引数　：[in]検索する文字列
//戻り値：文字列が存在したかどうか
//処理　：与えられた文字列を現在の地点から行の中で検索する。読込位置は更新しない。
bool	TextLoader::SearchLine( LPSTR str )
{
	char	temp[128];
	DWORD	oldIndex = nowPoint;
	
	//	行頭設定
	while(true){
		nowPoint --;
		if( fileBuf[nowPoint] == '\n' ) break;
		if( fileBuf[nowPoint] == '\r' ) break;
	}
	nowPoint ++;
	//	行内検索
	while(true){
		if( fileBuf[nowPoint] == '\n' ) break;
		if( fileBuf[nowPoint] == '\r' ) break;
		if( !IsCharAlphaNumeric(fileBuf[nowPoint]) ){
			nowPoint ++;
			continue;
		}
		LoadStr(temp);
		if( lstrcmp( temp, str ) == 0 ) return true;
	}
	nowPoint = oldIndex;
	return false;
}

//引数　：なし
//戻り値：なし
//処理　：次の行へ移動する。
void	TextLoader::nextLine()
{
	while(true){
		if( nowPoint >= fileSize ) return;
		if( fileBuf[nowPoint] == '\n' ) break;
		if( fileBuf[nowPoint] == '\r' ) break;
		nowPoint++;
	}
}

//引数　：なし
//戻り値：なし
//処理　：現在の読込位置を確保する。
void TextLoader::Push()
{
	keepPoint = nowPoint;
}

//引数　：なし
//戻り値：なし
//処理　：確保している場所に読込位置を移動する。
void TextLoader::Pop()
{
	nowPoint = keepPoint;
}