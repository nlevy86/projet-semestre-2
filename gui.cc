// Talia Meyer
// Noam Lévy

#include <iostream>
#include "gui.h"

using namespace std;

Gui::Gui(string& size_algae, string& size_corals, string& size_scavengers):
	main_box(Gtk::Orientation::HORIZONTAL,0),
	second_box(Gtk::Orientation::VERTICAL,0),
	button_box(Gtk::Orientation::VERTICAL,2),
	general_box(Gtk::Orientation::HORIZONTAL,2),
	info_box(Gtk::Orientation::VERTICAL,2),
	nombre_box(Gtk::Orientation::HORIZONTAL,0),
	maj_box(Gtk::Orientation::HORIZONTAL,60),
	algue_box(Gtk::Orientation::HORIZONTAL,90),
	cor_box(Gtk::Orientation::HORIZONTAL,88),
	sca_box(Gtk::Orientation::HORIZONTAL,49),
	label_general("General"),
	label_info("info : nombre de ..."),
	label_maj("mise à jour :"),
	label_algue("algues :"),
	label_cor("corails :"),
	label_sca("charognards :"),
	label_nb_maj("gd"),
	label_nb_algue(size_algae),
	label_nb_cor(size_corals),
	label_nb_sca(size_scavengers)
{
	set_title("microrécifs");
	set_default_size(500, 500);
	set_child(main_box);
	main_box.append(second_box);
	second_box.append(button_box);
	second_box.append(info_box);
	button_exit.set_label("                  exit                  ");
	button_open.set_label("open");
	button_save.set_label("save");
	button_start.set_label("start");
	button_step.set_label("step");
	naissance_algue.set_label("Naissance d'algue");
	button_box.append(general_box);
	general_box.append(label_general);
	button_box.append(button_exit);
	button_box.append(button_open);
	button_box.append(button_save);
	button_box.append(button_start);
	button_box.append(button_step);
	button_box.append(naissance_algue);
	button_box.append(separator);
	info_box.append(nombre_box);
	nombre_box.append(label_info);
	info_box.append(maj_box);
	maj_box.append(label_maj);
	maj_box.append(label_nb_maj);
	info_box.append(algue_box);
	algue_box.append(label_algue);
	algue_box.append(label_nb_algue);
	info_box.append(cor_box);
	cor_box.append(label_cor);
	cor_box.append(label_nb_cor);
	info_box.append(sca_box);
	sca_box.append(label_sca);
	sca_box.append(label_nb_sca);
	main_box.append(graph);
	button_exit.signal_clicked().connect(sigc::mem_fun(*this,
				&Gui::on_button_clicked_exit) );
	button_open.signal_clicked().connect(sigc::mem_fun(*this,
				&Gui::on_button_clicked_open) );
}

void Gui::on_button_clicked_exit(){
	exit(EXIT_SUCCESS);
}

void Gui::on_button_clicked_open(){
	Simulation sim;
	string fichier;
	cin>>fichier;
	sim.lecture(fichier);
	string size_algae(sim.get_size_algae());
	string size_corals(sim.get_size_corals());
	string size_scavengers(sim.get_size_scavengers());
	label_nb_algue.set_text(size_algae);
	label_nb_cor.set_text(size_corals);
	label_nb_sca.set_text(size_scavengers);
}

void Gui::on_button_clicked_save(){
}

void Gui::on_button_clicked_start(){
}

void Gui::on_button_clicked_step(){
}

bool Gui::activate_naissance_algue(){
	return naissance_algue.get_active();
}


