#pragma once
#include "DirectXMath.h"
#include "src/ConfigParser.h"


namespace CustomData{

	struct SimpleVertex{
		DirectX::XMFLOAT4 Pos;
		DirectX::XMFLOAT4 Normal;
		DirectX::XMFLOAT2 UV;
	};

	static ConfigParser parser;
	
	
};

