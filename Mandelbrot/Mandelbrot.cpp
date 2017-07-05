#include "stdafx.h"
#include <Windows.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <array>
#include <stdio.h>
#include <ctime>
#include <math.h>
#include <vector>
#include <string>

const int height(500);
const int width(500);
const int iterationCap(100);
const std::string baseDirectory("A:/Code/Mandelbrot/");

inline double normalize(int n, double x, double y)
{
	return n + 1 - log(log(sqrt(x*x+y*y))) / log(2);
}


template<class T>
inline double MandelbrotIteration(T cx, T cy)
{
	T x = 0;
	T y = 0;
	T xTemp = 0;
	int n = 0;

	while (x*x + y*y < 4 && n < iterationCap)
	{
		xTemp = x*x - y*y + cx;
		y = 2 * x*y + cy;
		x = xTemp;
		++n;
	}
	return normalize(n, x, y);
}

template<class T>
inline double BurningShipIteration(T cx, T cy)
{
	T x = 0;
	T y = 0;
	T xTemp = 0;
	int n = 0;

	while (x*x + y*y < 10 && n < iterationCap)//questionable
	{
		xTemp = x*x - y*y + cx;
		y = 2 * abs(x*y) + cy;
		x = xTemp;
		++n;
	}
	return normalize(n,x,y);
}

template<class T>
void CalculateArrayBetween(T xMin, T xMax, T yMin, T yMax)
{
	static int frameCount = 0;
	++frameCount;
	auto pixelArrayPtr = new std::array<T, width*height>;

	T dx = (xMax - xMin) / width;
	T dy = (yMax - yMin) / height;
	
	std::clock_t start(std::clock());

	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			(*pixelArrayPtr)[j*width + i] = MandelbrotIteration(xMin + i*dx, yMin + j*dy);
		}
	}

	std::cout << "Frame " << frameCount << ": " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;

	std::string writeLocation = baseDirectory + "rawArrays/" + std::to_string(frameCount) + ".txt";

	start = std::clock();
	WriteFile(pixelArrayPtr, writeLocation, xMin, xMax, yMin, yMax);
	std::cout << "Write took:" << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
}

void WriteFile(std::array<double, height*width>* pxArrayPtr, std::string& fileLocation, double xMin, double xMax, double yMin, double yMax)
{
	std::ofstream file;
	file.open(fileLocation.c_str());
	std::cout << "Opening file: " << fileLocation.c_str() << "\n";

	file << xMin << "," << xMax << "," << yMin << "," << yMax << "," << iterationCap << "\n";

	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			file << (*pxArrayPtr)[j*width + i];
			if (i % (width - 1) != 0 || i == 0)
				file << ",";
		}
		file << "\n";
	}

	std::cout << "Closing file\n";
	file.close();
}

int main(int argc, char* argv[])
{
	std::vector<double> frameBounds{ -2.3L, 0.7L, -1.2L, 1.2L };

	//parse arguments
	int i = 0;
	while (++i < argc)
	{
		char* parameter = argv[i];
		if (strcmp(parameter, "--frame") == 0)
		{
			int p = 0;
			while (++i < argc && p < 4)
			{
				try
				{
					frameBounds[p] = std::stod(argv[i]);
					++p;
				}
				catch (...)
				{
					std::cout << "Error: --frame argument couldn't be converted to numeric value\n";
					return -1;
				}
			}//frame arguments are read
			if (p != 4)
			{
				std::cout << "Error: exactly 4 parameters must follow the --frame argument\n";
				return -1;
			}
		}
	}

	CalculateArrayBetween(frameBounds[0], frameBounds[1], frameBounds[2], frameBounds[3] );
	
	return 0;
}

