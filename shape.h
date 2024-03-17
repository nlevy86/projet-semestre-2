#include <iostream>
#include <cmath>
#include <string>

constexpr double epsil_zero(0.5) ;

struct S2d{
	double x;
	double y;
};

struct segment{
	S2d base;
	double angle;
	double longueur;
};

double ecart_angulaire(segment segment_1, segment segment_2);
