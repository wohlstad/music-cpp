// MusicCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "MusicTypes.h"
#include <random>
#include <memory>
#include <assert.h>


void Quiz(int numRounds)
{
	RandomScaleGenerator scaleGen;
	for (int i = 0; i < numRounds; ++i)
	{
		auto res = scaleGen.GetNext();
		if (res)
		{
			auto& scale = res.value();
			std::cout << scale.Name();
			std::cout << " - press <ENTER> to reveal";
			getchar();
			std::cout << "--> " << scale.ToStr() << "\n";
		}
		else
		{
			std::cout << res.error() << "\n";
		}
		std::cout << "\n\n";
	}
}


void PrintScale(Scale::ScaleType type, Note const& root)
{
	std::string scaleName = Scale::GetScaleGenericName(type, root);
	std::cout << scaleName << ":  ";

	Scale scale{ type, root };
	std::cout << scale.ToStr();
	std::cout << "\n";
}


int main() {

	try
	{
//		PrintScale(Scale::ScaleType::NaturalMinor, Note{ "G#" });
//		std::cout << "\n";

//		Scale scale{ Scale::ScaleType::Major, Note{ "C" } };
//		scale = Scale{ Scale::ScaleType::Major, Note{ "D" } };

		Quiz(30);
	}
	catch (std::exception const& ex)
	{
		std::cout << "EXCEPTION: " << ex.what() << "\n";
	}
}