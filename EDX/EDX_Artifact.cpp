#include "../IEX/iextreme.h"
#include "EDXLIB.h"


void EDX::DrawBox(float x1, float y1, DWORD c1, float x2, float y2, DWORD c2, float x3, float y3, DWORD c3, float x4, float y4, DWORD c4){
	EDX::AF_VERTEX2D v1[3] = {
		{ x1, y1, 0.0f, 1.0f, c1 },
		{ x2, y2, 0.0f, 1.0f, c2 },
		{ x3, y3, 0.0f, 1.0f, c3 },
	};
	EDX::AF_VERTEX2D v2[3] = {
		{ x2, y2, 0.0f, 1.0f, c2 },
		{ x4, y4, 0.0f, 1.0f, c4 },
		{ x3, y3, 0.0f, 1.0f, c3 },
	};

	EDX::RM::SetRenderType(EDX::RM::RT::TYPE_DEFAULT);
	D3DMATERIAL9 m;
	m.Ambient = { 0.7, 0.7, 0.7, 1.0 };
	m.Diffuse = { 1.0, 1.0, 1.0, 1.0 };
	m.Emissive = { 1.0, 1.0, 1.0, 1.0 };
	m.Specular = { 1.0, 1.0, 1.0, 1.0 };
	m.Power = 1.0f;
	iexSystem::GetDevice()->SetMaterial(&m);

	iexSystem::GetDevice()->SetTexture(0, NULL);
	iexSystem::GetDevice()->SetFVF(FVF_VERTEX2D);
	iexSystem::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, v1, sizeof(EDX::AF_VERTEX2D));
	iexSystem::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, v2, sizeof(EDX::AF_VERTEX2D));
}