//--------------------------------------------------------------------------------------
// Shader resources
//--------------------------------------------------------------------------------------

Buffer<float>	g_HeightMap; // Buffer for the heightmap	
Texture2D		g_NormalMap;


Texture2D		g_Diffuse; // Material albedo for diffuse lighting

//additional for assignment 06
Texture2D specularEV;
Texture2D glowEV;


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

	matrix  g_WorldNormals;
    matrix  g_World;
    matrix  g_WorldViewProjection;
    float   g_Time;

	//additional for assignment 06
	float4 cameraPosWorldEV;
	matrix meshPass1;
};

cbuffer cbUserChanges
{
	float3	ambient_light;
	float3	light_color;
	float	weight;
	//float	intensity;
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

//assignment 06
struct T3dVertexVSIn {
	float3 Pos : POSITION; //Position in object space 
	float2 Tex : TEXCOORD; //Texture coordinate 
	float3 Nor : NORMAL; //Normal in object space 
	float3 Tan : TANGENT; //Tangent in object space (not used in Ass. 5) 
}; 

struct T3dVertexPSIn { 
	float4 Pos : SV_POSITION; //Position in clip space 
	float2 Tex : TEXCOORD; //Texture coordinate 
	float3 PosWorld : POSITION; //Position in world space 
	float3 NorWorld : NORMAL; //Normal in world space 
	float3 TanWorld : TANGENT; //Tangent in world space (not used in Ass. 5) 
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

	output.Pos.x = (float)(x - g_TerrainRes / 2) / g_TerrainRes;
	output.Pos.y = g_HeightMap[VertexID];
	output.Pos.z = (float)(z - g_TerrainRes / 2) / g_TerrainRes;
	output.Pos.w = 1.0f;

	output.Pos = mul(output.Pos, g_WorldViewProjection);

	// calculate textures
	output.Tex.x = (float)x / g_TerrainRes;
	output.Tex.y = (float)z / g_TerrainRes;

	return output;
}


float4 TerrainPS(PosTex Input) : SV_Target0 {

	float3 n;
	float4 matNormal = g_NormalMap.Sample(samAnisotropic, Input.Tex);

	// apply the x,z coordinates
	//n = (float4(matDiffuse.xz)  * 2).xz - 1;

	//n.xz = matNormal.xy * 2 - 1;
	n.x = matNormal.x * 2 - 1;
	n.z = matNormal.y * 2 - 1;

	// calc the height y , length of the vector is 1
	//     length                      = sqrt (x*x + y*y + z*z)      | ^2
	// ->       1                      = x*x + y*y + z*z             | -x*x -z*z
	// ->       1 - x * x  -  z * z    = y*y                         | sqrt()
	// -> sqrt(1 -  x * x  -  z * z )  = y
	n.y	 = sqrt(1 - n.x * n.x - n.z * n.z);
	

	// transform and normalize 
	n = normalize(mul(n, g_WorldNormals).xyz); // Assume orthogonal world matrix
	float i = saturate(dot(n, g_LightDir.xyz) + 0.1f);

	// sample diffuse texture
	float3 matDiffuse = g_Diffuse.Sample(samLinearClamp, Input.Tex);

	return float4(matDiffuse.rgb * i, 1.0f);
}


// *********************************** Assignment 06 ***************************************
T3dVertexPSIn MeshVS(T3dVertexVSIn Input) {
	T3dVertexPSIn output = (T3dVertexPSIn)0;
	output.Pos = mul(float4(Input.Pos, 1.0f), g_WorldViewProjection);
	output.Tex = Input.Tex;

	float4 tmp_PosWorld = mul(float4(Input.Pos, 1.0f) , g_World);
		output.PosWorld = tmp_PosWorld.xyz * (1.0f / tmp_PosWorld.w);

	float4 tmp_Normal = mul(float4(Input.Nor, 0.0f), g_WorldNormals);
		output.NorWorld = tmp_Normal.xyz;

	float4 tmp_TanWorld = mul(float4(Input.Tan, 0.0f), g_WorldNormals);
		output.TanWorld = tmp_TanWorld.xyz;

	return output;
}

float4 MeshPS(T3dVertexPSIn Input) : SV_Target0{
	
	// cd, cs, ca,cg control weighting of individual terms 
	// try to find the best values!!
	float c_d, c_s, c_a, c_g;
	c_d = 0.5f;
	c_s = 0.4f;
	c_a = 0.1f;
	c_g = 0.5f;


	float3 matDiffuse = g_Diffuse.Sample(samAnisotropic, Input.Tex);

	float3 matSpecular = specularEV.Sample(samAnisotropic, Input.Tex);
	float3 matGlow = glowEV.Sample(samAnisotropic, Input.Tex);
	float4 colLight = float4(1, 1, 1, 1);
	float4 colLightAmbient = float4(1, 1, 1, 1);


	float3 n = normalize(Input.NorWorld);
	float3 l = g_LightDir;
	float3 r = reflect(-1*l, n);
	float3 v = normalize(cameraPosWorldEV - Input.PosWorld);


	// slides
	float3 line1 = c_d * matDiffuse * saturate(dot(n, l)) * colLight;
	//line1 = float3(line1.x*light_color.x, line1.y*light_color.y, line1.z*light_color.z);
	float3 line2 = c_s * matSpecular * pow(saturate(dot(r, v)), 100) * colLight;
	//line2 = float3(line2.x*light_color.x, line2.y*light_color.y, line2.z*light_color.z);
	float3 line3 = c_a * matDiffuse * colLightAmbient;
	//line3 = float3(line3.x*light_color.x, line3.y*light_color.y, line3.z*light_color.z);
	float3 line4 = c_g * matGlow;

	// I have no idea what I am doing

	return float4 ( line1 + line2 + line3 + line4, 1.0);

	// use until assignment 06 is not complete
	//return g_Diffuse.Sample(samAnisotropic, Input.Tex);

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
	
	pass P1_Mesh
	{
		SetVertexShader(CompileShader(vs_4_0, MeshVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, MeshPS()));

		SetRasterizerState(rsCullBack);
		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	} 
}
