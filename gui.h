#ifndef GTKMM_GUI_H
#define GTKMM_GUI_H

#include <gtkmm.h>
#include <gtkmm/application.h>
#include <cairomm/context.H>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/drawingarea.h>
#include "simulation.h"
#include "constante.h"
#include "graphic.h"

int gui(int argc, char** argv, Simulation simulation);

class Drawing : public Gtk::DrawingArea
{
public:
	Drawing(Simulation sim);
	virtual ~Drawing();
	void reset_drawing(Simulation sim);
protected:
	Simulation simulation;
	void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
	void adjustFrame(int width, int height, Frame default_frame);
	Frame frame;
};

class Gui : public Gtk::Window
{
public:
	Gui(Simulation sim);

protected:
	void on_button_clicked_exit();
	void on_button_clicked_open();
	void on_button_clicked_save();
	void on_button_clicked_start();
	void on_button_clicked_step();
	
	void reset_label(Simulation sim);
	bool on_timeout();
	void on_file_dialog_response_open(int response_id, Gtk::FileChooserDialog* dialog);
	void on_file_dialog_response_save(int response_id, Gtk::FileChooserDialog* dialog);
	bool on_window_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);
	
	Gtk::Box m_Main_Box, m_Information_Box, m_Buttons_Box, m_Text_Box, m_Title_Box,
			 m_Values_Box ;
				
	Gtk::Button m_Button_Exit , m_Button_Open, m_Button_Save, m_Button_Start, 
				m_Button_Step ;
	Gtk::Label Title_label, MAJ_label, Part_label, RRS_label, RRR_label, RNS_label,
				RNP_label, RND_label, RNR_label;
	Gtk::Label nb_MAJ, nb_part, nb_RRS, nb_RRR, nb_RNS, nb_RNP, nb_RND, nb_RNR ;
	Gtk::Label general_label, info_label ;
	
	Gtk::Frame m_Frame_general, m_Frame_information;
	
	Simulation simulation;
	bool etat=false;
	Drawing draw; 
	const int timeout_value=delta_t*1000; 
	bool disconnect ;
	int val;
	bool keyb_driven_state; 
	
};

Simulation new_sim(std::string filename);

void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr, Frame frame);

#endif 
