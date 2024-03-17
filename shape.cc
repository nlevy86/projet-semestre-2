#include <iostream>
#include <cmath>
#include <string>
#include "shape.h"

double ecart_angulaire(segment segment_1, segment segment_2){
	double ecart(segment_1.angle + (M_PI - segment_2.angle));
	if (ecart < -M_PI){
		ecart+= 2*M_PI;
	} else if (ecart > M_PI){
		ecart -= 2*M_PI;
	}
	return ecart;
}


