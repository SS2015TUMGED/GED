#include "SpriteRenderer.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <list>

#include "DXUT.h"
#include "d3dx11effect.h"
#include "SDKmisc.h"
#include <DDSTextureLoader.h>


// Convenience macros for safe effect variable retrieval
#define SAFE_GET_PASS(Technique, name, var)   {assert(Technique!=NULL); var = Technique->GetPassByName( name );						assert(var->IsValid());}
#define SAFE_GET_TECHNIQUE(effect, name, var) {assert(effect!=NULL); var = effect->GetTechniqueByName( name );						assert(var->IsValid());}
#define SAFE_GET_SCALAR(effect, name, var)    {assert(effect!=NULL); var = effect->GetVariableByName( name )->AsScalar();			assert(var->IsValid());}
#define SAFE_GET_VECTOR(effect, name, var)    {assert(effect!=NULL); var = effect->GetVariableByName( name )->AsVector();			assert(var->IsValid());}
#define SAFE_GET_MATRIX(effect, name, var)    {assert(effect!=NULL); var = effect->GetVariableByName( name )->AsMatrix();			assert(var->IsValid());}
#define SAFE_GET_SAMPLER(effect, name, var)   {assert(effect!=NULL); var = effect->GetVariableByName( name )->AsSampler();			assert(var->IsValid());}
#define SAFE_GET_RESOURCE(effect, name, var)  {assert(effect!=NULL); var = effect->GetVariableByName( name )->AsShaderResource();	assert(var->IsValid());}

SpriteRenderer::SpriteRenderer(const std::vector<std::wstring>& textureFilenames)
	: m_textureFilenames(textureFilenames)
	, m_pEffect(nullptr)
	, m_spriteSRV(0)
	, m_spriteCountMax(0.0f)
	, m_pVertexBuffer(nullptr)
	, m_pInputLayout(nullptr) {

}

// Destructor does nothing. Destroy and ReleaseShader must be called first!
SpriteRenderer::~SpriteRenderer() {

}

// Load/reload the effect. Must be called once before create!
HRESULT SpriteRenderer::reloadShader(ID3D11Device* pDevice) {
	HRESULT hr;
	WCHAR path[MAX_PATH];

	using namespace std;

	// Find and load the rendering effect
	V_RETURN(DXUTFindDXSDKMediaFileCch(path, MAX_PATH, L"shader\\spriteRenderer.fxo"));
	ifstream is(path, std::ios_base::binary);
	is.seekg(0, std::ios_base::end);
	streampos pos = is.tellg();
	is.seekg(0, std::ios_base::beg);
	vector<char> effectBuffer((unsigned int)pos);
	is.read(&effectBuffer[0], pos);
	is.close();
	V_RETURN(D3DX11CreateEffectFromMemory((const void*)&effectBuffer[0], effectBuffer.size(), 0, pDevice, &m_pEffect));
	assert(m_pEffect->IsValid());
	return S_OK;
}

// Release the effect again.
void SpriteRenderer::releaseShader() {
	SAFE_RELEASE(m_pEffect);
}

// Create all required D3D resources (textures, buffers, ...).
// reloadShader must be called first!
HRESULT SpriteRenderer::create(ID3D11Device* pDevice) {
	HRESULT hr;

	D3D11_BUFFER_DESC bd;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.ByteWidth = 1024 * sizeof(SpriteVertex);
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.Usage = D3D11_USAGE_DEFAULT;

	V(pDevice->CreateBuffer(&bd, nullptr, &m_pVertexBuffer));

	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "RADIUS", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INDEX", 0, DXGI_FORMAT_R32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "STATE", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "ALPHA", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// Create the input layout
	D3DX11_PASS_DESC passDesc;

	V_RETURN(m_pEffect->GetTechniqueByName("sRender")->GetPassByName("P0")->GetDesc(&passDesc));

	V_RETURN(pDevice->CreateInputLayout(layout, numElements, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_pInputLayout));

	m_spriteSRV.resize(m_textureFilenames.size());
	for (int i = 0; i < m_textureFilenames.size(); i++)
	{
		V(DirectX::CreateDDSTextureFromFile(pDevice, m_textureFilenames[i].c_str(), nullptr, &m_spriteSRV[i]));
		std::cout << m_textureFilenames[i].c_str() << std::endl;
	}
	return S_OK;
}

// Release D3D resources again.
void SpriteRenderer::destroy() {
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pInputLayout);
	for (int i = 0; i < m_spriteSRV.size(); i++)
		SAFE_RELEASE(m_spriteSRV[i]);
}

// Render the given sprites. They must already be sorted into back-to-front order.
void SpriteRenderer::renderSprites(ID3D11DeviceContext* context, const std::vector<SpriteVertex>& sprites, const CFirstPersonCamera& camera) {

	HRESULT hr;

	if (sprites.size() > 1) {
		std::cout;
	}

	D3D11_BOX box;
	box.left = 0; box.right = sprites.size() * sizeof(SpriteVertex);
	box.top = 0; box.bottom = 1; box.front = 0; box.back = 1;
	context->UpdateSubresource(m_pVertexBuffer, 0, &box, sprites.data(), 0, 0);

	// Bind the vertex buffer to the input assembler stage 
	unsigned int strides[] = { sizeof(SpriteVertex), }, offsets[] = { 0, };
	context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, strides, offsets);
	// Set the Input Layout
	context->IASetInputLayout(m_pInputLayout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// Load variables to shader
	SAFE_GET_MATRIX(m_pEffect, "g_ViewProjection", g_ViewProjection);
	SAFE_GET_VECTOR(m_pEffect, "g_CamRVec", camRightVec);
	SAFE_GET_VECTOR(m_pEffect, "g_CamUVec", camUpVec);
	SAFE_GET_RESOURCE(m_pEffect, "g_SprTexGatling", sprTexGatling);
	SAFE_GET_RESOURCE(m_pEffect, "g_SprTexPlasma", sprTexPlasma);
	//SAFE_GET_RESOURCE(m_pEffect, "g_SprTex3", sprTex3);

	// Set view and projection transformations to get sprites to the right positions in world space
	DirectX::XMMATRIX viewProj = camera.GetViewMatrix() * camera.GetProjMatrix();
	V(g_ViewProjection->SetMatrix((float*)&viewProj));
	
	// Set Textures
	V(sprTexGatling->SetResource(m_spriteSRV[0]));
	V(sprTexPlasma->SetResource(m_spriteSRV[1]));
	//V(sprTex3->SetResource(m_spriteSRV[2]));
	
	// Get camera's right and up vector
	V(camRightVec->SetFloatVector((float*)&camera.GetWorldRight()));
	V(camUpVec->SetFloatVector((float*)&camera.GetWorldUp()));

	// Apply the pass from the effect
	V(m_pEffect->GetTechniqueByName("sRender")->GetPassByName("P0")->Apply(0, context));

	// Draw
	context->Draw(sprites.size(), 0);
}
