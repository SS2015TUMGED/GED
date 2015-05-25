#pragma once
#include "DXUT.h"
#include "d3dx11effect.h"

class Terrain
{
public:
	Terrain(void);
	~Terrain(void);

	HRESULT create(ID3D11Device* device);
	void destroy();

	void render(ID3D11DeviceContext* context, ID3DX11EffectPass* pass);

	 int numberOfIndicies;


private:

	

	Terrain(const Terrain&);
	Terrain(const Terrain&&);
	void operator=(const Terrain&);

	// Terrain rendering resources
	
	// assignment 05 remove vertexBuffer
	//ID3D11Buffer*                           vertexBuffer;	// The terrain's vertices
	ID3D11Buffer*                           indexBuffer;	// The terrain's triangulation
	ID3D11Texture2D*                        diffuseTexture; // The terrain's material color for diffuse lighting
	ID3D11ShaderResourceView*               diffuseTextureSRV; // Describes the structure of the diffuse texture to the shader stages

	// assignment 05: add shader resource and shader resource view

	// for the normalmap
	ID3D11Texture2D*						normalmap_ShaderResource;
	ID3D11ShaderResourceView*				normalmap_ShaderResView;

	// for the heightmap
	ID3D11Buffer*							heightBuffer;
	ID3D11ShaderResourceView*				heightmap_ShaderResView;
	int resolution;



	// General resources
	ID3D11ShaderResourceView*               debugSRV;
};

