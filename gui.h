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


class Gui : public Gtk::Window {
public:
	Gui(std::string& size_algae, std::string& size_cor, std::string& size_sca);
	bool activate_naissance_algue();	
protected:
	void on_button_clicked_exit();
	void on_button_clicked_open();
	void on_button_clicked_save();
	void on_button_clicked_start();
	void on_button_clicked_step();
	Gtk::Box main_box, second_box, button_box, general_box, info_box, nombre_box,
	maj_box, algue_box, cor_box, sca_box;
	Graphic graph;
	Gtk::Button button_exit;
	Gtk::Button button_open;
	Gtk::Button button_save;
	Gtk::Button button_start;
	Gtk::Button button_step;
	Gtk::CheckButton naissance_algue;
	Gtk::Label label_general, label_info, label_maj, label_algue, label_cor, label_sca,
	label_nb_maj, label_nb_algue, label_nb_cor, label_nb_sca;
	Gtk::Separator separator;
};

#endif
