// Talia Meyer
// Noam Lévy

#ifndef GRAPHIC_H
#define GRAPHIC_H


#include <iostream>
#include "graphic_gui.h"
#include <gtkmm/drawingarea.h>

class Graphic : public Gtk::DrawingArea
{
public:
	Graphic();
protected:
	void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
};

#endif
