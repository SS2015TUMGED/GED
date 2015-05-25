//--------------------------------------------------------------------------------------
// Shader resources
//--------------------------------------------------------------------------------------

Buffer<float>	g_HeightMap; // Buffer for the heightmap	
Texture2D		g_NormalMap;


Texture2D		g_Diffuse; // Material albedo for diffuse lighting


//--------------------------------------------------------------------------------------
// Constant buffers
//--------------------------------------------------------------------------------------

cbuffer cbConstant
{
    float4  g_LightDir; // Object space
	int g_TerrainRes;
};

cbuffer cbChangesEveryFrame
{

	matrix g_WorldNormals;

    matrix  g_World;
    matrix  g_WorldViewProjection;
    float   g_Time;
};

cbuffer cbUserChanges
{
};


//--------------------------------------------------------------------------------------
// Structs
//--------------------------------------------------------------------------------------

struct PosNorTex
{
    float4 Pos : SV_POSITION;
    float4 Nor : NORMAL;
    float2 Tex : TEXCOORD;
};

struct PosTexLi
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
    float   Li : LIGHT_INTENSITY;
	float3 normal: NORMAL;
};
struct PosTex
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
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
// Shaders
//--------------------------------------------------------------------------------------

PosTexLi SimpleVS(PosNorTex Input) {
    PosTexLi output = (PosTexLi) 0;

    // Transform position from object space to homogenious clip space
    output.Pos = mul(Input.Pos, g_WorldViewProjection);

    // Pass trough normal and texture coordinates
    output.Tex = Input.Tex;

    // Calculate light intensity
    output.normal = normalize(mul(Input.Nor, g_World).xyz); // Assume orthogonal world matrix
    output.Li = saturate(dot(output.normal, g_LightDir.xyz));
        
    return output;
}

float4 SimplePS(PosTexLi Input) : SV_Target0 {
    // Perform lighting in object space, so that we can use the input normal "as it is"
    //float4 matDiffuse = g_Diffuse.Sample(samAnisotropic, Input.Tex);
    float4 matDiffuse = g_Diffuse.Sample(samLinearClamp, Input.Tex);
    return float4(matDiffuse.rgb * Input.Li, 1);
	//return float4(Input.normal, 1);
}


// assignment 05
PosTex TerrainVS(uint VertexID : SV_VertexID)  {
	PosTex output = (PosTex)0;

	// put the coordinates into the struct PosTex

	// int % resolution = x
	int x = (VertexID % g_TerrainRes);
	// int / resolution = z
	int z = (VertexID / g_TerrainRes);

	output.Pos.x = (float)(x - g_TerrainRes / 2);
	output.Pos.y = g_HeightMap[VertexID];
	output.Pos.z = (float)(z - g_TerrainRes / 2);
	output.Pos.w = 1.0f;

	output.Pos = mul(output.Pos, g_WorldViewProjection);

	// calculate textures
	output.Tex.x = (float)x / g_TerrainRes;
	output.Tex.y = (float)z / g_TerrainRes;

	return output;
}


	float4 TerrainPS(PosTex Input) : SV_Target0{

		float3 n;
		float4 matNormal = g_NormalMap.Sample(samAnisotropic, Input.Tex);

			// apply the x,z coordinates
			//n = (float4(matDiffuse.xz)  * 2).xz - 1;

		n.x = matNormal.x * 2 - 1;
		n.z = matNormal.z * 2 - 1;

		// calc the height y , length of the vector is 1
		//					length	= sqrt (x*x + y*y + z*z)
		// ->					1	= x*x + y*y + z*z
		// ->		1 - x*x - z*z	= y*y
		// -> sqrt(1 - x*x - z*z)	= y
		n.y = 1 - (n.x * n.x) - (n.z * n.z) ;
		n.y = sqrt(n.y);


		// transform and normalize 
		n = normalize(mul(n, g_World).xyz); // Assume orthogonal world matrix


		float3 matDiffuse = g_Diffuse.Sample(samAnisotropic, Input.Tex);
		
		float i = saturate(dot(n, g_LightDir.xyz));

		return float4(matDiffuse.rgb * i, 1);

	}



//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique11 Render
{
    pass P0
    {
		SetVertexShader(CompileShader(vs_4_0, TerrainVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, TerrainPS()));
        
        SetRasterizerState(rsCullNone);
        SetDepthStencilState(EnableDepth, 0);
        SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
    }
}
