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
    void lecture(const std::string& nom_fichier, bool is_testing);
    void file_writing(std::string filename);
    const std::string get_size_algae();
    const std::string get_size_corals();
    const std::string get_size_scavengers();
    void maj(bool creation_algue);
    const void dessin(int width, int height);
    const int get_nb_sim();
    void set_nb_sim();
    
private:

    std::vector<Algue> algae {};
    std::vector<Corail> corals {};
    std::vector<Sca> scavengers {};
	
	int nb_sim = 0;
	
	void new_alga(double x, double y, int age, bool& test);
	Corail* new_coral(double x, double y, int age, unsigned int id, 
					  int status_cor, int dir_rot, int status_dev, int nb_seg, 
					  bool& test);
	void new_sca(double x, double y, int age, double rayon, int status_sca, 
				 int target_id, bool& test);
	void new_segment(double angle, double length, Corail *current, bool& test);
    bool id_match(unsigned int tested_id, bool& test);
    const int algue_proche(size_t i);
    void destruction_algue(int j);
    const double calcul_angle(size_t i, int j);
    const unsigned int creation_id();
    void apparition_aleatoire_algue();
    void super_maj_cor(size_t i);
    int not_intersec_others(size_t k, S2d fin);
    void batterie_tests(size_t i,double vieil_angle);
    
};

#endif
