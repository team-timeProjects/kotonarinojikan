#include "../IEX/iextreme.h"
#include "EDXLIB.h"

BOOL EDX::EDX_RenderMgr::ScreenUpdate(){
	BOOL success;
	success = EndSceneModule();
	success = BeginSceneModule();
	return success;
}

BOOL EDX::EDX_RenderMgr::BeginSceneModule(){
	iexSystem::GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET| D3DCLEAR_ZBUFFER,_ARGB(0,0, 0, 0), 1.0f, 0);
	iexSystem::GetDevice()->BeginScene();
	return TRUE;
}

BOOL EDX::EDX_RenderMgr::EndSceneModule(){
	iexSystem::GetDevice()->EndScene();
	if (FAILED(iexSystem::GetDevice()->Present(NULL, NULL, NULL, NULL))){
		//iexSystem::ResetDevice();
		//// 復帰可能？
		//		if (iexSystem::GetDevice()->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
		//		// ここでデバイスの再生成＆画像とか素材の再ロード
		//		}

	//	return FALSE;
	}
	return TRUE;
}

void EDX::EDX_RenderMgr::SetRenderType(RT RenderType){
	switch (RenderType){
	case TYPE_DEFAULT:
		//アルファブレンドする			
		RM::SetRS(RM::ALPHA_ENABLE);
		//Zバッファ参照有効
		RM::SetRS(RM::DEPTHREAD_ENABLE);
		//Zバッファ書き込み有効
		RM::SetRS(RM::DEPTHWRITE_ENABLE);
		//アルファテストしない
		RM::SetRS(RM::ALPHATEST_DISABLE);
		//ディザ有効
		RM::SetRS(RM::DITHER_ENABLE);
		//カリング無効
		RM::SetRS(RM::CULL_DISABLE);
		//法線正規化無効
		RM::SetRS(RM::NORMALIZE_DISABLE);
		//線形補完
		RM::SetRS(RM::LINEAR_ENABLE);
		//加算しない
		RM::SetRS(RM::ADD_DISABLE);
		break;
	case TYPE_ADD:
		//アルファブレンドする			
		RM::SetRS(RM::ALPHA_ENABLE);
		//Zバッファ参照有効
		RM::SetRS(RM::DEPTHREAD_ENABLE);
		//Zバッファ書き込み有効
		RM::SetRS(RM::DEPTHWRITE_ENABLE);
		//アルファテストしない
		RM::SetRS(RM::ALPHATEST_DISABLE);
		//ディザ有効
		RM::SetRS(RM::DITHER_ENABLE);
		//カリング無効
		RM::SetRS(RM::CULL_DISABLE);
		//法線正規化無効
		RM::SetRS(RM::NORMALIZE_DISABLE);
		//線形補完
		RM::SetRS(RM::LINEAR_ENABLE);
		//加算する
		RM::SetRS(RM::ADD_ENABLE);
		break;
	case TYPE_2DEFFECT:
		//アルファブレンドする			
		RM::SetRS(RM::ALPHA_ENABLE);
		//Zバッファ参照有効
		RM::SetRS(RM::DEPTHREAD_ENABLE);
		//Zバッファ書き込み無効
		RM::SetRS(RM::DEPTHWRITE_DISABLE);
		//アルファテストしない
		RM::SetRS(RM::ALPHATEST_DISABLE);
		//ディザ有効
		RM::SetRS(RM::DITHER_ENABLE);
		//カリング無効
		RM::SetRS(RM::CULL_DISABLE);
		//法線正規化無効
		RM::SetRS(RM::NORMALIZE_DISABLE);
		//線形補完
		RM::SetRS(RM::LINEAR_ENABLE);
		//加算しない
		RM::SetRS(RM::ADD_DISABLE);
		break;
	case TYPE_2DADDEFFECT:
		//アルファブレンドする			
		RM::SetRS(RM::ALPHA_ENABLE);
		//Zバッファ参照有効
		RM::SetRS(RM::DEPTHREAD_ENABLE);
		//Zバッファ書き込み無効
		RM::SetRS(RM::DEPTHWRITE_DISABLE);
		//アルファテストしない
		RM::SetRS(RM::ALPHATEST_DISABLE);
		//ディザ有効
		RM::SetRS(RM::DITHER_ENABLE);
		//カリング無効
		RM::SetRS(RM::CULL_DISABLE);
		//法線正規化無効
		RM::SetRS(RM::NORMALIZE_DISABLE);
		//線形補完
		RM::SetRS(RM::LINEAR_ENABLE);
		//加算しない
		RM::SetRS(RM::ADD_ENABLE);
		break;
	}
}

void EDX::EDX_RenderMgr::SetRS(RS State){
	switch (State){
	case ADD_DISABLE:
		iexSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		iexSystem::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	case ADD_ENABLE:
		iexSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		iexSystem::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case ALPHA_ENABLE://アルファブレンド
		iexSystem::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		iexSystem::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		break;
	case ALPHA_DISABLE://アルファブレンド
		iexSystem::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		break;
	case DEPTHREAD_ENABLE://Zバッファに従って描画
		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
		break;
	case DEPTHREAD_DISABLE://Zバッファを無視して描画
		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);
		break;
	case DEPTHWRITE_ENABLE://Zバッファに書き込む
		iexSystem::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		break;
	case DEPTHWRITE_DISABLE://Zバッファに書き込まない
		iexSystem::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		break;
	case ALPHATEST_ENABLE://アルファテスト
		iexSystem::GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		iexSystem::GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		iexSystem::GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0xfe);
		break;
	case ALPHATEST_DISABLE://アルファテストなし
		iexSystem::GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		break;
	case DITHER_ENABLE://ディザ
		iexSystem::GetDevice()->SetRenderState(D3DRS_DITHERENABLE, TRUE);
		break;
	case DITHER_DISABLE://ディザなし
		iexSystem::GetDevice()->SetRenderState(D3DRS_DITHERENABLE, FALSE);
		break;
	case CULL_ENABLE://カリング
		iexSystem::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		break;
	case CULL_DISABLE://カリングなし
		iexSystem::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		break;
	case NORMALIZE_ENABLE://法線正規化
		iexSystem::GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		break;
	case NORMALIZE_DISABLE://法線正規化なし
		iexSystem::GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
		break;
	case LINEAR_ENABLE://線形補完
		iexSystem::GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		iexSystem::GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		break;
	case POINT_ENABLE://点補完
		iexSystem::GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		iexSystem::GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		break;
	}
}