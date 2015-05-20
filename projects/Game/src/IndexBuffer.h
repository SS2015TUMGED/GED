#pragma once
#include <vector>


class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();


	// fills a IndexBuffer with the int resolution
	static void fillIndexBuffer(std::vector<int>& targetVector, int resolution);


};

