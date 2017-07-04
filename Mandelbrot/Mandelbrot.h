#pragma once
#include <Windows.h>
#include <memory>
#include <string>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <string>

const int height(1000);
const int width(1000);
const int iterationCap(1000);

template<class A, class B>
void WriteFile(std::array<A,height*width>* pxArrayPtr, std::string& fileLocation, B xMin, B xMax, B yMin, B yMax)
{
	std::ofstream file;
	file.open(fileLocation.c_str());

	file << xMin << "," << xMax << "," << yMin << "," << yMax << "," << iterationCap << "\n";

	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			file << (*pxArrayPtr)[j*width + i];
			if (i % (width-1) != 0 || i == 0)
				file << ",";
		}
		file << "\n";
	}

	file.close();
}

