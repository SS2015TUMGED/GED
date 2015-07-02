//--------------------------------------------------------------------------------------
// Rasterizer states
//--------------------------------------------------------------------------------------

RasterizerState rsDefault {
};

RasterizerState rsCullFront {
	CullMode = Front;
};

RasterizerState rsCullBack {
	CullMode = Back;
};

RasterizerState rsCullNone {
	CullMode = None;
};

RasterizerState rsLineAA {
	CullMode = None;
	AntialiasedLineEnable = true;
};

//--------------------------------------------------------------------------------------
// DepthStates
//--------------------------------------------------------------------------------------
DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
	DepthFunc = LESS_EQUAL;
};

BlendState NoBlending
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = FALSE;
};

//--------------------------------------------------------------------------------------
// Structs
//--------------------------------------------------------------------------------------
struct SpriteVertex
{
	float3 pos : POSITION;     // world-space position (sprite center)
	float  rad : RADIUS;       // world-space radius (= half side length of the sprite quad)
	int    ind : INDEX;        // which texture to use (out of SpriteRenderer::m_spriteSRV)
	//float  t : STATE;
	//float alpha : ALPHA;
};

// Dummy methods
void DummyVS(SpriteVertex input, out float4 pos : SV_Position) {
	pos = float4(0, 0, 0.5, 1);
}

float4 DummyPS(float4 pos : SV_Position) : SV_Target0{
	return float4(1, 1, 0, 1);
}

//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique11 sRender
{
    pass P0
    {
		SetVertexShader(CompileShader(vs_4_0, DummyVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, DummyPS()));
        
        SetRasterizerState(rsCullNone);
        SetDepthStencilState(EnableDepth, 0);
        SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
    }
}
