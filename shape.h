#include <iostream>
#include <cmath>
#include <string>

constexpr double epsil_zero(0.5) ;
constexpr double epsil_1(0.0000000001);

struct S2d{
	double x=0.;
	double y=0.;
};

struct Segment{
	S2d base;
	double angle;
	double longueur;
};

double ecart_angulaire(Segment segment_1, Segment segment_2);
bool on_segment(S2d p, S2d q, S2d r, bool is_epsil_zero);
int orientation(S2d p, S2d q, S2d r, bool is_epsil_zero);
bool do_intersect(S2d p1, S2d q1, S2d p2, S2d q2, bool is_epsil_zero);
