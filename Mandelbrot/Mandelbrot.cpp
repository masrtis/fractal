#include "stdafx.h"
#include <array>
#include <stdio.h>
#include <ctime>
#include <Windows.h>
#include <vector>
#include "Mandelbrot.h"
#include <math.h>



template<class T>
inline int IteratePoint(T cx, T cy)
{
	T x = 0;
	T y = 0;
	T xTemp = 0;
	int n = 0;

	while ( x*x + y*y < 4 && n < iterationCap)
	{
		xTemp = x*x - y*y + cx;
		y = 2 * x*y + cy;
		x = xTemp;
		++n;
	}
	return n;
}

template<class T>
void CalculateArrayBetween(T xMin, T xMax, T yMin, T yMax)
{
	static int frameCount = 0;
	++frameCount;
	auto pixelArrayPtr = new std::array<int, width*height>;

	T dx = (xMax - xMin) / width;
	T dy = (yMax - yMin) / height;
	
	std::clock_t start(std::clock());

	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			(*pixelArrayPtr)[j*width + i] = IteratePoint(xMin + i*dx, yMin + j*dy);
		}
	}

	std::cout << "Frame " << frameCount << ": " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;

	start = std::clock();
	WriteFile(pixelArrayPtr, std::string("../rawArrays/ex.txt"), xMin, xMax, yMin, yMax);
	std::cout << "Write " << ": " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
}



int main()
{
	CalculateArrayBetween(-2.0L, 0.5L, -1.25L, 1.25L);
	std::cout << "Press any key\n";
	std::getchar();
	return 0;
}

