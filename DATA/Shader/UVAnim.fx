//**************************************************************************************************
//
//		ＵＶアニメーション用
//
//**************************************************************************************************

float	uMove;
float	vMove;

//------------------------------------------------------
//	頂点シェーダー
//------------------------------------------------------
VS_BASIC VS_UvAniUV( VS_INPUTL In )
{
	VS_BASIC Out = (VS_BASIC)0;

	float3 P = mul(In.Pos, TransMatrix);

	float3x3	mat = TransMatrix;
	float3 N = mul(In.Normal, mat);
	N = normalize(N);

	Out.Pos   = mul(In.Pos, Projection);
	Out.Color.rgb = DirLight( LightDir, N ) + HemiLight( N );
	Out.Color.a = 1.0f;
	Out.Tex = In.Tex + float2(uMove, vMove);

	return Out;
}

technique animUV
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		ZWriteEnable     = true;

		VertexShader = compile vs_3_0 VS_UvAniUV();
		PixelShader  = compile ps_3_0 PS_UseColorRate();
    }
};


//------------------------------------------------------
//	残り敵数表示オブジェクト用シェーダ
//------------------------------------------------------
float remainAlpha = 1.0f;

VS_BASIC VS_RemainObj( VS_INPUTL In )
{
	VS_BASIC Out = (VS_BASIC)0;

	float3 P = mul(In.Pos, TransMatrix);

	float3x3	mat = TransMatrix;
	float3 N = mul(In.Normal, mat);
	N = normalize(N);

	Out.Pos   = mul(In.Pos, Projection);
	Out.Color.rgb = DirLight( LightDir, N ) + HemiLight( N );
	Out.Color.a = remainAlpha;
	Out.Tex = In.Tex + float2(uMove, vMove);

	return Out;
}

technique remainUV
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		ZWriteEnable     = true;

		VertexShader = compile vs_3_0 VS_RemainObj();
		PixelShader  = compile ps_3_0 PS_UseColorRate();
    }
};