// Talia Meyer
// Noam Lévy

#include <iostream>
#include "graphic.h"

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);

void graphic_set_context( const Cairo::RefPtr<Cairo::Context>& cr){
	ptcr = &cr;
}

void draw_frame(int width, int height)
{
	(*ptcr)->set_line_width(1.0);
	(*ptcr)->set_source_rgb(0.0, 0.0, 0.0);
	if (width<height){
		(*ptcr)->move_to(0, (height/2)+(width/2));
		(*ptcr)->line_to(width,(height/2)+(width/2));
		(*ptcr)->line_to(width,(height/2)-(width/2));
		(*ptcr)->line_to(0,(height/2)-(width/2));
		(*ptcr)->line_to(0,(height/2)+(width/2));
	} else {
		(*ptcr)->move_to((width/2)-(height/2), 0);
		(*ptcr)->line_to((width/2)+(height/2), 0);
		(*ptcr)->line_to((width/2)+(height/2),height);
		(*ptcr)->line_to((width/2)-(height/2),height);
		(*ptcr)->line_to((width/2)-(height/2),0);
	}
	(*ptcr)->stroke();
}

void draw_line(double x, double y, double l, 
				double a, double r, double g, double b){
	(*ptcr)->set_line_width(1.0);
	(*ptcr)->set_source_rgb(r, g, b);
	(*ptcr)->move_to(x, y);
	(*ptcr)->line_to(x+l*cos(a), y+l*sin(a));
	(*ptcr)->stroke();
}

void draw_arc(double x, double y, double ray, 
				double start, double stop, double r, double g, double b){
	(*ptcr)->set_line_width(1.0);
	(*ptcr)->set_source_rgb(r, g, b);
	(*ptcr)->arc(x,y,ray,start,stop);
	(*ptcr)->stroke();
}


