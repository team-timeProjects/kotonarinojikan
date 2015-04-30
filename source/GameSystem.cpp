
//	システム
#include	"iextreme.h"
#include	"system/system.h"

//	シーン
#include	"sceneMain.h"

#include	"GameSystem.h"

//*****************************************************************************************************
//
//	GameSystemクラス
//
//*****************************************************************************************************

//----------------------------------------------------------------------------
//	座標変換
//----------------------------------------------------------------------------

	//	スクリーン座標からワールド座標に変換１
	Vector3	GameSystem::TransCoordScreenToWorld( const Vector3& _ScreenCoord )
	{
		D3DXVECTOR3		d3dxScreenCoord(_ScreenCoord.x, _ScreenCoord.y, _ScreenCoord.z);

		D3DVIEWPORT9	viewPort;
		iexSystem::Device->GetViewport(&viewPort);

		D3DXMATRIX		worldMatrix;
		D3DXMatrixIdentity(&worldMatrix);

		D3DXVECTOR3		result;
		D3DXVec3Unproject(&result, &d3dxScreenCoord, &viewPort, &matProjection, &matView, &worldMatrix);

		return	Vector3(result.x, result.y, result.z);
	}

	//	スクリーン座標からワールド座標に変換２
	Vector3	GameSystem::TransCoordScreenToWorld( const Vector3& _ScreenCoord, const D3DVIEWPORT9* _pViewPort, const Matrix* _pProjectionMatrix, const Matrix* _pViewMatrix )
	{
		D3DXVECTOR3		d3dxScreenCoord(_ScreenCoord.x, _ScreenCoord.y, _ScreenCoord.z);

		D3DXMATRIX		worldMatrix;
		D3DXMatrixIdentity(&worldMatrix);

		D3DXVECTOR3		result;
		D3DXVec3Unproject(&result, &d3dxScreenCoord, _pViewPort, _pProjectionMatrix, _pViewMatrix, &worldMatrix);

		return	Vector3(result.x, result.y, result.z);
	}
	
	//	ワールド座標からスクリーン座標に変換
	bool	GameSystem::WorldToClient( const Vector3& pos, Vector3& out, const Matrix& mat )
	{
		//	頂点×行列で射影する
		Vector3	temp;
		temp.x = pos.x *mat._11 + pos.y * mat._21 + pos.z * mat._31 + 1.0f * mat._41;
		temp.y = pos.x *mat._12 + pos.y * mat._22 + pos.z * mat._32 + 1.0f * mat._42;
		temp.z = pos.x *mat._13 + pos.y * mat._23 + pos.z * mat._33 + 1.0f * mat._43;
		float w = pos.x *mat._14 + pos.y * mat._24 + pos.z * mat._34 + 1.0f * mat._44;
		temp.x /= w;
		temp.y /= w;
		temp.z /= w;

		//	画面座標に換算(ビューポート変換)
		long WinWide, WinHeight;
		WinWide = iexSystem::ScreenWidth;
		WinWide /= 2;
		WinHeight = iexSystem::ScreenHeight;
		WinHeight /= 2;

		out.x = ( temp.x *WinWide + WinWide );
		out.y = ( -temp.y*WinHeight + WinHeight );
		out.z = temp.z;

		if ( temp.x >  1.0f ) return false;
		if ( temp.x < -1.0f ) return false;
		if ( temp.y >  1.0f ) return false;
		if ( temp.y < -1.0f ) return false;
		if ( temp.z <  0 )	return false;
		return true;
	}

//----------------------------------------------------------------------------
//	当たり判定用関数
//----------------------------------------------------------------------------

	//	カプセルとカプセルの当たり判定
	bool	GameSystem::Capsule( const Vector3& Pos1, const Vector3& Pos2, float r1, const Vector3& Pos3, const Vector3& Pos4, float r2 )
	{
		Vector3 v3 = Pos3 - Pos1;
		Vector3 d0(Pos2 - Pos1);
		Vector3 d1(Pos4 - Pos3);
		float Length0 = d0.Length(); d0.Normalize();
		float Length1 = d1.Length(); d1.Normalize();
		Vector3 Cro_v1_v1; Vector3Cross(Cro_v1_v1, d0, d1);
		Vector3 Cro_v_v1;  Vector3Cross(Cro_v_v1, v3, d0);
		Vector3 Cro_v_v2;  Vector3Cross(Cro_v_v2, v3, d1);
		float t0 = Vector3Dot(Cro_v1_v1, Cro_v_v2) / (Cro_v1_v1.Length() * Cro_v1_v1.Length());
		float t1 = Vector3Dot(Cro_v1_v1, Cro_v_v1) / (Cro_v1_v1.Length() * Cro_v1_v1.Length());
		if (t0 > Length0){ t0 = Length0; }
		else if (t0 < 0.0f){ t0 = 0.0f; }
		if (t1 > Length1){ t1 = Length1; }
		else if (t1 < 0.0f){ t1 = 0.0f; }
		Vector3 q0 = Pos1 + (d0 * t0);
		Vector3 q1 = Pos3 + (d1 * t1);
		Vector3 q2(q0 - q1);
		float l2 = q2.Length();
		//長さがお互いの半径を足した数より小さければtrueを返す
		if (l2 <= (r1 + r2)){ return true; }
		return false;
	}

	//	カプセルとカプセルの当たり判定2
	bool	GameSystem::Capsule2( const Vector3& Pos1, const Vector3& Pos2, float r1, const Vector3& Pos3, const Vector3& Pos4, float r2 )
	{
		Vector3 d0(Pos2 - Pos1);
		Vector3 d1(Pos4 - Pos3);
		Vector3 v = Pos1 - Pos3;

		float a = Vector3Dot(d0, d0);
		float b = Vector3Dot(d0, d1);
		float c = Vector3Dot(d0, v);
		float e = Vector3Dot(d1, d1);
		float f = Vector3Dot(d1, v);

		float d = a * e - b * b;

		float t0 = (b * f - c * e) / d;
		float t1 = (a * f - b * c) / d;
		if ((t0 < 0.0f || t0 > 1.0f) && (t1 < 0.0f || t1 > 1.0f)){
			t0 = (t0 < 0.0f) ? 0.0f : 1.0f;
			t1 = (t1 < 0.0f) ? 0.0f : 1.0f;
		}
		else if (t0 < 0.0f || t0 > 1.0f){
			t0 = (t0 < 0.0f) ? 0.0f : 1.0f;
			t1 = Vector3Dot((Pos1 + (d0 * t0) - Pos1), d1) / Vector3Dot(d1, d1);
		}
		else if (t1 < 0.0f || t1 > 1.0f){
			t1 = (t1 < 0.0f) ? 0.0f : 1.0f;
			t0 = Vector3Dot((Pos3 + (d1 * t1) - Pos3), d0) / Vector3Dot(d0, d0);
		}
		Vector3 p0 = Pos1 + (d0 * t0);
		Vector3 p1 = Pos3 + (d1 * t1);
		Vector3 p2(p1 - p0);
		//長さがお互いの半径を足した数より小さければtrueを返す
		if (p2.Length() <= (r1 + r2)){ return true; }
		return false;
	}
	
	//	カプセルと球の当たり判定 ( カプセルの上、カプセルの下、カプセル半径、球の中心、球の半径 )
	bool	GameSystem::CapsuleVsBall(const Vector3& Pos1, const Vector3& Pos2, float r1, const Vector3& Ball, float r2 )
	{
		Vector3 v0(Pos2 - Pos1);
		Vector3 v1(Ball - Pos1);	
		Vector3 v2 = v0;
		v1.Normalize();
		v2.Normalize();
		float Dot = Vector3Dot(v2, v1);
		if (Dot <= 0){ Dot = 0; }
		Vector3 q = Pos1 + ( v0 * Dot );
		Vector3 Fit( q - Ball );
		if ( Fit.Length() <= ( r1 + r2 ) ){ return true; }
		return false;
	}

	//	球同士の当たり判定
	bool	GameSystem::Ball( const Vector3& Pos1, float r1, const Vector3& Pos2, float r2 )
	{
		Vector3 v(Pos2 - Pos1);
		if (v.Length() <= (r1 + r2)){ return true; }
		return false;
	}

//----------------------------------------------------------------------------
//	描画関数
//----------------------------------------------------------------------------
	void	GameSystem::DrawLine( const Vector3& Pos1, const Vector3& Pos2 )
	{
		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		DrawLine( iexSystem::GetDevice(), D3DXVECTOR3(Pos1.x, Pos1.y, Pos1.z), D3DXVECTOR3(Pos2.x, Pos2.y, Pos2.z), 0xFF000000);
		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	}
	void	GameSystem::DrawSphere( const Vector3& Pos, float r1 )
	{
		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		DrawSphere(iexSystem::GetDevice(), D3DXVECTOR3(Pos.x, Pos.y, Pos.z), r1, 0xFF000000);
		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	}
	void	GameSystem::DrawCapsule(const Vector3& Pos1, const Vector3& Pos2, float r1 )
	{
		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		DrawCapsule(iexSystem::GetDevice(), D3DXVECTOR3(Pos1.x, Pos1.y, Pos1.z), D3DXVECTOR3(Pos2.x, Pos2.y, Pos2.z), r1, 0xFF000000);
		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	}
	 
//----------------------------------------------------------------------------
//	線の描画(本体)
//----------------------------------------------------------------------------
	void	GameSystem::DrawLine( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p0, CONST D3DXVECTOR3 &p1, D3DCOLOR c )
	{
		static _VB vb( d3dd, sizeof( D3DXVECTOR3 )* 2 );

		D3DXVECTOR3 *p;
		vb->Lock(0, 0, (void **)&p, 0);
		p[0] = p0;
		p[1] = p1;
		vb->Unlock();

		D3DMATERIAL9 m = { 0 };
		m.Diffuse = m.Ambient = D3DXCOLOR(c);
		d3dd->SetMaterial(&m);

		d3dd->SetFVF(D3DFVF_XYZ);
		d3dd->SetStreamSource(0, vb, 0, sizeof(D3DXVECTOR3));
		d3dd->SetTransform(D3DTS_WORLD, &D3DXMATRIX(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));
		d3dd->DrawPrimitive(D3DPT_LINELIST, 0, 1);
	}

//----------------------------------------------------------------------------
//	球の描画(本体)
//----------------------------------------------------------------------------
	void	GameSystem::DrawSphere( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p0, FLOAT r, D3DCOLOR c )
	{
		CONST INT N = 36;

		static _VB vb(d3dd, sizeof(D3DXVECTOR3)* (N + 1));
		for (int n = 0; n < 3; n++)
		{

			D3DXVECTOR3 *p;
			vb->Lock(0, 0, (void **)&p, 0);
			for (int i = 0; i < N; i++)
			{
				switch (n)
				{
				case 0: p[i] = r * D3DXVECTOR3(sinf(360.0f / N * 0.01745f * i), cosf(360.0f / N * 0.01745f * i), 0) + p0; break;
				case 1: p[i] = r * D3DXVECTOR3(sinf(360.0f / N * 0.01745f * i), 0, cosf(360.0f / N * 0.01745f * i)) + p0; break;
				case 2: p[i] = r * D3DXVECTOR3(0, sinf(360.0f / N * 0.01745f * i), cosf(360.0f / N * 0.01745f * i)) + p0; break;
				}
			}
			p[N] = p[0];
			vb->Unlock();

			D3DMATERIAL9 m = { 0 };
			m.Diffuse = m.Ambient = D3DXCOLOR(c);
			d3dd->SetMaterial(&m);

			d3dd->SetFVF(D3DFVF_XYZ);
			d3dd->SetStreamSource(0, vb, 0, sizeof(D3DXVECTOR3));
			d3dd->SetTransform(D3DTS_WORLD, &D3DXMATRIX(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));
			d3dd->DrawPrimitive(D3DPT_LINESTRIP, 0, N);
		}
	}

//----------------------------------------------------------------------------
//	カプセルの描画(本体)
//----------------------------------------------------------------------------
	void	GameSystem::DrawCapsule( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p0, CONST D3DXVECTOR3 &p1, FLOAT r, D3DCOLOR c )
	{
		CONST INT N = 36;
		CONST INT L0 = 0;
		CONST INT L1 = N + 1;
		CONST INT L2 = L1 + N + 1;

		CONST INT L3 = L2 + N * 2;
		CONST INT L4 = L3 + N + 1;
		CONST INT L5 = L4 + N + 1;
		CONST INT L6 = L5 + N + 1;

		static _VB vb(d3dd, sizeof(D3DXVECTOR3)* ((N + 1) * 2 + N * 2 + (N + 1) * 4));
		D3DXVECTOR3 *p;
		vb->Lock(0, 0, (void **)&p, 0);
		D3DXVECTOR3 up = p0 - p1;
		CONST FLOAT L = D3DXVec3Length(&up);
		for (int i = 0; i < N; i++)
		{
			p[L0 + i] = D3DXVECTOR3(r * sinf(360.0f / N * 0.01745f * i), L / 2.0f, r * cosf(360.0f / N * 0.01745f * i));
			p[L1 + i] = D3DXVECTOR3(r * sinf(360.0f / N * 0.01745f * i), -L / 2.0f, r * cosf(360.0f / N * 0.01745f * i));
			if (i % (N / 4) == 0)
			{
				p[L2 + 2 * i] = p[L0 + i];
				p[L2 + 2 * i + 1] = p[L1 + i];
			}
			p[L3 + i] = D3DXVECTOR3(r * cosf(180.0f / N * 0.01745f * i), r * sinf(180.0f / N * 0.01745f * i) + L / 2.0f, 0);
			p[L4 + i] = D3DXVECTOR3(0, r * sinf(180.0f / N * 0.01745f * i) + L / 2.0f, r * cosf(180.0f / N * 0.01745f * i));
			p[L5 + i] = D3DXVECTOR3(r * cosf(180.0f / N * 0.01745f * i), -r * sinf(180.0f / N * 0.01745f * i) - L / 2.0f, 0);
			p[L6 + i] = D3DXVECTOR3(0, -r * sinf(180.0f / N * 0.01745f * i) - L / 2.0f, r * cosf(180.0f / N * 0.01745f * i));
		}
		p[L0 + N] = p[L0];
		p[L1 + N] = p[L1];
		p[L3 + N] = D3DXVECTOR3(-r, L / 2.0f, 0);
		p[L4 + N] = D3DXVECTOR3(0, L / 2.0f, -r);
		p[L5 + N] = D3DXVECTOR3(-r, L / 2.0f, 0);
		p[L6 + N] = D3DXVECTOR3(0, L / 2.0f, -r);

		vb->Unlock();

		D3DXVECTOR3 axis;
		D3DXVec3Cross(&axis, &D3DXVECTOR3(0, 1, 0), &up);
		D3DXVec3Normalize(&axis, &axis);
		FLOAT angle = acosf(D3DXVec3Dot(&up, &D3DXVECTOR3(0, 1, 0)) / D3DXVec3Length(&up));
		D3DXMATRIX M;
		D3DXMatrixRotationAxis(&M, &axis, angle);
		D3DXVECTOR3 g = (p0 + p1) / 2;
		M._41 = g.x;
		M._42 = g.y;
		M._43 = g.z;

		D3DMATERIAL9 m = { 0 };
		m.Diffuse = m.Ambient = D3DXCOLOR(c);
		d3dd->SetMaterial(&m);

		d3dd->SetFVF(D3DFVF_XYZ);
		d3dd->SetStreamSource(0, vb, 0, sizeof(D3DXVECTOR3));
		d3dd->SetTransform(D3DTS_WORLD, &M);
		d3dd->DrawPrimitive(D3DPT_LINESTRIP, L0, N);
		d3dd->DrawPrimitive(D3DPT_LINESTRIP, L1, N);
		d3dd->DrawPrimitive(D3DPT_LINELIST, L2, N);
		d3dd->DrawPrimitive(D3DPT_LINESTRIP, L3, N);
		d3dd->DrawPrimitive(D3DPT_LINESTRIP, L4, N);
		d3dd->DrawPrimitive(D3DPT_LINESTRIP, L5, N);
		d3dd->DrawPrimitive(D3DPT_LINESTRIP, L6, N);
	}

//-----------------------------------------------------------------------------------
//	ベジェ曲線(適当)( 出力, 始点, 操作点, 終点, 割合 )
//-----------------------------------------------------------------------------------
	void	GameSystem::BezierCurveZ( Vector3& out, Vector3& p1, Vector3& p2, Vector3& p3, float t )
	{
		out.x = ((1 - t) * (1 - t)) * p1.x + ((2 * (1 - t) * t * p2.x) + ((t * t) * p3.x));
		out.y = ((1 - t) * (1 - t)) * p1.y + ((2 * (1 - t) * t * p2.y) + ((t * t) * p3.y));
		out.z = ((1 - t) * (1 - t)) * p1.z + ((2 * (1 - t) * t * p2.z) + ((t * t) * p3.z));
		
	}
