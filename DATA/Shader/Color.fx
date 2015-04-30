//**************************************************************************************************
//
//		色関連シェーダー
//
//**************************************************************************************************

float3 colorRate;
float4 ColorRate(float4 color)
{
	float gray = (color.r + color.g + color.b)/3.0;
	float3 diff;
	diff = color.rgb - gray;
	color.rgb = gray;
	color.rgb += diff*colorRate;

	return color;
}

float4 PS_UseColorRate(VS_BASIC In)	: COLOR
{
	float4 Out;

	Out = tex2D( DecaleSamp, In.Tex );
	Out.a *= In.Color.a;
	Out = ColorRate(Out);

	return Out;
}

technique useColorRate
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		ZWriteEnable     = true;
		CullMode         = CCW;

		VertexShader = compile vs_3_0 VS_Basic2();
		PixelShader  = compile ps_3_0 PS_UseColorRate();
    }
}

float4 PS_UseColorRateParticle(VS_BASIC In)	: COLOR
{
	float4 Out;

	Out = tex2D( DecaleSamp, In.Tex );
	Out *= In.Color;
	Out = ColorRate(Out);

	return Out;
}

technique colorParticle
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		ZWriteEnable     = false;

		VertexShader = compile vs_3_0 VS_Basic2();
		PixelShader  = compile ps_3_0 PS_UseColorRateParticle();
    }
}


//**************************************************************************************************
//
//		スイッチシェーダー
//
//**************************************************************************************************

float3 switchColor;

struct VS_SWITCH{
	float4 Pos    : POSITION;
};

VS_SWITCH VS_Switch( VS_INPUTL In )
{
	VS_SWITCH Out = (VS_SWITCH)0;
	Out.Pos = mul(In.Pos, Projection);
	return Out;
}

float4 PS_Switch(VS_SWITCH In)	: COLOR
{
	float4 Out;

	Out.rgb = switchColor;
	Out.a = 1;

	return Out;
}

technique button
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		ZWriteEnable     = true;

		VertexShader = compile vs_3_0 VS_Basic2();
		PixelShader  = compile ps_3_0 PS_Switch();
    }
}