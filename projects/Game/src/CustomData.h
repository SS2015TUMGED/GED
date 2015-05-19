#pragma once
#include "DirectXMath.h"
#include "ConfigParser.h"
#include <vector>


namespace CustomData{

	struct SimpleVertex{
		DirectX::XMFLOAT4 Pos;
		DirectX::XMFLOAT4 Normal;
		DirectX::XMFLOAT2 UV;
	};

	static ConfigParser parser;
	
	
};

