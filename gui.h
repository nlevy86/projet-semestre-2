// Talia Meyer
// Noam Lévy

#ifndef GUI_H
#define GUI_H

#include <iostream>
#include "simulation.h"
#include "graphic_gui.h"
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/label.h>
#include <gtkmm/separator.h>
#include <glibmm/main.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/eventcontrollerkey.h>

struct Frame // Model Framing and window parameters
{
	double xMin; // frame parameters
	double xMax;
	double yMin;
	double yMax;
	double asp;  // frame aspect ratio
	int height;  // window height
	int width;   // window width
};

class Drawing : public Gtk::DrawingArea
{
public:
	Drawing(Simulation* simptr);
	
    void setFrame(Frame x); 
    void adjustFrame(int width, int height);
    void refresh();

protected:
	void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);

private:
	Simulation* sim;
    Frame frame;
    
};


class Gui : public Gtk::Window {
public:
	Gui(Simulation sim);
private:
	Simulation simulation;
	Drawing draw;
	bool apparition_algue = false;
	bool simulation_running = false;
	void on_button_clicked_exit();
	void on_button_clicked_open();
	void on_button_clicked_save();
	void on_file_dialog_response(int response_id, Gtk::FileChooserDialog* dialog);
	void on_button_clicked_start();
	void on_button_clicked_step();
	void activate_naissance_algue();
	bool on_window_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);
	bool start_simulation();
	bool is_save;
	bool keyb_driven_state;
	Gtk::Box main_box, second_box, button_box, general_box, info_box, nombre_box,
	maj_box, algue_box, cor_box, sca_box;
	Gtk::Button button_exit;
	Gtk::Button button_open;
	Gtk::Button button_save;
	Gtk::Button button_start;
	Gtk::Button button_step;
	Gtk::CheckButton naissance_algue;
	Gtk::Label label_general, label_info, label_maj, label_algue, label_cor, label_sca,
	label_nb_maj, label_nb_algue, label_nb_cor, label_nb_sca;
	Gtk::Separator separator1, separator2, separator3;
};

#endif
