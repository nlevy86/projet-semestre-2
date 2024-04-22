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
	
	std::string size_algae(sim.get_size_algae());
	std::string size_corals(sim.get_size_corals());
	std::string size_scavengers(sim.get_size_scavengers());
	
	auto app = Gtk::Application::create();
	return app-> make_window_and_run<Gui>(1, argv, size_algae, size_corals, size_scavengers);
}
