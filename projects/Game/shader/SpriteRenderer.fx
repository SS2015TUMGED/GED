matrix g_ViewProjection;
float4 g_CamRVec;
float4 g_CamUVec;

Texture2DArray g_SprTexGatling;
Texture2DArray g_SprTexPlasma;
Texture2DArray g_SprTexBoom;

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
// Samplers
//--------------------------------------------------------------------------------------

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	AddressU = Wrap;
	AddressV = Wrap;
};

SamplerState samLinearClamp
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
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

BlendState BSBlendOver
{
	BlendEnable[0] = TRUE;
	SrcBlend[0] = SRC_ALPHA;
	SrcBlendAlpha[0] = ONE;
	DestBlend[0] = INV_SRC_ALPHA;
	DestBlendAlpha[0] = INV_SRC_ALPHA;
};

//--------------------------------------------------------------------------------------
// Structs
//--------------------------------------------------------------------------------------
struct SpriteVertex
{
	float3 pos : POSITION;     // world-space position (sprite center)
	float  rad : RADIUS;       // world-space radius (= half side length of the sprite quad)
	int    ind : INDEX;        // which texture to use (out of SpriteRenderer::m_spriteSRV)
	float  t : STATE;
	float  alpha : ALPHA;
};

struct PSVertex
{
	float4 pos : SV_POSITION;
	float  rad : RADIUS;
	int    ind : INDEX;
	float3 tex : TEXCOORD;
	float  alpha : ALPHA;
};

// Dummy methods
void DummyVS(inout SpriteVertex input) {
	//pos = float4(0, 0, 0.5, 1);
}

[maxvertexcount(4)]
void SpriteGS(point SpriteVertex vertex[1], inout TriangleStream<PSVertex> stream) {
	
	PSVertex temp = (PSVertex)0;
	temp.pos = mul(float4(vertex[0].pos, 1.0f) - (g_CamRVec * vertex[0].rad)
		+ (g_CamUVec* vertex[0].rad), g_ViewProjection);
	temp.tex = float3(0.0f, 0.0f, vertex[0].t);
	temp.ind = vertex[0].ind;
	temp.alpha = vertex[0].alpha;
	stream.Append(temp);

	temp.pos = mul(float4(vertex[0].pos, 1.0f) - (g_CamRVec * vertex[0].rad)
		- (g_CamUVec* vertex[0].rad), g_ViewProjection);
	temp.tex = float3(0.0f, 1.0f, vertex[0].t);
	stream.Append(temp);

	temp.pos = mul(float4(vertex[0].pos, 1.0f) + (g_CamRVec * vertex[0].rad)
		+ (g_CamUVec* vertex[0].rad), g_ViewProjection);
	temp.tex = float3(1.0f, 0.0f, vertex[0].t);
	stream.Append(temp);

	temp.pos = mul(float4(vertex[0].pos, 1.0f) + (g_CamRVec * vertex[0].rad)
		- (g_CamUVec* vertex[0].rad), g_ViewProjection);
	temp.tex = float3(1.0f, 1.0f, vertex[0].t);
	stream.Append(temp);
	
}


float4 DummyPS(in PSVertex input) : SV_Target0{
	float4 matDiffuse;
	if (input.ind == 0) {
		matDiffuse = g_SprTexGatling.Sample(samAnisotropic, input.tex);
	}
	else if (input.ind == 1) {
		matDiffuse = g_SprTexPlasma.Sample(samAnisotropic, input.tex);
	}
	else if (input.ind == 2) {
		matDiffuse = g_SprTexBoom.Sample(samAnisotropic, input.tex);
	}
	return matDiffuse;
}

//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique11 sRender
{
    pass P0
    {
		SetVertexShader(CompileShader(vs_4_0, DummyVS()));
        SetGeometryShader(CompileShader(gs_4_0, SpriteGS()));
        SetPixelShader(CompileShader(ps_4_0, DummyPS()));
        
        SetRasterizerState(rsCullNone);
        SetDepthStencilState(EnableDepth, 0);
        SetBlendState(BSBlendOver, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
    }
}
