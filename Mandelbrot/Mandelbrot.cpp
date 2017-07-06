#include "stdafx.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <array>
#include <stdio.h>
#include <ctime>
#include <math.h>
#include <vector>
#include <string>

//Config ----------------------------------------------------------------
const int height(2000);
const int width(2000);
const int iterationCap(2000);

//Why set the base directory as absolute path? I want running the binary from MSVS
//and running the binary from the command line to create files in the same place.
const std::string baseDirectory("A:/Code/Mandelbrot/");
//#define dontWriteFile //This is mostly or benchmarking the main loop
#define takeModOfIterations
//------------------------------------------------------------------------


template<class T>
inline double MandelbrotIteration(T cx, T cy)
{
	T x = 0;
	T y = 0;

	//Claim: computing the sqaures once and saving them results in a 5% performance improvement.
	T x2 = 0;
	T y2 = 0;

	T xTemp = 0;
	int n = 0;

	while (true)
	{
		x2 = x*x;
		y2 = y*y;
		if (x2 + y2 >= 4 || n == iterationCap)
			break;
		xTemp = x2 - y2 + cx;
		y = 2 * x*y + cy;
		x = xTemp;
		++n;
	}

#ifdef takeModOfIterations
	//If n is at the bailout value, it's left alone so the renderer can place it off the scale
	return n == iterationCap ? n : n % 10;
#else
	return n;
#endif
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
		xTemp = x*x - y*y - cx;
		y = 2 * abs(x*y) - cy;
		x = xTemp;
		++n;
	}
#ifdef takeModOfIterations
	return n == iterationCap ? n : n % 10;
#else
	return n;
#endif
}

template<class T>
void CalculateArrayBetween(T xMin, T xMax, T yMin, T yMax)
{
	static int frameCount = 0;
	++frameCount;

	//The below allocation will be moved out of this function in the event that many frames are computed
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
#ifndef dontWriteFile
	start = std::clock();
	WriteFile(pixelArrayPtr, writeLocation, xMin, xMax, yMin, yMax);
	std::cout << "Write took: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
#endif
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
	std::vector<double> frameBounds{ -2.3L, 0.7L, -1.2L, 1.2L };//mandelbrot default
	std::vector<double> frameBounds{ -4.0L, 4.0L, -2.0L, 2.0L };//burning ship default (biggest ship @ 1.7 1.82 -0.02 0.08)

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
					frameBounds[p] = std::stod(argv[i]);//stod() actually has some stange ideas about what strings are convertible
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

