#include "../IEX/iextreme.h"
#include "EDXLIB.h"

struct EDX::EDX_TextureMgr::TexSave EDX::EDX_TextureMgr::TexSave[MAX_TEXTURE];

BOOL EDX::EDX_TextureMgr::Init(){
	for (int i = 0; i<MAX_TEXTURE; i++){
		if (TexSave[i].lpTexture) TexSave[i].lpTexture->Release();
		TexSave[i].lpTexture = NULL;
		TexSave[i].UseObjCount = 0;
	}
	return TRUE;
}

LPDIRECT3DTEXTURE9* EDX::EDX_TextureMgr::LoadTexture(char* filename, D3DXIMAGE_INFO* lpinfo){
	int SaveTexNo = -1;
	HRESULT hr;
	//既に読みこんであるテクスチャかを調べる
	for (int i = 0; i<MAX_TEXTURE; i++){
		//テクスチャの中に何も入っていないならcontinueで戻る
		if (!TexSave[i].lpTexture) continue;

		//ファイル名で比較して一致してないなら戻る
		if (lstrcmpi(TexSave[i].FilePass, filename)) continue;
		//一致したものを見つけたならその要素番号を記録してループから出る
		SaveTexNo = i;
		break;
	}

	//SaveTexNoが-1ということは、見つからなかったということ
	if (SaveTexNo == -1){
		//新しいテクスチャなので新規登録
		//空きを探す
		for (SaveTexNo = 0; SaveTexNo < MAX_TEXTURE; SaveTexNo++){
			if (!TexSave[SaveTexNo].UseObjCount) break;
		}
		//もしいっぱいだったら終わり
		if (SaveTexNo > MAX_TEXTURE)return NULL;
		//引数のファイルネームをファイルパスに保存しておく
		CopyMemory(TexSave[SaveTexNo].FilePass, filename, strlen(filename) + 1);
		//ここから新規登録
		//画像の情報をもらう
		D3DXGetImageInfoFromFile(filename, &TexSave[SaveTexNo].info);

		hr =  D3DXCreateTextureFromFileEx(//画像からテクスチャーを作る
			iexSystem::GetDevice(),//ビデオカードデバイス
			(LPCSTR)filename,//ファイルの位置
			TexSave[SaveTexNo].info.Width, TexSave[SaveTexNo].info.Height,//上で取得しているサイズを使用する
			5,//ミップマップレベル。
			0,//テクスチャの性質や確保するメモリの場所を指定する。ミップマップ自動生成にしてある
			TexSave[SaveTexNo].info.Format,//画像の書式。上で取得している物を使う
			D3DPOOL_DEFAULT,//テクスチャを格納するメモリの位置とフラグについて。
			D3DX_FILTER_LINEAR,//線形補間
			D3DX_FILTER_LINEAR,//ミップマップに大しても線形補間
			NULL,//透明にする色を指定できる。したくない場合は0とかNULL
			NULL,//元の画像の情報を格納するD3DXIMAGE_INFO構造体へのポインタを渡す。上で保存してるのでいらない
			NULL,
			&TexSave[SaveTexNo].lpTexture//テクスチャオブジェクトへのポインタ
			);
	}
	if (lpinfo){
		*lpinfo = TexSave[SaveTexNo].info;
	}
	TexSave[SaveTexNo].UseObjCount++;
	return &TexSave[SaveTexNo].lpTexture;
}

void EDX::EDX_TextureMgr::Release(LPDIRECT3DTEXTURE9 lpTexture){
	if (!lpTexture)return;
	int SaveTexNo = -1;
	//送られてきたテクスチャと同じ物を探す
	for (int i = 0; i<MAX_TEXTURE; i++){
		//テクスチャの中に何も入っていないなら戻る
		if (!TexSave[i].lpTexture) continue;
		//一致してないなら戻る
		if (TexSave[i].lpTexture != lpTexture) continue;
		//位置してたらその要素番号を記録して戻る
		SaveTexNo = i;
		break;
	}

	//noが-1以外ということは見つかっている
	if (SaveTexNo != -1){
		//そのテクスチャを利用している2DObjのカウンタを減らす
		TexSave[SaveTexNo].UseObjCount--;
		//まだ使っている2DObjが残っているならここで終わり
		if (TexSave[SaveTexNo].UseObjCount > 0) return;
		//そうでないならテクスチャを解除
		//edxSystem::GetDevice()->SetTexture(0, NULL);
		//	テクスチャ解放して初期化
		if (TexSave[SaveTexNo].lpTexture){
			TexSave[SaveTexNo].lpTexture->Release();
			TexSave[SaveTexNo].lpTexture = NULL;
		}
		TexSave[SaveTexNo].UseObjCount = 0;
	}else {//LoadTexture関数でロードされていない場合は見つからないので、リリースするだけ
		lpTexture->Release();
	}
}