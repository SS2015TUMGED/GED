#include "Terrain.h"
#include "ConfigParser.h"
#include "GameEffect.h"
#include "SimpleImage.h"
#include <DDSTextureLoader.h>
#include "DirectXTex.h"
#include "FillVertex.h"
#include "IndexBuffer.h"
#include "CustomData.h"


Terrain::Terrain(void):
	indexBuffer(nullptr),
	//vertexBuffer(nullptr),
	diffuseTexture(nullptr),
	diffuseTextureSRV(nullptr),
	debugSRV(nullptr)
{
}


Terrain::~Terrain(void)
{
}

HRESULT Terrain::create(ID3D11Device* device)
{
	using namespace std;

	HRESULT hr;

	// In our example, we load a debug texture
	//V(DirectX::CreateDDSTextureFromFile(device, L"..\\..\\Release\\resources\\debug_green.dds", nullptr, &debugSRV));

	//if (hr != S_OK) {
	//	MessageBoxA(NULL, "Could not load texture \"\\Release\\resources\\debug_green.dds\"", "Invalid texture", MB_ICONERROR | MB_OK);
	//	return hr;
	//}
	/*
	// This buffer contains positions, normals and texture coordinates for one triangle
	float triangle[] = {
	// Vertex 0
	-400.0f, 0.0f, -400.0f,  1.0f, // Position
	0.0f,    1.0f,    0.0f,  0.0f, // Normal
	0.0f,    0.0f,                 // Texcoords

	// Vertex 1
	400.0f,   0.0f, -400.0f, 1.0f, // Position
	0.0f,     1.0f,    0.0f, 0.0f, // Normal
	0.0f,     1.0f,                // Texcoords

	// Vertex 2
	-400.0f, 0.0f,  400.0f,  1.0f, // Position
	0.0f,    1.0f,    0.0f,  0.0f, // Normal
	1.0f,    0.0f,                 // Texcoords
	};
	//*/
	// TODO: Replace this vertex array (triangle) with an array (or vector)
	// which contains the vertices of your terrain. Calculate position,
	// normal and texture coordinates according to your height field and
	// the dimensions of the terrain specified by the ConfigParser
	// Note 2: For each vertex 10 floats are stored. Do not use another
	// layout.
	// Note 3: In the coordinate system of the vertex buffer (output):
	// x = east,    y = up,    z = south,          x,y,z in [0,1] (float)



	// ***************** 1 Load heightfield, Normal and Color **********************
	string heightPath = ConfigParser::height;
	string colorPath = ConfigParser::color;
	string normalPath = ConfigParser::normal;


	// *********************** create a buffer for the heightmap ****************

	// load the heightmap to get the resolution
	resolution = FillVertex::returnResolution(heightPath);

	// *********************** Assignment 04 *************************************
	// Create a new Vertex
	//std::vector<CustomData::SimpleVertex> triangle(resolution * resolution)
	// fill the vertex with the data from the heightmap
	//FillVertex::insertHeightfield(heightPath, triangle);
	// fill the vertex with Normals
	//FillVertex::insertNormalmap(triangle);
	// ************************* End Assignment 04 *******************************

	// ************************* Assignment 05 ****************************
	
		// vector where the array is stored
		std::vector<float> heightfield(resolution * resolution);

		// load the heightfield from the picture
		GEDUtils::SimpleImage hfield(heightPath.c_str());

		// fill the heightfield vector
		for (int y = 0; y < resolution; y++)
		{
			for (int x = 0; x < resolution; x++)
			{
				heightfield[IDX(x, y, resolution)] = hfield.getPixel(x, y);
			}
		}



	D3D11_SUBRESOURCE_DATA id;
	id.pSysMem = &heightfield[0]; // assignment 04 replace with triangle[0]
	id.SysMemPitch = sizeof(float); // Stride, assignment 04 add * 10
	id.SysMemSlicePitch = 0;
	
	D3D11_BUFFER_DESC bd;
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;  // assignment 04 : D3D11_BIND_VERTEX_BUFFER;
	bd.ByteWidth = heightfield.size() * sizeof(float); // assignment 04 add * 10, replace heightfield with triangle

    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    bd.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SHADER_RESOURCE_VIEW_DESC heightShader;
	heightShader.Buffer.FirstElement = 0;
	heightShader.Buffer.NumElements = resolution * resolution;
	heightShader.Format = DXGI_FORMAT_R32_FLOAT;
	heightShader.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

	V(device->CreateBuffer(&bd, &id, &heightBuffer)); // http://msdn.microsoft.com/en-us/library/ff476899%28v=vs.85%29.aspx
	V(device->CreateShaderResourceView(heightBuffer, &heightShader, &heightmap_ShaderResView));






	//*********************** Create index buffer ********************************

	int indexBufferSize = (resolution - 1) * (resolution - 1) * 6;
	numberOfIndicies = indexBufferSize;

	std::vector<int> indices(indexBufferSize);

	IndexBuffer::fillIndexBuffer(indices, resolution);

	
	// Create and fill description
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int)* indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	// Define initial data
	ZeroMemory(&id, sizeof(id));
	id.pSysMem = &indices[0];
	// Create Buffer
	V(device->CreateBuffer(&bd, &id, &indexBuffer));
	
	
	
	
	
	//************************ load textures ********************************
	// Load color texture (color map)
	// Insert your code to load the color texture and create
	// the texture "diffuseTexture" as well as the shader resource view
	// "diffuseTextureSRV"

	// convert std::string to std::wstring neccessary
	std::wstring w(colorPath.begin(), colorPath.end());
	DirectX::CreateDDSTextureFromFile(device, w.c_str() , nullptr, &diffuseTextureSRV);


	// ********************** load the normals *****************************

	// convert the normalPath to wstring and load the normal
	std::wstring w2(normalPath.begin(), normalPath.end());
	DirectX::CreateDDSTextureFromFile(device, w2.c_str(), nullptr, &normalmap_ShaderResView);









	// DELETE ALL CREATED VARS

	return hr;
}


void Terrain::destroy()
{
	//SAFE_RELEASE(vertexBuffer);
	
	//Release the index buffer
	SAFE_RELEASE(indexBuffer);
	//Release the terrain's shader resource view and texture
	SAFE_RELEASE(debugSRV);

	SAFE_RELEASE(diffuseTextureSRV);

	SAFE_RELEASE(heightBuffer);

	SAFE_RELEASE(heightmap_ShaderResView);

	SAFE_RELEASE(normalmap_ShaderResource);

	SAFE_RELEASE(normalmap_ShaderResView);
    
    
}


void Terrain::render(ID3D11DeviceContext* context, ID3DX11EffectPass* pass)
{
	HRESULT hr;
	// Bind the terrain vertex buffer to the input assembler stage 
    ID3D11Buffer* vbs[] = { nullptr, }; // assignment 04 vertexBuffer
    
	
	unsigned int strides[] = { 0, }, offsets[] = { 0, }; // assignment 04: 10 * sizeof(float)
    
	
	//context->IASetVertexBuffers(0, 1, vbs, strides, offsets);
	
	// Bind the terrain index buffer to the input assembler stage
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);



    // Tell the input assembler stage which primitive topology to use
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);    

    // Bind the SRV of the terrain diffuse texture to the effect variable
    // (instead of the SRV of the debug texture)
	V(g_gameEffect.diffuseEV->SetResource(diffuseTextureSRV));
	V(g_gameEffect.heightmap->SetResource(heightmap_ShaderResView));
	V(g_gameEffect.normalmap->SetResource(normalmap_ShaderResView));

	
	V(g_gameEffect.shader->SetInt( resolution));
	
	// additional light effects
	// weight of the light
	V(g_gameEffect.weight->SetFloat(1.0f));

	// the ambientlight color
	const float ambient_light[3] = { 1.0f, 1.0f, 1.0f };
	V(g_gameEffect.ambient_light->SetFloatVector(ambient_light));

	// the color of the light
	const float light_color[3] = { 1.0f, 1.0f, 1.0f };
	V(g_gameEffect.light_color->SetFloatVector(light_color));



    // Apply the rendering pass in order to submit the necessary render state changes to the device
    V(pass->Apply(0, context));

    // Draw
    // Use DrawIndexed to draw the terrain geometry using as shared vertex list
    // (instead of drawing only the vertex buffer)
    context->DrawIndexed(numberOfIndicies ,0 , 0);
}
