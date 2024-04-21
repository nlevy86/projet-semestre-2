// Talia Meyer
// Noam Lévy

#include <iostream>
#include "graphic.h"

Graphic::Graphic()
{
set_draw_func(sigc::mem_fun(*this, &Graphic::on_draw));
}

void Graphic::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height){
	cr->set_line_width(1.0);
	cr->set_source_rgb(0.0, 0.0, 0.0);
	cr->move_to(0,0);
	cr->line_to(width, height);
	if (width < height){
		cr->move_to(0, (height/2)-(width/2));
		cr->line_to(width, (height/2)-(width/2));
		cr->line_to(width, (height/2)+(width/2));
		cr->line_to(0, (height/2)+(width/2));
		cr->line_to(0, (height/2)-(width/2));
	} else {
		cr->move_to((width/2)-(height/2), 0);
		cr->line_to((width/2)-(height/2), height);
		cr->line_to((width/2)+(height/2), height);
		cr->line_to((width/2)+(height/2), 0);
		cr->line_to((width/2)-(height/2), 0);
	}
	cr->stroke();
	set_content_width(500);
	set_content_height(500);
}
