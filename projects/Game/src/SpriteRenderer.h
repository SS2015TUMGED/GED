#pragma once

#include <string>
#include <vector>
#include <list>

#include <DXUT.h>
#include <DXUTcamera.h>

#include <d3dx11effect.h>


struct SpriteVertex
{
	DirectX::XMFLOAT3 position;     // world-space position (sprite center)
	float radius;                   // world-space radius (= half side length of the sprite quad)
	UINT index;               // which texture to use (out of SpriteRenderer::m_spriteSRV)
	float t;
	float alpha;
	DirectX::XMFLOAT3 velocity;		// Velocity vector of the Sprite
	float grav;
	float dmg;
	float camDist;
public:
	SpriteVertex(DirectX::XMFLOAT3 pos, float radius, UINT texInd) : position(pos), radius(radius), index(texInd),
		t(0.0f), alpha(1.0f), camDist(0.0f) {}
	SpriteVertex() {}
};




struct Explosion
{
	int index;
	float timePassed;
	float duration;
	SpriteVertex sprite;
public:
	Explosion(SpriteVertex spr) : index(2), timePassed(0.0f), duration(200.5f), sprite(spr) {}
	Explosion() {}
};

class SpriteRenderer
{
public:
	// Constructor: Create a SpriteRenderer with the given list of textures.
	// The textures are *not* be created immediately, but only when create is called!
	SpriteRenderer(const std::vector<std::wstring>& textureFilenames);
	// Destructor does nothing. Destroy and ReleaseShader must be called first!
	~SpriteRenderer();

	// Load/reload the effect. Must be called once before create!
	HRESULT reloadShader(ID3D11Device* pDevice);
	// Release the effect again.
	void releaseShader();

	// Create all required D3D resources (textures, buffers, ...).
	// reloadShader must be called first!
	HRESULT create(ID3D11Device* pDevice);
	// Release D3D resources again.
	void destroy();

	// Render the given sprites. They must already be sorted into back-to-front order.
	void renderSprites(ID3D11DeviceContext* context, const std::vector<SpriteVertex>& sprites, const CFirstPersonCamera& camera);

	// Global variable
	ID3DX11EffectMatrixVariable*			g_ViewProjection;
	ID3DX11EffectVectorVariable*			camRightVec;
	ID3DX11EffectVectorVariable*			camUpVec;
	ID3DX11EffectShaderResourceVariable*	sprTexGatling;
	ID3DX11EffectShaderResourceVariable*	sprTexPlasma;
	ID3DX11EffectShaderResourceVariable*	sprTexBoom;

private:
	std::vector<std::wstring> m_textureFilenames;

	// Rendering effect (shaders and related GPU state). Created/released in Reload/ReleaseShader.
	ID3DX11Effect* m_pEffect;

	// Sprite textures and corresponding shader resource views.
	//std::vector<ID3D11Texture2D*>          m_spriteTex;       // You may not need this if you use CreateDDSTExtureFromFile!
	std::vector<ID3D11ShaderResourceView*> m_spriteSRV;

	// Maximum number of allowed sprites, i.e. size of the vertex buffer.
	size_t m_spriteCountMax;
	// Vertex buffer for sprite vertices, and corresponding input layout.
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11InputLayout* m_pInputLayout;
};
