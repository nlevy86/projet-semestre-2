#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include "lifeform.h"

enum Section { ALGA, CORAL, SCAVENGER, NONE };

class Simulation {
public:
    void lecture(const std::string& nom_fichier); 
private:

    std::vector<Algue> algae {};
    std::vector<Corail> corals {};
    std::vector<Sca> scavengers {};
	
    void new_alga(double x, double y, int age);
    Corail* new_coral(double x, double y, int age, unsigned int id, 
							int status_cor, int dir_rot, int status_dev, int nb_seg);
    void new_sca(double x, double y, int age, double rayon, int status_sca, 
																		int target_id);
    void new_segment(double angle, double length, Corail *current);
    bool id_match(unsigned int tested_id);
};

#endif
