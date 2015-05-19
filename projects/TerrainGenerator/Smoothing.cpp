#include "Smoothing.h"
#include "2DAControl.h"
#include <ppl.h>


//determines if the coordinate is inside the 2D vector with width width and height height
bool Smoothing::isValidCoord(int x, int y, int width, int height){
	if (x >= 0 && x < width && y >= 0 && y < height){
		return true;
	}
	return false;
}

/*
Generates all the coordinates all the points that are used to smooth back into one
array position
*/
concurrency::concurrent_vector<bestGroup::Vec2f> Smoothing::getSmoothingCoords(int x0,
	int y0, int range, int width, int height){

	int f = 1 - range;
	int ddF_x = 0;
	int ddF_y = -2 * range;
	int x = 0;
	int y = range;
	//creating new return vector
	concurrency::concurrent_vector<bestGroup::Vec2f> coords;

	coords.push_back(bestGroup::Vec2f(x0, y0));
	if (isValidCoord(x0, y0 + range, width, height))
	{
		coords.push_back(bestGroup::Vec2f(x0, y0 + range));
	}
	if (isValidCoord(x0, y0 - range, width, height))
	{
		coords.push_back(bestGroup::Vec2f(x0, y0 - range));
	}
	if (isValidCoord(x0 + range, y0, width, height))
	{
		coords.push_back(bestGroup::Vec2f(x0 + range, y0));
	}
	if (isValidCoord(x0 - range, y0, width, height))
	{
		coords.push_back(bestGroup::Vec2f(x0 - range, y0));
	}

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;
		
		//coords.push_back(bestGroup::Vec2f(x0 + x, y0 + y));
		//coords.push_back(bestGroup::Vec2f(x0 - x, y0 + y));
		//coords.push_back(bestGroup::Vec2f(x0 + x, y0 - y));
		//coords.push_back(bestGroup::Vec2f(x0 - x, y0 - y));
		int ypos = (y0 + y);
		int ypos2 = (y0 - y);
		for(int xpos = (x0 - x); xpos <= (x0 + x); xpos++) {
			if (isValidCoord(xpos, ypos, width, height)) {
				coords.push_back(bestGroup::Vec2f(xpos, ypos));
			}
			if (isValidCoord(xpos, ypos2, width, height)) {
				coords.push_back(bestGroup::Vec2f(xpos, ypos2));
			}
		};

		//coords.push_back(bestGroup::Vec2f(x0 + y, y0 + x));
		//coords.push_back(bestGroup::Vec2f(x0 - y, y0 + x));
		//coords.push_back(bestGroup::Vec2f(x0 + y, y0 - x));
		//coords.push_back(bestGroup::Vec2f(x0 - y, y0 - x));

		ypos = y0 + x;
		ypos2 = y0 - x;
		for (int xpos = (x0 - y); xpos <= (x0 + y); xpos++){
			if (isValidCoord(xpos, ypos, width, height)) {
				coords.push_back(bestGroup::Vec2f(xpos, ypos));
			}
			if (isValidCoord(xpos, ypos2, width, height)) {
				coords.push_back(bestGroup::Vec2f(xpos, ypos2));
			}
		};
		
	}

	return coords;
}

void Smoothing::squareSmoothing(std::vector<float> &array2D_, int width_, int height_){

	//create new, temporal array
	std::vector<float> array2D = std::vector<float>(width_ * height_);

	//get the 4 corners right
#pragma region corners
	//top left
	array2D[0] = (
		array2D_[0] + //corner piece
		array2D_[1] + //right of the corner
		array2D_[width_] + //below the corner
		array2D_[width_ + 1] //bottom right
		) / 4.0f;

	//top right
	array2D[width_ - 1] = (
		array2D_[width_ - 1] + //corner piece
		array2D_[width_ - 2] + //left of the corner
		array2D_[width_ + width_ - 1] + //below the corner
		array2D_[width_ + width_ - 2] //bottom left
		) / 4.0f;

	//bottom left
	array2D[IDX(0, height_ - 1, width_)] = (
		array2D_[IDX(0, height_ - 1, width_)] + //corner piece
		array2D_[IDX(0, height_ - 2, width_)] + //top of the corner
		array2D_[IDX(1, height_ - 1, width_)] + //right of the corner
		array2D_[IDX(1, height_ - 2, width_)] //top right
		) / 4.0f;

	//bottom right
	array2D[IDX(width_ - 1, height_ - 1, width_)] = (
		array2D_[IDX(width_ - 1, height_ - 1, width_)] + //corner piece
		array2D_[IDX(width_ - 1, height_ - 2, width_)] + //top of the corner
		array2D_[IDX(width_ - 2, height_ - 1, width_)] + //left of the corner
		array2D_[IDX(width_ - 2, height_ - 2, width_)] //top left
		) / 4.0f;

	//get the edges right
#pragma endregion
#pragma region edges
	int i;
	for (i = 1; i < width_ - 1; i++){
		//top edge
		array2D[i] = (
			array2D_[i] + //piece
			array2D_[i - 1] + //left of piece
			array2D_[i + 1] + //right of piece
			array2D_[i + width_] + //below piece
			array2D_[i + width_ - 1] + //bottom left
			array2D_[i + width_ + 1] //bottom right
			) / 6.0f;
		//bottom edge
		array2D[IDX(i, height_ - 1, width_)] = (
			array2D_[IDX(i, height_ - 1, width_)] + //piece
			array2D_[IDX(i - 1, height_ - 1, width_)] + //left of piece
			array2D_[IDX(i + 1, height_ - 1, width_)] + //right of piece
			array2D_[IDX(i, height_ - 2, width_)] + //top of piece
			array2D_[IDX(i - 1, height_ - 2, width_)] + //top left
			array2D_[IDX(i + 1, height_ - 2, width_)] //top right
			) / 6.0f;
		//left edge
		array2D[IDX(0, i, width_)] = (
			array2D_[IDX(0, i, width_)] + //piece
			array2D_[IDX(0, i - 1, width_)] + //top of piece
			array2D_[IDX(0, i + 1, width_)] + //below piece
			array2D_[IDX(1, i, width_)] + //right of piece
			array2D_[IDX(1, i - 1, width_)] + //Top right
			array2D_[IDX(1, i + 1, width_)] //Bottom right
			) / 6.0f;
		//right edge
		array2D[IDX(width_ - 1, i, width_)] = (
			array2D_[IDX(width_ - 1, i, width_)] + //piece
			array2D_[IDX(width_ - 1, i - 1, width_)] + //top of piece
			array2D_[IDX(width_ - 1, i + 1, width_)] + //below of piece
			array2D_[IDX(width_ - 2, i, width_)] + //left of piece
			array2D_[IDX(width_ - 2, i - 1, width_)] + //Top left
			array2D_[IDX(width_ - 2, i + 1, width_)] //Bottom left
			) / 6.0f;
	}
#pragma endregion

	//get the rectangle in the middle
	int xpos, ypos;
	for (ypos = 1; ypos < height_ - 1; ypos++){ //"lines"
		int startOfLinePos = IDX(0, ypos, width_); //position of the first  piece of the "line"
		for (xpos = 1; xpos < width_ - 1; xpos++){ //"columns"
			array2D[startOfLinePos + xpos] = (
				array2D_[startOfLinePos + xpos] + //piece
				array2D_[startOfLinePos + xpos - width_] + //top of piece
				array2D_[startOfLinePos + xpos + width_] + //below piece
				array2D_[startOfLinePos + xpos - 1] + //left of piece
				array2D_[startOfLinePos + xpos + 1] + //right of piece
				array2D_[startOfLinePos + xpos - width_ - 1] + //top right
				array2D_[startOfLinePos + xpos + width_ - 1] + //bottom right
				array2D_[startOfLinePos + xpos - width_ + 1] + //top left
				array2D_[startOfLinePos + xpos + width_ + 1]  //bottom left
				) / 9.0f;
		}
	}

	//copy array2D to array2D_
	for (int ypos = 0; ypos < height_; ypos++){
		for (int xpos = 0; xpos < width_; xpos++){
			array2D_[IDX(xpos, ypos, width_)] =
				array2D[IDX(xpos, ypos, width_)];
		}
	}
}

//function to smooth the values in a 2D Array
void Smoothing::circularSmoothing(std::vector<float> &array2D_, int width_, int height_, int  range){

	//check if range is valid
	if (range > width_ || range > height_){
		std::cerr << "Smothing failed! range too big!";
	}
	else if (range < 1){
		std::cerr << "Smoothing failed! range too small!";
	}

	concurrency::concurrent_vector<float> array2D(width_ * height_);
	
	concurrency::concurrent_vector<float> array2D__(array2D_.begin(), array2D_.end());
	
	float counter = 1.0f;
	//iterate through the array
	concurrency::parallel_for(int(0),height_,[&] (int ypos){
		for (int xpos = 0; xpos < width_; xpos++)
		{
			//hold all the coordinates to smooth
			concurrency::concurrent_vector<bestGroup::Vec2f> finalCoordinates;

			finalCoordinates = getSmoothingCoords(xpos, ypos, range, width_, height_);

			finalCoordinates.shrink_to_fit();

			float value = 0.0f;

			//get all the values from the original array
			for (unsigned i = 0; i < finalCoordinates.size(); i++){
				value += array2D__[IDX(finalCoordinates[i].x, finalCoordinates[i].y, width_)];
			}

			value = value / finalCoordinates.size();
			array2D[IDX(xpos, ypos, width_)] = value;
			counter++;
		}

		//counter++;
		std::cout << (int) (((counter / width_)/width_) * 100.0f) << "%                   " << "\r";
		std::cout.flush();

	});

	//copy array2D to array2D_
	for (int ypos = 0; ypos < height_; ypos++){
		for (int xpos = 0; xpos < width_; xpos++){
			array2D_[IDX(xpos, ypos, width_)] =
				array2D[IDX(xpos, ypos, width_)];
		}
	}
}

void Smoothing::squareSmoothing_nTimes(std::vector<float> &array2D_, int width_, int height_, int n){
	using namespace std;
	int g;
	for (g = 1; g <= n; g++){
		cout << endl;
		cout << "Smoothing " << g << "/" << n << endl;;
		Smoothing::squareSmoothing(array2D_, width_, height_);
	}
	cout << "smoothing complete" << endl;
}

void Smoothing::circularSmoothing_nTimes(std::vector<float> &array2D_, int width_, int height_, int n, int range){
	using namespace std;
	int g;
	for (g = 1; g <= n; g++){
		cout << endl;
		cout << "Smoothing " << g << "/" << n << endl;;
		Smoothing::circularSmoothing(array2D_, width_, height_, range);
	}
	cout << "smoothing complete" << endl;
}

void Smoothing::anotherSimpleSmoothing(std::vector<float> &array2d_, int width, int n, int range){
	using namespace std;
	std::vector<float> tempvec(width*width);
	int count;
	float temp;
	for (int i = 0; i < n; i++){
		cout << endl;
		cout << "Smoothing " << i << "/" << n << endl;
		for (int y = 0; y < width; y++){
			for (int x = 0; x < width; x++){

				temp = 0.0f;
				count = 0;
				if (isValidCoord(x - range, y - range, width, width)){
					temp += array2d_[IDX(x - range, y - range, width)];
					count++;
				}
				if (isValidCoord(x, y - range, width, width)){
					temp += array2d_[IDX(x, y - range, width)];
					count++;
				}
				if (isValidCoord(x + range, y - range, width, width)){
					temp += array2d_[IDX(x + range, y - range, width)];
					count++;
				}
				if (isValidCoord(x - range, y, width, width)){
					temp += array2d_[IDX(x - range, y, width)];
					count++;
				}
				if (isValidCoord(x + range, y, width, width)){
					temp += array2d_[IDX(x + range, y, width)];
					count++;
				}

				if (isValidCoord(x - range, y + range, width, width)){
					temp += array2d_[IDX(x - range, y + range, width)];
					count++;
				}

				if (isValidCoord(x, y + range, width, width)){
					temp += array2d_[IDX(x, y + range, width)];
					count++;
				}
				if (isValidCoord(x + range, y + range, width, width)){
					temp += array2d_[IDX(x + range, y + range, width)];
					count++;
				}
				tempvec[IDX(x, y, width)] = temp / count;

			}
		}
		array2d_ = tempvec;
	}
}