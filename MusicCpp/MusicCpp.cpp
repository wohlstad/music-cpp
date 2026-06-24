// MusicCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "MusicTypes.h"


void test1(std::string_view noteStr)
{
	Note n(noteStr);
	std::cout << n.ToStrDebug() << "\n";
	n.RaiseSemitone();
	std::cout << n.ToStrDebug() << "\n";
	n.LowerSemitone();
	std::cout << n.ToStrDebug() << "\n";
	n.LowerSemitone();
	std::cout << n.ToStrDebug() << "\n";
	n.LowerSemitone();
	std::cout << n.ToStrDebug() << "\n";
	n.LowerSemitone();
	std::cout << n.ToStrDebug() << "\n";
	n.LowerSemitone();
	std::cout << n.ToStrDebug() << "\n";
}


void test2(std::string_view noteStr)
{
	Note n(noteStr);
	std::cout << n.ToStrDebug() << "\n";
	n.LowerSemitone();
	std::cout << n.ToStrDebug() << "\n";
	n.RaiseSemitone();
	std::cout << n.ToStrDebug() << "\n";
	n.RaiseSemitone();
	std::cout << n.ToStrDebug() << "\n";
	n.RaiseSemitone();
	std::cout << n.ToStrDebug() << "\n";
	n.RaiseSemitone();
	std::cout << n.ToStrDebug() << "\n";
	n.RaiseSemitone();
	std::cout << n.ToStrDebug() << "\n";
}


int main() {

	//	test2("Cbb");
	//	test1("F##");
		//{
		//	ScaleMajor majScale{ Note{"Eb"} };
		//	majScale.Print();
		//}
		//{
		//	ScaleMajor majScale{ Note{"C#"} };
		//	majScale.Print();
		//}
		//{
		//	ScaleMajor majScale{ Note{"F"} };
		//	majScale.Print();
		//}
		//{
		//	ScaleMajor majScale{ Note{"Ab"} };
		//	majScale.Print();
		//}

	{
		ScaleMinor scale{ Note{"C#"} };
		scale.Print();
	}
}