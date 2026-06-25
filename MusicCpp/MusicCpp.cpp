// MusicCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "MusicTypes.h"
#include <random>
#include <memory>
#include <assert.h>


void Quiz()
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<int> distLetter(0, 6);		  // range [0, 6]: A .. G
	std::uniform_int_distribution<int> distAccidentalCnt (-1, 1); // range [-1, 1]: -1: flat, 0:natural, 1:sharp
	std::uniform_int_distribution<int> distScaleType(0, static_cast<int>(Scale::ScaleType::ScaleType_COUNT)-1);	  // range [0, 3]: 0: major,  1: minor,  2: harmonic minor, 3: melodic minor 

	for (int i = 0; i < 30; ++i)
	{
		char rootLetter = 'A' + distLetter(rng);	
		int rootAccCnt = distAccidentalCnt(rng);    
		assert(rootLetter >= 'A' && rootLetter <= 'G');
		assert(rootAccCnt == -1 || rootAccCnt == 0 || rootAccCnt == 1);
		Scale::ScaleType scaleType = static_cast<Scale::ScaleType>(distScaleType(rng));
		assert(scaleType < Scale::ScaleType::ScaleType_COUNT);
		Note root(rootLetter, rootAccCnt);
		std::string scaleName = Scale::GetName(scaleType, root);
		std::cout << scaleName;
		try
		{
			Scale scale{ scaleType, root };
			std::cout << "   - press <ENTER> to reveal";
			getchar();
			std::cout << "--> ";
			std::cout << scale.ToStr();
		}
		catch (std::exception const & ex)
		{
			std::cout << "\n***   FAILED to create scale: " << ex.what() << "\n";
		}
		std::cout << "\n";
	}
}


void PrintScale(Scale::ScaleType type, Note const& root)
{
	std::string scaleName = Scale::GetName(type, root);
	std::cout << scaleName << ":  ";

	Scale scale{ type, root };
	std::cout << scale.ToStr();
	std::cout << "\n";
}


int main() {

	try
	{
		PrintScale(Scale::ScaleType::NaturalMinor, Note{ "C" });

		Quiz();
		//{
		//	ScaleMajor{ Note{"Gb"} }.Print();
		//}
		//{
		//	ScaleMinor{ Note{"D"} }.Print();
		//}
		//{
		//	ScaleMinorHarmonic{ Note{"C#"} }.Print();
		//}
		//{
		//	ScaleMinorMelodic{ Note{"C#"} }.Print();
		//}
	}
	catch (std::exception const& ex)
	{
		std::cout << "EXCEPTION: " << ex.what() << "\n";
	}
}