//**************************************************************************************************
//
//		ÇªÇÃëºé©ï™Ç≈èëÇ¢ÇΩÇ‡ÇÃ
//
//**************************************************************************************************

technique debug
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		CullMode         = CCW;
		ZWriteEnable     = false;

		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader  = compile ps_3_0 PS_Basic();
	}
};

technique cullCW
{
	pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = One;
		CullMode         = CW;
		ZWriteEnable     = false;

		VertexShader = compile vs_3_0 VS_Basic();
		PixelShader  = compile ps_3_0 PS_Basic();
    }
};