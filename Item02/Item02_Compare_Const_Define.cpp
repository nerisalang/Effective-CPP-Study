#include<iostream>
#define ASPECT_RATIO_MACRO 1.653

const double AspectRatioConst = 1.653;

int main()
{
	double a = ASPECT_RATIO_MACRO;
	double b = AspectRatioConst;

	std::cout<< "Macor: " << a << ", Const: " << b << std::endl;
	return 0;
}

