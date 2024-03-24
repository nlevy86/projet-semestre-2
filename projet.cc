#include <iostream>
#include "shape.h"

using namespace std;



int main(int argc, char *argv[]){

	Simulation sim;

	if (argc != 2){
		return 1;
	}
	sim.lecture(string{argv[1]});	
		
	return 0;
}
