#include "../IEX/iextreme.h"
#include "EDXLIB.h"

//********************************************
//機能概要：Float型の誤差を考慮し比較する
//引数：float a , b 比較対象
//戻り値：同じ場合true
//********************************************
bool EDX::FloatCmp(float a, float b){
	if (a > b - FLT_EPSILON || a<b + FLT_EPSILON)
	{
		return true;
	}
	return false;
}

//********************************************
//機能概要：ベクトルを取得する
//引数：float vx,vy,vz ベクトルの各成分
//戻り値：ベクトル
//********************************************
EDX::Vector EDX::GetV(float vx, float vy, float vz){
	Vector V = { vx, vy, vz };
	return V;
}
EDX::Vector EDX::GetV(D3DXVECTOR3 D3Vec){
	Vector V = { D3Vec.x, D3Vec.y, D3Vec.z };
	return V;
}

//********************************************
//機能概要：点と点の距離を取得する
//　　　　　またはベクトルの大きさを取得する
//引数：Vector Vec1,Vec2 点
//戻り値：点と点の直線距離
//********************************************
float EDX::Length(Vector Vec1, Vector Vec2){

	Vector sub = Vec2 - Vec1;

	return sqrtf(sub.x*sub.x + sub.y*sub.y + sub.z*sub.z);
}

float EDX::Length(Vector Vec){

	Vector sub = Vec;

	return sqrtf(sub.x*sub.x + sub.y*sub.y + sub.z*sub.z);
}

//********************************************
//機能概要：ベクトルの正規化を行う
//引数：D3DVECTOR Vector 正規化を行うベクトル
//戻り値：正規化したベクトル
//********************************************
EDX::Vector EDX::Normalize(Vector Vec){
	float r = sqrtf(Vec.x*Vec.x + Vec.y*Vec.y + Vec.z*Vec.z);
	Vec.x /= r;
	Vec.y /= r;
	Vec.z /= r;
	return Vec;
}

D3DXVECTOR3 EDX::Normalize(D3DXVECTOR3 Vec){
	float r = sqrtf(Vec.x*Vec.x + Vec.y*Vec.y + Vec.z*Vec.z);
	Vec.x /= r;
	Vec.y /= r;
	Vec.z /= r;
	return Vec;
}

//********************************************
//機能概要：ベクトル同士の外積を取得する
//引数：Vector v1 ベクトル1
//Vector v2 ベクトル2
//戻り値：外積値
//********************************************
EDX::Vector EDX::CrossVec(Vector v1, Vector v2){
	Vector Out;
	Out.x = v1.y*v2.z - v1.z*v2.y;
	Out.y = v1.z*v2.x - v1.x*v2.z;
	Out.z = v1.x*v2.y - v1.y*v2.x;
	return Out;
}

//********************************************
//機能概要：ベクトル同士の内積を取得する
//引数：Vector v1 ベクトル1
//Vector v2 ベクトル2
//戻り値：内積値
//********************************************
float EDX::DotVec(Vector v1, Vector v2){
	float Out = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	return Out;
}

//********************************************
//機能概要：クォータニオン同士の乗算を行う
//引数：Quaternion Q1 クォータニオン1
//Quaternion Q2 クォータニオン2
//戻り値：乗算の結果
//********************************************
EDX::Quaternion EDX::MulQuater(Quaternion Q1, Quaternion Q2){
	Quaternion Out;
	Out.w = Q1.w*Q2.w - DotVec(Q1.xyz, Q2.xyz);
	Out.xyz = Q2.xyz*Q1.w + Q1.xyz*Q2.w + CrossVec(Q1.xyz, Q2.xyz);
	
	return Out;
}

//********************************************
//機能概要：クォータニオンを用いた任意軸回転を行う
//引数：D3DVECTOR Pos 回転したい座標
//D3DVECTOR Axis 任意軸ベクトル
//float RotateAngle 回転する角度(ラジアン角)
//戻り値：回転後の座標
//********************************************
EDX::Vector EDX::FreeAxisRotate(Vector Pos, Vector Axis, float RotateAngle){
	Quaternion Out;
	Out.xyz = Pos;
	Out.w = 0;

	if (RotateAngle > EDX_PI_F)RotateAngle -= EDX_PI_F*2.0f;
	if (RotateAngle < EDX_PI_F)RotateAngle += EDX_PI_F*2.0f;

	Axis = Normalize(Axis);

	Quaternion Q;
	Q.w = cosf(RotateAngle*0.5f);
	Q.x = Axis.x*sinf(RotateAngle * 0.5f);
	Q.y = Axis.y*sinf(RotateAngle * 0.5f);
	Q.z = Axis.z*sinf(RotateAngle * 0.5f);

	Quaternion R;
	R.w = cosf(RotateAngle*0.5f);
	R.x = -Axis.x*sinf(RotateAngle * 0.5f);
	R.y = -Axis.y*sinf(RotateAngle * 0.5f);
	R.z = -Axis.z*sinf(RotateAngle * 0.5f);

	Out = MulQuater(MulQuater(R, Out), Q);
	return Out.xyz;
}

EDX::Vector EDX::FreeAxisRotate(Vector Pos, Vector Offset, Vector Axis, float RotateAngle){
	Quaternion Out;
	Out.xyz = Pos - Offset;
	Out.w = 0;

	if (RotateAngle > EDX_PI_F)RotateAngle -= EDX_PI_F*2.0f;
	if (RotateAngle < EDX_PI_F)RotateAngle += EDX_PI_F*2.0f;

	Axis = Normalize(Axis);

	Quaternion Q;
	Q.w = cosf(RotateAngle*0.5f);
	Q.x = Axis.x*sinf(RotateAngle * 0.5f);
	Q.y = Axis.y*sinf(RotateAngle * 0.5f);
	Q.z = Axis.z*sinf(RotateAngle * 0.5f);

	Quaternion R;
	R.w = cosf(RotateAngle*0.5f);
	R.x = -Axis.x*sinf(RotateAngle * 0.5f);
	R.y = -Axis.y*sinf(RotateAngle * 0.5f);
	R.z = -Axis.z*sinf(RotateAngle * 0.5f);

	Out = MulQuater(MulQuater(R, Out), Q);
	return Out.xyz + Offset;
}


//********************************************
//機能概要：D3DXヘルパー関数を用いた任意軸回転を行う
//引数：D3DVECTOR Pos 回転したい座標
//D3DVECTOR Axis 任意軸ベクトル
//float RotateAngle 回転する角度(ラジアン角)
//戻り値：回転後の座標
//********************************************
D3DXMATRIX EDX::FreeAxisRotateDX(Vector Pos, Vector Axis, float RotateAngle){
	D3DXMATRIX mat;
	D3DXVECTOR3 buff = { Axis.x, Axis.y, Axis.z};
	D3DXMatrixRotationAxis(&mat, &buff, RotateAngle);
	mat = mat;
	return mat;
}


//********************************************
//機能概要：ベクトルと行列の掛け算を行う
//引数：Vector v ベクトル
//D3DXMATRIX mat 行列
//戻り値：計算後のベクトル
//********************************************
EDX::Vector EDX::MulVM(Vector v, D3DXMATRIX mat) {
	Vector Out;
	Out.x = mat._11*v.x + mat._12*v.y + mat._13*v.z + mat._41*1.0f;
	Out.y = mat._21*v.x + mat._22*v.y + mat._23*v.z + mat._42*1.0f;
	Out.z = mat._31*v.x + mat._32*v.y + mat._33*v.z + mat._43*1.0f;
	return Out;
}
EDX::Vector EDX::MulVM(D3DXVECTOR3 v, D3DXMATRIX mat) {
	Vector Out;
	Out.x = mat._11*v.x + mat._12*v.y + mat._13*v.z + mat._14*1.0f;
	Out.y = mat._21*v.x + mat._22*v.y + mat._23*v.z + mat._24*1.0f;
	Out.z = mat._31*v.x + mat._32*v.y + mat._33*v.z + mat._34*1.0f;
	return Out;
}
//
////********************************************
////機能概要：変換行列を用いて3D空間上の座標が
////			スクリーン上でどの点に存在するか調べる
////引数：Vector WPos ワールド空間上での座標
////戻り値：スクリーン上での座標
////********************************************
//Vector GetScreenPos(Vector WPos){
//	D3DXMATRIX View, Persp;
//	D3DXVECTOR4 Pos = { WPos.x, WPos.y, WPos.z, 1 };
//
//	float Width = (float)edxSystem::GetRect().right;
//	float Height = (float)edxSystem::GetRect().bottom;
//
//	D3DXVec4Transform(&Pos, &Pos, &EDX_Camera::GetVPMat());
//
//
//	Vector Out = { 0, 0, 0 };
//	Out.x = 0.5f*Pos.x / Pos.w + 0.5f;
//	Out.y = 0.5f*Pos.y / Pos.w + 0.5f;
//	Out.y = 1.0f - Out.y;
//	Out.x *= Width;
//	Out.y *= Height;
//	Out.z = Pos.z / Pos.w;
//	if (Out.z >= 1.0f){
//		Out.x *= (float)edxSystem::GetRect().right;
//		Out.y *= (float)edxSystem::GetRect().bottom;
//	}
//	return Out;
//}
