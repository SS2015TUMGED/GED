#include "IndexBuffer.h"
#include "Terrain.h"
#include "CustomData.h"

IndexBuffer::IndexBuffer()
{
}


IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::fillIndexBuffer(std::vector<int>& indbuf, int resolution){
	
	
	// number of squares in the vector
	int squares = (resolution - 1) *  (resolution - 1);
	
	// counter for the buffer
	int bufferCounter = 0;
	
	int count = 0;
	for (int y = 0; y < (resolution - 1); y++)
	{
		for (int x = 0; x < (resolution - 2); x++, count += 6)
		{
			indbuf[count] = IDX(x, y, resolution);
			indbuf[count + 1] = IDX(x + 1, y, resolution);
			indbuf[count + 2] = IDX(x, y + 1, resolution);
			indbuf[count + 3] = IDX(x, y + 1, resolution);
			indbuf[count + 4] = IDX(x + 1, y, resolution);
			indbuf[count + 5] = IDX(x + 1, y + 1, resolution);
		}
	}

}
