#pragma once
#include "DirectXMath.h"
#include "ConfigParser.h"
#include <vector>



// You can use this macro to access your height field
#define IDX(X,Y,WIDTH) ((X) + (Y) * (WIDTH))

// Help macros
#define DEG2RAD( a ) ( (a) * XM_PI / 180.f )

//extern ConfigParser parser;


namespace CustomData{

	struct SimpleVertex{
		DirectX::XMFLOAT4 Pos;
		DirectX::XMFLOAT4 Normal;
		DirectX::XMFLOAT2 UV;
	};

	
	
	
};

