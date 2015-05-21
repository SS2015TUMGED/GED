#include "IndexBuffer.h"
#include "Terrain.h"

IndexBuffer::IndexBuffer()
{
}


IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::fillIndexBuffer(std::vector<int>& targetVector, int resolution){
	
	
	// number of squares in the vector
	int squares = (resolution - 1) *  (resolution - 1);
	
	

	// counter for the buffer
	int bufferCounter = 0;


	// iterate through all the squares and calc the triangles
	for (int i = 0;  i < squares;  i++)
	{
		targetVector[bufferCounter] = i;

		bufferCounter++;

		targetVector[bufferCounter] = i + 1;

		bufferCounter++;

		targetVector[bufferCounter] = i + resolution;

		bufferCounter++;

		targetVector[bufferCounter] = i + resolution;

		bufferCounter++;

		targetVector[bufferCounter] = i + 1;

		bufferCounter++;

		targetVector[bufferCounter] = i + 1 + resolution;

		bufferCounter++;
	}




}
