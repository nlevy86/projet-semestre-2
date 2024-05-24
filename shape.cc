// Talia Meyer 30%
// Noam Lévy   70%

#include <iostream>
#include <string>
#include "shape.h"

using namespace std;

Segment::Segment(const S2d& _base, double _angle, double _longueur):
	base(_base), angle(_angle), longueur(_longueur) {}

double ecart_angulaire(Segment segment_1, Segment segment_2){
	double ecart(segment_1.angle + (M_PI - segment_2.angle));
	if (ecart < -M_PI) ecart+= 2*M_PI;
	else if (ecart > M_PI) ecart -= 2*M_PI;
	
	return ecart;
}


bool superpo_commun(Segment segment_1, Segment segment_2, bool is_epsil_zero){
	if (is_epsil_zero) return abs(ecart_angulaire(segment_1, segment_2)) <= epsil_zero;
	
	return abs(ecart_angulaire(segment_1, segment_2)) <= epsil_1;
}

// point q lies on line segment 'pr'
bool on_segment(S2d p, S2d q, S2d r, bool is_epsil_zero) { 
	if (is_epsil_zero){
		double norme_pr(sqrt(pow((r.x-p.x),2)+pow((r.y-p.y),2)));
		double scal((r.x-p.x)*(q.x-p.x)+(r.y-p.y)*(q.y-p.y));
		return (-epsil_zero <= scal/norme_pr and 
				scal/norme_pr <= norme_pr + epsil_zero);
	}
	if (q.x <= max(p.x, r.x) and q.x >= min(p.x, r.x) and 
		q.y <= max(p.y, r.y) and q.y >= min(p.y, r.y)){
		return true; 
	}
	return false; 	
} 

// 0 --> p, q and r are collinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 

int orientation(S2d p, S2d q, S2d r, bool is_epsil_zero){
	double val = (q.y - p.y) * (r.x - q.x) -
				(q.x - p.x) * (r.y - q.y);
	
	if (is_epsil_zero){
		double distance = val/sqrt(pow((p.y - q.y),2) + pow((p.x - q.x),2));
		if(abs(distance) <= epsil_zero) return 0;
	} else {
		if (val ==0) return 0;
	}
	return (val > 0)? 1: 2;
}

bool do_intersect(S2d p1, S2d q1, S2d p2, S2d q2, bool is_epsil_zero){ 
    int o1 = orientation(p1, q1, p2, is_epsil_zero); 
    int o2 = orientation(p1, q1, q2, is_epsil_zero); 
    int o3 = orientation(p2, q2, p1, is_epsil_zero); 
    int o4 = orientation(p2, q2, q1, is_epsil_zero); 
  
    if (o1 and o2 and o3 and o4 and o1 != o2 and o3 != o4) return true; 
 
    if (o1 == 0 and on_segment(p1, p2, q1, is_epsil_zero)) return true; 
  
    if (o2 == 0 and on_segment(p1, q2, q1, is_epsil_zero)) return true; 
  
    if (o3 == 0 and on_segment(p2, p1, q2, is_epsil_zero)) return true; 
  
    if (o4 == 0 and on_segment(p2, q1, q2, is_epsil_zero)) return true; 
  
    return false;
}

void square(double x, double y, double d, double r, double g, double b){
	draw_line(x-(d/2),y-(d/2), d, 0, r, g, b);
	draw_line(x+(d/2),y-(d/2), d, M_PI/2, r, g, b);
	draw_line(x+(d/2),y+(d/2), d, -M_PI, r, g, b);
	draw_line(x-(d/2),y+(d/2), d, -M_PI/2, r, g, b);
}

void circle(double x, double y, double ray, double r, double g, double b){
	draw_arc (x, y, ray, -M_PI, M_PI, r, g, b);
}
