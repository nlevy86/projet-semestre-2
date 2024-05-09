// Talia Meyer
// Noam Lévy

#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include "lifeform.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <locale>

enum Section { ALGA, CORAL, SCAVENGER, NONE };

class Simulation {
public:
	void reinit();
    void lecture(const std::string& nom_fichier);
    void file_writing(std::string filename);
    const std::string get_size_algae();
    const std::string get_size_corals();
    const std::string get_size_scavengers();
    void maj(bool creation_algue);
    const void dessin(int width, int height);
    
private:

    std::vector<Algue> algae {};
    std::vector<Corail> corals {};
    std::vector<Sca> scavengers {};
	
	void new_alga(double x, double y, int age, bool& test);
	Corail* new_coral(double x, double y, int age, unsigned int id, 
					  int status_cor, int dir_rot, int status_dev, int nb_seg, 
					  bool& test);
	void new_sca(double x, double y, int age, double rayon, int status_sca, 
				 int target_id, bool& test);
	void new_segment(double angle, double length, Corail *current, bool& test);
    bool id_match(unsigned int tested_id, bool& test);
    const int algue_proche(int i);
};

#endif
