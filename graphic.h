#ifndef GTKM_GRAPHIC_H_INCLUDED
#define KTKM_GRAPHIC_H_INCLUDED

#include <gtkmm.h>
#include <iostream>
#include <vector>
#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>
#include <cmath>
#include "constantes.h"

struct Frame {
	double xMin, xMax, yMin, yMax, asp; 
	int height, width; 
};

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr);

void draw_figure(double x, double y, double l, char c );

void draw_center(double x, double y);

void draw_line(double x0, double y0, double r, double a);

void draw_frame(Frame frame);

#endif
