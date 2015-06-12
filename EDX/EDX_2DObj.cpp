#include "../IEX/iextreme.h"
#include "EDXLIB.h"

EDX::EDX_2DObj::~EDX_2DObj(){
	if (pTexture) EDX_TextureMgr::Release(pTexture);
}

EDX::EDX_2DObj::EDX_2DObj(char * filename){
	pTexture = NULL;
	pTexture = *EDX_TextureMgr::LoadTexture(filename, &info);
	ZeroMemory(v, sizeof(VERTEXGRAPH));
	Angle = 0.0f;
	Scale = GetV(1.0f, 1.0f, 1.0f);
	LRTurnOver = FALSE;
	CenterX = CenterY = 0.0f;
	RenderType = RM::RT::TYPE_DEFAULT;
	v[0].dwColor = v[1].dwColor = v[2].dwColor = v[3].dwColor = 0xFFFFFFFF;
	v[0].z = v[1].z = v[2].z = v[3].z = 0;
	v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = 1.0f;//何これ？
}

EDX::EDX_2DObj::EDX_2DObj(UINT w, UINT h){

	HRESULT hr;
	//新しい書き込み先の作成
	hr = iexSystem::GetDevice()->CreateTexture(
		w, h,// テクスチャのサイズ
		1,
		D3DUSAGE_RENDERTARGET,// レンダリングターゲットとして利用する
		D3DFMT_A8R8G8B8, // RGBAのテクスチャモードを設定
		D3DPOOL_DEFAULT,
		&pTexture,// テクスチャ変数へのアドレス
		NULL);

	//ステンシルバッファは作成していません。
	hr = iexSystem::GetDevice()->CreateDepthStencilSurface(
		w, h,// Zバッファのサイズ
		D3DFMT_D24X8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&ZBuffer,         // Zバッファ変数へのアドレス
		NULL);
	//作成したテクスチャからサーフェス情報を取り出す
	hr = pTexture->GetSurfaceLevel(0, &Surface);
	//ビューポートの設定
	ViewPort.X = 0;
	ViewPort.Y = 0;
	info.Width = ViewPort.Width = w;
	info.Height = ViewPort.Height = h;
	ViewPort.MinZ = 0.0f;
	ViewPort.MaxZ = 1.0f;

	ZeroMemory(v, sizeof(VERTEXGRAPH));
	Scale = GetV(1.0f, 1.0f, 1.0f);
	Angle = 0.0f;
	LRTurnOver = FALSE;
	CenterX = CenterY = 0.0f;
	RenderType = RM::RT::TYPE_DEFAULT;
	v[0].dwColor = v[1].dwColor = v[2].dwColor = v[3].dwColor = 0xFFFFFFFF;
	v[0].z = v[1].z = v[2].z = v[3].z = 0;
	v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = 1.0f;//何これ？
}

void EDX::EDX_2DObj::Draw(int x, int y){
	v[0].z = v[1].z = v[2].z = v[3].z = 0;
	//原点にて拡大
	v[0].x = v[2].x = (float)(info.Width  * -0.5f - CenterX) * Scale.x;
	v[1].x = v[3].x = (float)(info.Width  * 0.5f - CenterX) *Scale.x;
	v[0].y = v[1].y = (float)(info.Height * -0.5f - CenterY) * Scale.y;
	v[2].y = v[3].y = (float)(info.Height * 0.5f - CenterY)  * Scale.y;


	//回転
	for (DWORD i = 0; i < 4; i++){
		const float xbuff = v[i].x;
		const float ybuff = v[i].y;
		v[i].x = xbuff * cos(Angle) + ybuff * sin(Angle);
		v[i].y = -xbuff * sin(Angle) + ybuff * cos(Angle);
	}

	//元の位置に戻して移動
	float buffx = (float)(x + CenterX + ((info.Width ) * 0.5f));
	float buffy = (float)(y + CenterY + ((info.Height) * 0.5f));
	v[0].x += buffx;
	v[2].x += buffx;
	v[1].x += buffx;
	v[3].x += buffx;
	v[0].y += buffy;
	v[1].y += buffy;
	v[2].y += buffy;
	v[3].y += buffy;

	//テクスチャ内の座標
	if (LRTurnOver == FALSE){
		v[0].u = v[2].u = 0.0f;
		v[1].u = v[3].u = 1.0f;
	}
	else{
		v[0].u = v[2].u = 1.0f;
		v[1].u = v[3].u = 0.0f;
	}
	v[0].v = v[1].v = 0.0f;
	v[2].v = v[3].v = 1.0f;

	//レンダーステート
	RM::SetRenderType(RenderType);


	//ポリゴンの透明度を使用する
	iexSystem::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	iexSystem::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	iexSystem::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	iexSystem::GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	iexSystem::GetDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	iexSystem::GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	//テクスチャをセット
	iexSystem::GetDevice()->SetTexture(0, pTexture);
	//セットしたテクスチャを描画する
	iexSystem::GetDevice()->SetFVF(FVF_VERTEX2DGRAPH);
	//三角形を、2つ、頂点情報と、そのサイズ、でDraw！！！
	iexSystem::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(VERTEXGRAPH));
}

void EDX::EDX_2DObj::Draw(int x, int y, int W, int H, int Tx, int Ty, int Tw, int Th){
	v[0].z = v[1].z = v[2].z = v[3].z = 0;
	v[0].x = v[2].x = (float)(W  * -0.5f - CenterX) * Scale.x;
	v[1].x = v[3].x = (float)(W  * 0.5f - CenterX) *Scale.x;
	v[0].y = v[1].y = (float)(H * -0.5f - CenterY) * Scale.y;
	v[2].y = v[3].y = (float)(H * 0.5f - CenterY)  * Scale.y;

	for (int i = 0; i < 4; i++){
		const float xbuff = v[i].x;
		const float ybuff = v[i].y;
		v[i].x = xbuff * cos(Angle) + ybuff * sin(Angle);
		v[i].y = -xbuff * sin(Angle) + ybuff * cos(Angle);
	}
	float buffx = (float)(x + CenterX + ((W) / 2.0f));
	float buffy = (float)(y + CenterY + ((H ) / 2.0f));
	v[0].x += buffx;
	v[2].x += buffx;
	v[1].x += buffx;
	v[3].x += buffx;
	v[0].y += buffy;
	v[1].y += buffy;
	v[2].y += buffy;
	v[3].y += buffy;
	//テクスチャ内の座標
	if (LRTurnOver == FALSE){
		v[0].u = v[2].u = (float)(Tx) / (float)info.Width;
		v[1].u = v[3].u = (float)(Tx + Tw) / (float)info.Width;
	}
	else{
		v[0].u = v[2].u = (float)(Tx + Tw) / (float)info.Width;
		v[1].u = v[3].u = (float)(Tx) / (float)info.Width;
	}
	v[0].v = v[1].v = (float)(Ty) / (float)info.Height;
	v[2].v = v[3].v = (float)(Ty + Th) / (float)info.Height;

	//レンダーステート
	RM::SetRenderType(RenderType);


	iexSystem::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	iexSystem::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	iexSystem::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	iexSystem::GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	iexSystem::GetDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	iexSystem::GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	iexSystem::GetDevice()->SetTexture(0, pTexture);
	iexSystem::GetDevice()->SetFVF(FVF_VERTEX2DGRAPH);
	iexSystem::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(VERTEXGRAPH));

}

void EDX::EDX_2DObj::SetScale(float Scale){
	this->Scale.x = this->Scale.y = this->Scale.z = Scale;

}

void EDX::EDX_2DObj::SetScale(float x, float y, float z){
	this->Scale.x = x;
	this->Scale.y = y;
	this->Scale.z = z;

}

void EDX::EDX_2DObj::SetScale(Vector Scale){
	this->Scale.x = Scale.x;
	this->Scale.y = Scale.y;
	this->Scale.z = Scale.z;

}

void EDX::EDX_2DObj::SetAngle(float Angle){
	this->Angle = Angle;

}

void EDX::EDX_2DObj::SetCenter(int X, int Y){
	this->CenterX = X;
	this->CenterY = Y;
}

void EDX::EDX_2DObj::SetVtARGB(int VtNum, COLOR A, COLOR R, COLOR G, COLOR B){
	if (0 > VtNum || VtNum > 3)return;
	COLORBUFFER buff;
	buff.dwColor = 0x00000000;
	buff.b |= B;
	buff.g |= G;
	buff.r |= R;
	buff.a |= A;


	v[VtNum].dwColor = buff.dwColor;
}

void EDX::EDX_2DObj::SetARGB(COLOR A, COLOR R, COLOR G, COLOR B){
	COLORBUFFER buff;
	buff.dwColor = 0x00000000;
	buff.b |= B;
	buff.g |= G;
	buff.r |= R;
	buff.a |= A;

	v[0].dwColor = buff.dwColor;
	v[1].dwColor = buff.dwColor;
	v[2].dwColor = buff.dwColor;
	v[3].dwColor = buff.dwColor;
}

void EDX::EDX_2DObj::SetARGB(DWORD ARGB){

	v[0].dwColor = ARGB;
	v[1].dwColor = ARGB;
	v[2].dwColor = ARGB;
	v[3].dwColor = ARGB;
}

void EDX::EDX_2DObj::SetTurnOver(BOOL LR){
	LRTurnOver = LR;
}

void EDX::EDX_2DObj::SetRenderTarget(DWORD Idx, bool IsClear){
	HRESULT hr;
	hr = iexSystem::GetDevice()->SetRenderTarget(Idx, nullptr);
	hr = iexSystem::GetDevice()->SetRenderTarget(Idx, Surface);        // ◆レンダリングターゲット
	//	hr = iexSystem::GetDevice()->SetDepthStencilSurface(ZBuffer);    // ◆Zバッファ
	//hr = iexSystem::GetDevice()->SetViewport(&ViewPort);             // ◆ビューポート
	////初期化する
	if (IsClear){
		iexSystem::GetDevice()->Clear(0, NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_ARGB(0,90, 90, 90),          // 背景を黒に設定
			1.0f, 0);
	}
	return;
}

void EDX::EDX_2DObj::SetRenderType(EDX::RM::RT RenderType){
	this->RenderType = RenderType;
}