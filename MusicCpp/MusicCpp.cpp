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
	std::uniform_int_distribution<int> distLetter(0, 6);		  // range [0, 6]
	std::uniform_int_distribution<int> distAccidentalCnt (-1, 1); // range [-1, 1]
	std::uniform_int_distribution<int> distScaleType3(1, 4);	  // range [1, 4]
	for (int i = 0; i < 30; ++i)
	{
		char letter = 'A' + distLetter(rng);	// A .. G
		int accCnt = distAccidentalCnt(rng);    // -1 (flat) / 0 (natural) / 1 (sharp)
		assert(letter >= 'A' && letter <= 'G');
		assert(accCnt == -1 || accCnt == 0 || accCnt == 1);
		int scaleType = distScaleType3(rng);	// 1: major,  2: minor,  3: harmonic minor, 4: melodic minor 
		Note root(letter, accCnt);
		std::unique_ptr<Scale> scale;
		std::string scaleName;
		switch (scaleType)
		{
		case 1: scaleName = GetScaleName<ScaleMajor>(root); break;
		case 2: scaleName = GetScaleName<ScaleMinor>(root); break;
		case 3: scaleName = GetScaleName<ScaleMinorHarmonic>(root); break;
		case 4: scaleName = GetScaleName<ScaleMinorMelodic>(root); break;
		default: assert(false); throw std::exception("quiz: unexpected scaleType");
		}
		std::cout << scaleName;
		try
		{
			switch (scaleType)
			{
			case 1: scale = std::make_unique<ScaleMajor>(root);	break;
			case 2: scale = std::make_unique<ScaleMinor>(root);	break;
			case 3: scale = std::make_unique<ScaleMinorHarmonic>(root);	break;
			case 4: scale = std::make_unique<ScaleMinorMelodic>(root); break;
			default: assert(false); throw std::exception("quiz: unexpected scaleType");
			}

			std::cout << "   - press <ENTER> to reveal";
			getchar();
			std::cout << "--> ";
			std::cout << scale->ToStr();
		}
		catch (std::exception const & ex)
		{
			std::cout << "\n***   FAILED to create scale: " << ex.what() << "\n";
		}
		std::cout << "\n";
	}
}


template<typename T>
void PrintScale(Note const & root)
{
	std::string scaleName = GetScaleName<T>(root);
	std::cout << scaleName << ":  ";

	T scale{ root };
	std::cout << scale.ToStr();
	std::cout << "\n";
}


int main() {

	try
	{
		//PrintScale<ScaleMinor>(Note{ "F" });
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