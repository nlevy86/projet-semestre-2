// Talia Meyer
// Noam Lévy

#ifndef GRAPHIC_H
#define GRAPHIC_H


#include <iostream>
#include <cmath>
#include "graphic_gui.h"
#include <gtkmm/drawingarea.h>

void draw_frame(int width, int height);
void draw_line(double x, double y, double l, double a, double r, double g, double b);
void draw_arc(double x, double y, double ray, double start, double stop, double r, 
																double g, double b);
#endif
