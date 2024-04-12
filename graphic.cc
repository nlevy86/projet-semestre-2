#include "graphic.h"
#include <iostream>
#include <vector>
using namespace std;

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr); 

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr)  {ptcr = &cr;}

void draw_figure(double x, double y, double l, char c ){
	if (c=='P'){
		(*ptcr)->set_line_width(1.0);
		(*ptcr)->set_source_rgb(1.0,0.,0);
		(*ptcr)->rectangle(x-l/2, y-l/2, l, l);
		(*ptcr)->stroke();
		(*ptcr)->set_source_rgb(0.8,0.8,0.8);
		(*ptcr)->rectangle(x-l/2, y-l/2, l, l);
		(*ptcr)->fill();
		(*ptcr)->stroke();
	}
	else {		
		vector<double> color1;
		vector<double> color2;
		if (c=='R') {
			color1={0,0,0};
			color2={0,1,0};
		}
		if(c=='N') {
			color1={0,0,0};
			color2={1,1,1};
			draw_center(x,y);
		}
		if(c=='S') {
			color1={0.5,0.5,0.9};
			color2={1,1,1};	
			draw_center(x,y);
			
		}		
		(*ptcr)->set_line_width(1.0);
		(*ptcr)->set_source_rgb(color1[0],color1[1],color1[2]);
		(*ptcr)->arc(x, y, l, 0.0, 2.0 * M_PI);
		(*ptcr)->stroke();
		(*ptcr)->set_source_rgb(color2[0],color2[1],color2[2]);
		(*ptcr)->arc(x, y, l, 0.0, 2.0 * M_PI);
		(*ptcr)->fill();
		(*ptcr)->stroke();
		if (c=='S') {draw_center(x,y);}
	}
}

void draw_center(double x, double y){
	(*ptcr)->set_line_width(0.7);
	(*ptcr)->set_source_rgb(0,0,0);
	(*ptcr)->arc(x, y, 0.6, 0.0, 2.0 * M_PI);
	(*ptcr)->fill();
	(*ptcr)->stroke();
}

void draw_line(double x0, double y0, double r, double a){
	(*ptcr)->set_line_width(0.7);
	(*ptcr)->set_source_rgb(0,1.,0);

	double x1 = x0 + r * cos(a);
	double y1 = y0 - r * sin(a);

	(*ptcr)->move_to(x0, y0);
	(*ptcr)->line_to(x1, y1);
	(*ptcr)->stroke();
}

void draw_frame(Frame frame){
	(*ptcr)->set_line_width(1.5);
	(*ptcr)->set_source_rgb(0.8, 0.8, 0.8);
	(*ptcr)->rectangle(-dmax,-dmax,2*dmax,2*dmax);
	(*ptcr)->stroke();
	(*ptcr)->set_source_rgb(1, 1, 1);
	(*ptcr)->rectangle(-dmax,-dmax,2*dmax,2*dmax);
	(*ptcr)->fill();
	(*ptcr)->stroke();
}
