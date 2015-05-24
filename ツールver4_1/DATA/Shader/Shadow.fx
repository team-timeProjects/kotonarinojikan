//******************************************************************************
//
//		シャドウマップ作成
//
//******************************************************************************
float4x4 ShadowProjection;

texture ShadowMap;
sampler ShadowSamp = sampler_state
{
	Texture = <ShadowMap>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;

	BorderColor = 0xFFFFFFFF;
	AddressU = BORDER;
	AddressV = BORDER;
};

struct VS_SHADOW
{
	float4 Pos		: POSITION;
	float4 Color	: TEXCOORD0;		// 頂点色
};

//------------------------------------------------------
//		頂点シェーダー
//------------------------------------------------------
VS_SHADOW VS_ShadowBuf(float4 Pos : POSITION)
{
	VS_SHADOW Out;

	// 座標変換
	float4x4	mat = mul(TransMatrix, ShadowProjection);
	Out.Pos = mul(Pos, mat);
	Out.Color = Out.Pos.z;
	return Out;
}

//------------------------------------------------------
//		ピクセルシェーダー
//------------------------------------------------------
float4 PS_ShadowBuf(VS_SHADOW In) : COLOR
{
	return	In.Color;
}

//------------------------------------------------------
//		テクニック
//------------------------------------------------------
technique ShadowBuf
{
	pass Pass0
	{
		AlphaBlendEnable	= false;
		ZWriteEnable		= true;
		CullMode 			= NONE;

		VertexShader = compile vs_2_0 VS_ShadowBuf();
		PixelShader  = compile ps_2_0 PS_ShadowBuf();
	}
}

//******************************************************************************
//
//		シャドウマップ採用
//
//******************************************************************************
struct VS_STAGE_SHADOW
{
    float4 Pos		: POSITION;
    float4 Color	: COLOR0;
    float2 Tex		: TEXCOORD0;
	float3 vShadow	: TEXCOORD1;
};

float AdjustValue = -0.004f;
float Shadow = 0.4f;

inline float3 GetShadowTex(float3 Pos)
{
	float3 Tex;
	//テクスチャ座標計算
	float4 ppp;
	ppp.xyz = Pos;
	ppp.w = 1;
	Tex = mul(ppp, ShadowProjection);

	Tex.y = -Tex.y;
	Tex.xy = 0.5f*Tex.xy + 0.5f;
	
	return Tex;
}

inline float GetShadow(float3 Tex)
{
	float d = tex2D(ShadowSamp, Tex.xy).r;//シャドウマップから深度を取り出す
	if(d < Tex.z+AdjustValue){
		return Shadow;
	}
	return 1;
}



VS_STAGE_SHADOW VS_StageShadow( VS_INPUTL In )
{
    VS_STAGE_SHADOW Out = (VS_STAGE_SHADOW)0;
	
	float3 P = mul( In.Pos, TransMatrix );

	float3x3	mat = TransMatrix;
	float3 N = mul(In.Normal, mat);
	N = normalize(N);

	Out.Pos   = mul(In.Pos, Projection);
	Out.Color.rgb = (DirLight( LightDir, N ) + HemiLight( N ));
	Out.Color.a = In.Color.a;
	Out.Tex   = In.Tex;

	//シャドウマップ
	Out.vShadow = GetShadowTex(In.Pos);
	

    return Out;
}


float4 PS_StageShadow(VS_STAGE_SHADOW In)	: COLOR
{
	float4 Out;

	Out = tex2D( DecaleSamp, In.Tex );
	Out.a *= In.Color.a;
	Out = ColorRate(Out);

//	シャドウマップ適用
	Out.rgb *= GetShadow(In.vShadow);

	return Out;
}


technique stageUseShadow
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		ZWriteEnable     = true;

		VertexShader = compile vs_3_0 VS_StageShadow();
		PixelShader  = compile ps_3_0 PS_StageShadow();
	}
}