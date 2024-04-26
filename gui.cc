// Talia Meyer
// Noam Lévy

#include <iostream>
#include "gui.h"

using namespace std;

static Frame default_frame = {0, 256, 0, 256, 1, 500, 500}; 

static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr, 
														Frame frame);

Gui::Gui(Simulation sim):
	simulation(sim),
	draw(&simulation),
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
	label_nb_maj(to_string(compteur_maj)),
	label_nb_algue(simulation.get_size_algae()),
	label_nb_cor(simulation.get_size_corals()),
	label_nb_sca(simulation.get_size_scavengers())
{
	set_title("Microrécif");
	set_default_size(500, 500);
	set_child(main_box);
	main_box.append(second_box);
	main_box.append(separator3);
	draw.set_vexpand(true);
	draw.set_hexpand(true);
	main_box.append(draw);
	second_box.append(button_box);
	second_box.append(info_box);
	button_exit.set_label("exit");
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
	button_box.append(separator1);
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
	info_box.append(separator2);
	button_exit.signal_clicked().connect(sigc::mem_fun(*this,
				&Gui::on_button_clicked_exit) );
	button_open.signal_clicked().connect(sigc::mem_fun(*this,
				&Gui::on_button_clicked_open) );
	button_save.signal_clicked().connect(sigc::mem_fun(*this,
				&Gui::on_button_clicked_save) );
	button_start.signal_clicked().connect(sigc::mem_fun(*this,
				&Gui::on_button_clicked_start) );
	button_step.signal_clicked().connect(sigc::mem_fun(*this,
				&Gui::on_button_clicked_step) );
	naissance_algue.signal_toggled().connect(sigc::mem_fun(*this,
				&Gui::activate_naissance_algue) );
	auto controller = Gtk::EventControllerKey::create();
    controller->signal_key_pressed().connect(
                  sigc::mem_fun(*this, &Gui::on_window_key_pressed), false);
    add_controller(controller);
}

void Gui::on_button_clicked_exit(){
	hide();
}

void Gui::on_button_clicked_open(){
	is_save = false;
	auto dialog = new Gtk::FileChooserDialog("Please choose a file",
	Gtk::FileChooser::Action::OPEN);
	dialog->set_transient_for(*this);
	dialog->set_modal(true);
	dialog->signal_response().connect(sigc::bind(
	sigc::mem_fun(*this, &Gui::on_file_dialog_response), dialog));
	
	//Add response buttons to the dialog:
	dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
	dialog->add_button("_Open", Gtk::ResponseType::OK);
	
	//Add filters, so that only certain file types can be selected:
	
	auto filter_text = Gtk::FileFilter::create();
	filter_text->set_name("Text files");
	filter_text->add_mime_type("text/plain");
	dialog->add_filter(filter_text);
	
	auto filter_cpp = Gtk::FileFilter::create();
	filter_cpp->set_name("C/C++ files");
	filter_cpp->add_mime_type("text/x-c");
	filter_cpp->add_mime_type("text/x-c++");
	filter_cpp->add_mime_type("text/x-c-header");
	dialog->add_filter(filter_cpp);
	
	auto filter_any = Gtk::FileFilter::create();
	filter_any->set_name("Any files");
	filter_any->add_pattern("*");
	dialog->add_filter(filter_any);
	
	//Show the dialog and wait for a user response:
	dialog->show();
}

void Gui::on_button_clicked_save(){
	is_save = true;
	auto dialog = new Gtk::FileChooserDialog("Please choose a file",
	Gtk::FileChooser::Action::SAVE);
	dialog->set_transient_for(*this);
	dialog->set_modal(true);
	dialog->signal_response().connect(sigc::bind(
	sigc::mem_fun(*this, &Gui::on_file_dialog_response), dialog));
	
	//Add response buttons to the dialog:
	dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
	dialog->add_button("_Save", Gtk::ResponseType::OK);
	
	//Add filters, so that only certain file types can be selected:
	
	auto filter_text = Gtk::FileFilter::create();
	filter_text->set_name("Text files");
	filter_text->add_mime_type("text/plain");
	dialog->add_filter(filter_text);
	
	auto filter_cpp = Gtk::FileFilter::create();
	filter_cpp->set_name("C/C++ files");
	filter_cpp->add_mime_type("text/x-c");
	filter_cpp->add_mime_type("text/x-c++");
	filter_cpp->add_mime_type("text/x-c-header");
	dialog->add_filter(filter_cpp);
	
	auto filter_any = Gtk::FileFilter::create();
	filter_any->set_name("Any files");
	filter_any->add_pattern("*");
	dialog->add_filter(filter_any);
	
	//Show the dialog and wait for a user response:
	dialog->show();
}

bool Gui::start_simulation(){
	if (simulation_running){
		button_start.set_label("stop");
		simulation.maj(apparition_algue);
		++compteur_maj;
		label_nb_maj.set_text(to_string(compteur_maj));
		label_nb_algue.set_text(simulation.get_size_algae());
		label_nb_cor.set_text(simulation.get_size_corals());
		label_nb_sca.set_text(simulation.get_size_scavengers());
		draw.refresh();
		return true;
	} else { 
		button_start.set_label("start");
		return false;
	}
}

void Gui::on_button_clicked_step(){
	simulation.maj(apparition_algue);
	++compteur_maj;
	label_nb_maj.set_text(to_string(compteur_maj));
	label_nb_algue.set_text(simulation.get_size_algae());
	label_nb_cor.set_text(simulation.get_size_corals());
	label_nb_sca.set_text(simulation.get_size_scavengers());
	draw.refresh();
}

bool Gui::on_window_key_pressed(guint keyval, guint, Gdk::ModifierType state)
{
	switch(gdk_keyval_to_unicode(keyval))
	{
		case '1':
			on_button_clicked_step();
			return true;
		case 's':
			on_button_clicked_start();
			return true;
	}
    return false;
}

void Gui::activate_naissance_algue(){
	apparition_algue = naissance_algue.get_active();
}

void Gui::on_button_clicked_start(){
	if (not simulation_running){
		simulation_running = true;
		sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this,
		                                        &Gui::start_simulation));
		auto conn = Glib::signal_timeout().connect(my_slot,25);
	} else {
		simulation_running = false;
	}
}

void Gui::on_file_dialog_response(int response_id, Gtk::FileChooserDialog* dialog)
{
	//Handle the response:
	switch (response_id)
	{
		case Gtk::ResponseType::OK:
		{
			auto filename = dialog->get_file()->get_path();
			if (is_save){
				simulation.file_writing(filename);
			} else {
				simulation.reinit();
				simulation.lecture(filename);
				label_nb_maj.set_text("0");
				label_nb_algue.set_text(simulation.get_size_algae());
				label_nb_cor.set_text(simulation.get_size_corals());
				label_nb_sca.set_text(simulation.get_size_scavengers());
				draw.refresh();
			}
		    break;
		}
		case Gtk::ResponseType::CANCEL:
		{
		    break;
		}
		default:
		{
		    break;
		}
	}
	delete dialog;
}

Drawing::Drawing(Simulation* simptr):
	sim(simptr)
{
	set_content_width(500);
	set_content_height(500);
	set_draw_func(sigc::mem_fun(*this, &Drawing::on_draw));
}

void Drawing::setFrame(Frame f)
{
	if((f.xMin <= f.xMax) and (f.yMin <= f.yMax) and (f.height > 0))
	{
		f.asp = f.width/f.height;
		frame = f;
	}
} 

void Drawing::adjustFrame(int width, int height)
{
	frame.width  = width;
	frame.height = height;

	// Preventing distorsion by adjusting the frame (cadrage)
	// to have the same proportion as the graphical area
	
    // use the reference framing as a guide for preventing distortion
    double new_aspect_ratio((double)width/height);
    if( new_aspect_ratio > default_frame.asp)
    { // keep yMax and yMin. Adjust xMax and xMin
	    frame.yMax = default_frame.yMax ;
	    frame.yMin = default_frame.yMin ;	
	  
	    double delta(default_frame.xMax - default_frame.xMin);
	    double mid((default_frame.xMax + default_frame.xMin)/2);
        // the new frame is centered on the mid-point along X
	    frame.xMax = mid + 0.5*(new_aspect_ratio/default_frame.asp)*delta ;
	    frame.xMin = mid - 0.5*(new_aspect_ratio/default_frame.asp)*delta ;		  	  
    }
    else
    { // keep xMax and xMin. Adjust yMax and yMin
	    frame.xMax = default_frame.xMax ;
	    frame.xMin = default_frame.xMin ;
	  	  
 	    double delta(default_frame.yMax - default_frame.yMin);
	    double mid((default_frame.yMax + default_frame.yMin)/2);
        // the new frame is centered on the mid-point along Y
	    frame.yMax = mid + 0.5*(default_frame.asp/new_aspect_ratio)*delta ;
	    frame.yMin = mid - 0.5*(default_frame.asp/new_aspect_ratio)*delta ;		  	  
    }
}




void Drawing::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
	graphic_set_context(cr);
		// adjust the frame (cadrage) to prevent distortion 
	adjustFrame(width, height);
		
	draw_frame(width, height);// drawing the drawingArea space
	
	orthographic_projection(cr, frame); // set the transformation MODELE to GTKmm
		
	sim->dessin(width, height);
	
}

void Drawing::refresh(){
	queue_draw();
}

static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr, 
								    Frame frame)
{
	// déplace l'origine au centre de la fenêtre
	cr->translate(frame.width/2, frame.height/2);
  
	// normalise la largeur et hauteur aux valeurs fournies par le cadrage
	// ET inverse la direction de l'axe Y
	cr->scale(frame.width/(frame.xMax - frame.xMin), 
             -frame.height/(frame.yMax - frame.yMin));
  
	// décalage au centre du cadrage
	cr->translate(-(frame.xMin + frame.xMax)/2, -(frame.yMin + frame.yMax)/2);
}
