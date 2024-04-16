// Talia Meyer
// Noam Lévy

#include <iostream>
#include "gui.h"

using namespace std;



int main(int argc, char *argv[]){

	Simulation sim;

	if (argc != 2){
		return 1;
	}
	sim.reinit();
	sim.lecture(string{argv[1]});
	cout << message::success();
	exit(EXIT_SUCCESS);
	
	return 0;
}
