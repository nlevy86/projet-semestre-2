// Talia Meyer
// Noam Lévy

#include <iostream>
#include "gui.h"
#include <gtkmm/application.h>

int main(int argc, char *argv[]){
	
	Simulation sim;
	if (argc != 2){
		return 1;
	}
	
	sim.reinit();
	sim.lecture(std::string{argv[1]});
	
	auto app = Gtk::Application::create();
	return app-> make_window_and_run<Gui>(1, argv, sim);
}
