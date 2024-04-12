#include <gtkmm/application.h>
#include "gui.h"
#include "simulation.h"
#include <cairomm/context.h>
#include <gtkmm/label.h>
#include <gtkmm.h>
#include <iostream>
#include <string>

using namespace std;

constexpr unsigned taille_dessin(500);

int gui(int argc, char** argv, Simulation simulation){
	auto app = Gtk::Application::create();
	if (simulation.simulation_ok==false) { 
		Simulation simulation;
		return app->make_window_and_run<Gui>(argc, argv, simulation);
	}
	return app->make_window_and_run<Gui>(argc, argv, simulation);
}

Drawing::Drawing(Simulation sim){
	simulation=sim;
	set_content_width(taille_dessin);
	set_content_height(taille_dessin);
	set_draw_func(sigc::mem_fun(*this, &Drawing::on_draw));
}

void Drawing::reset_drawing(Simulation sim){
	simulation=sim;
	set_content_width(taille_dessin);
	set_content_height(taille_dessin);
	set_draw_func(sigc::mem_fun(*this, &Drawing::on_draw));
}

Drawing::~Drawing() {}

void Drawing::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height){
	static Frame default_frame = {-dmax, dmax, -dmax, dmax, 1, width, height}; 
	adjustFrame(width, height, default_frame);
	orthographic_projection(cr, frame);
	graphic_set_context(cr);
	draw_frame(frame);
	simulation.draw_sim();
	cr->stroke();
}

void Drawing::adjustFrame(int width, int height, Frame default_frame){
	frame.width  = width;
	frame.height = height;

    double new_aspect_ratio((double)width/height);
    if( new_aspect_ratio > default_frame.asp){ 
	    frame.yMax = default_frame.yMax ;
	    frame.yMin = default_frame.yMin ;	
	  
	    double delta(default_frame.xMax - default_frame.xMin);
	    double mid((default_frame.xMax + default_frame.xMin)/2);
	    frame.xMax = mid + 0.5*(new_aspect_ratio/default_frame.asp)*delta ;
	    frame.xMin = mid - 0.5*(new_aspect_ratio/default_frame.asp)*delta ;		  	  
    }
    else { 
	    frame.xMax = default_frame.xMax ;
	    frame.xMin = default_frame.xMin ;
	  	  
 	    double delta(default_frame.yMax - default_frame.yMin);
	    double mid((default_frame.yMax + default_frame.yMin)/2);
	    frame.yMax = mid + 0.5*(default_frame.asp/new_aspect_ratio)*delta ;
	    frame.yMin = mid - 0.5*(default_frame.asp/new_aspect_ratio)*delta ;		  	  
    }
}

void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr, Frame frame){
	cr->translate(frame.width/2, frame.height/2);
	cr->scale(frame.width/(frame.xMax - frame.xMin), 
             -frame.height/(frame.yMax - frame.yMin));
	cr->translate(-(frame.xMin + frame.xMax)/2, -(frame.yMin + frame.yMax)/2);
}

Gui::Gui(Simulation sim):
	m_Main_Box(Gtk::Orientation::HORIZONTAL, 0), 
	m_Information_Box(Gtk::Orientation::VERTICAL, 4),
	m_Buttons_Box(Gtk::Orientation::VERTICAL, 0),   
	m_Text_Box(Gtk::Orientation::HORIZONTAL, 100),
	m_Title_Box(Gtk::Orientation::VERTICAL, 0),
	m_Values_Box(Gtk::Orientation::VERTICAL, 0),
	m_Button_Exit("exit"),
	m_Button_Open("open"),
	m_Button_Save("save"),
	m_Button_Start("start"),
	m_Button_Step("step"),
	MAJ_label  ("mises à jour :                                          "),
	Part_label ("particules:                                               "), 
	RRS_label  ("robots réparateurs en service:   "),
	RRR_label  ("robots réparateurs en réserve:  "),
	RNS_label  ("robots neutraliseurs en service:"),
	RNP_label  ("robots neutraliseurs en panne:  "), 
	RND_label  ("robots neutraliseurs détruits:     "), 
	RNR_label  ("robots neutraliseurs en réserve:"),
	nb_MAJ(std::to_string(sim.get_nbMaj())),
	nb_part(std::to_string(sim.get_nbP())),
	nb_RRS(std::to_string(sim.get_nbRs())), 
	nb_RRR(std::to_string(sim.get_nbRr())), 
	nb_RNS(std::to_string(sim.get_nbNs())), 
	nb_RNP("0"), 
	nb_RND(std::to_string(sim.get_nbNd())), 
	nb_RNR(std::to_string(sim.get_nbNr())),
	draw(sim) {
		
	simulation=sim ;
	set_title("Propre En Ordre");
	set_child(m_Main_Box);	
	m_Frame_general.set_label("General") ;
	m_Frame_general.set_child(m_Buttons_Box);	
	m_Frame_information.set_label("Info: nombre de ... ");
	m_Frame_information.set_child(m_Text_Box);	
	m_Main_Box.append(m_Information_Box);
	m_Main_Box.append(draw);	
	draw.set_expand();	
	m_Information_Box.append(m_Frame_general);
	m_Information_Box.append(m_Frame_information);	
	m_Buttons_Box.append(m_Button_Exit);
	m_Buttons_Box.append(m_Button_Open);  
	m_Buttons_Box.append(m_Button_Save);
	m_Buttons_Box.append(m_Button_Start);   
	m_Buttons_Box.append(m_Button_Step); 	  
	m_Text_Box.append(m_Title_Box);
	m_Text_Box.append(m_Values_Box); 
	m_Title_Box.append(MAJ_label); m_Values_Box.append(nb_MAJ);
	m_Title_Box.append(Part_label); m_Values_Box.append(nb_part);
	m_Title_Box.append(RRS_label); m_Values_Box.append(nb_RRS); 
	m_Title_Box.append(RRR_label); m_Values_Box.append(nb_RRR);
	m_Title_Box.append(RNS_label); m_Values_Box.append(nb_RNS);
	m_Title_Box.append(RNP_label); m_Values_Box.append(nb_RNP);
	m_Title_Box.append(RND_label); m_Values_Box.append(nb_RND);
	m_Title_Box.append(RNR_label); m_Values_Box.append(nb_RNR);
	val=simulation.get_nbMaj();			
	m_Button_Exit.signal_clicked().connect(
		sigc::mem_fun(*this, &Gui::on_button_clicked_exit));
    m_Button_Open.signal_clicked().connect(
		sigc::mem_fun(*this, &Gui::on_button_clicked_open));
	m_Button_Save.signal_clicked().connect(
		sigc::mem_fun(*this, &Gui::on_button_clicked_save));
    m_Button_Start.signal_clicked().connect(
		sigc::mem_fun(*this, &Gui::on_button_clicked_start));
	m_Button_Step.signal_clicked().connect(
		sigc::mem_fun(*this, &Gui::on_button_clicked_step));
	auto controller = Gtk::EventControllerKey::create();
    controller->signal_key_pressed().connect(
                  sigc::mem_fun(*this, &Gui::on_window_key_pressed), false);
    add_controller(controller);
}

void Gui::on_button_clicked_open(){
	if (not etat){
		auto dialog = new Gtk::FileChooserDialog("Please choose a file",
			  Gtk::FileChooser::Action::OPEN);
		dialog->set_transient_for(*this);
		dialog->set_modal(true);
		dialog->signal_response().connect(sigc::bind(
		sigc::mem_fun(*this, &Gui::on_file_dialog_response_open), dialog));

		dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
		dialog->add_button("_Open", Gtk::ResponseType::OK);
		
		
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
		
		dialog->show();
	}
}

void Gui::on_button_clicked_exit(){
	hide();
}

void Gui::on_button_clicked_save(){
	if (not etat and (simulation.simulation_ok)){
		auto dialog = new Gtk::FileChooserDialog("Please choose a file",
			  Gtk::FileChooser::Action::SAVE);
		dialog->set_transient_for(*this);
		dialog->set_modal(true);
		dialog->signal_response().connect(sigc::bind(
		sigc::mem_fun(*this, &Gui::on_file_dialog_response_save), dialog));
		
		dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
		dialog->add_button("_SAVE", Gtk::ResponseType::OK);
		
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
		
		dialog->show();
	}
}

void Gui::on_button_clicked_start(){
	if(simulation.simulation_ok){
		if (not etat) {
			m_Button_Start.set_label("stop") ;
			etat=true ;
			sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this,
									      &Gui::on_timeout));
			auto conn = Glib::signal_timeout().connect(my_slot,timeout_value);
		}
		else {
			m_Button_Start.set_label("start");
			disconnect = true;   
			etat=false ;
		}
	}
}

void Gui::on_button_clicked_step(){
	if ((not etat) and (simulation.simulation_ok)) { 
		++val; 
		simulation.desintegration_part();
		simulation.update_simulation(val);
		reset_label(simulation);
		draw.reset_drawing(simulation);
	}
}

bool Gui::on_timeout(){
	if(disconnect)
	{
		disconnect = false; 
		return false; 
	}
	++val;
	simulation.desintegration_part();
	simulation.update_simulation(val);
	reset_label(simulation);
	draw.reset_drawing(simulation);
	return true; 
}

void Gui::on_file_dialog_response_open(int response_id,Gtk::FileChooserDialog* dialog){
	switch (response_id){
		case Gtk::ResponseType::OK:{
		    auto filename = dialog->get_file()->get_path();
			simulation=new_sim(filename);
			val=simulation.get_nbMaj();
			reset_label(simulation);
			draw.reset_drawing(simulation);
		    break;
		}
		case Gtk::ResponseType::CANCEL:{
		    break;
		}
		default:{
		    std::cout << "Unexpected button clicked." << std::endl;
		    break;
		}
	}
	dialog->hide();
}

void Gui::on_file_dialog_response_save(int response_id,Gtk::FileChooserDialog* dialog){
	switch (response_id){
		case Gtk::ResponseType::OK:{
		    auto filename = dialog->get_file()->get_path();
		    simulation.write_file(filename);
		    break;
		}
		case Gtk::ResponseType::CANCEL:{
		    break;
		}
		default:{
		    std::cout << "Unexpected button clicked." << std::endl;
		    break;
		}
	}
	dialog->hide();
}

bool Gui::on_window_key_pressed(guint keyval, guint, Gdk::ModifierType state){
	switch(gdk_keyval_to_unicode(keyval)){
		case 's':
			on_button_clicked_start();
			return true;
		case '1':
			on_button_clicked_step();
			return true;
	}
    return false;
}

Simulation new_sim(string filename) {
	Simulation sim ;
    char* ptr=new char[filename.size()+1];
	strcpy(ptr, filename.c_str());
	sim.lecture(ptr);
	if (not sim.simulation_ok) {
		Simulation simulation;
		return simulation;
	}
	else {return sim;}
}

void Gui::reset_label(Simulation sim){
	nb_MAJ.set_text(std::to_string(sim.get_nbMaj()));
	nb_part.set_text(std::to_string(sim.get_nbP()));
	nb_RRS.set_text(std::to_string(sim.get_nbRs()));
	nb_RRR.set_text(std::to_string(sim.get_nbRr()));
	nb_RNS.set_text(std::to_string(sim.get_nbNs())); 
	nb_RNP.set_text("0"); 
	nb_RND.set_text(std::to_string(sim.get_nbNd())); 
	nb_RNR.set_text(std::to_string(sim.get_nbNr()));
}
