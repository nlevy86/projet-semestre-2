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
    void lecture(const std::string& nom_fichier, bool is_testing, bool epsil_zero, 
				 bool output);
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
	
	void new_alga(double x, double y, int age, bool& test, bool output);
	Corail* new_coral(double x, double y, int age, unsigned int id, 
					  int status_cor, int dir_rot, int status_dev, int nb_seg, 
					  bool& test, bool output);
	void new_sca(double x, double y, int age, double rayon, int status_sca, 
				 int target_id, bool& test, bool output);
	void new_segment(double angle, double length, Corail *current, bool& test, 
					 bool output);
    bool id_match(unsigned int tested_id, bool& test);
    const int algue_proche(size_t i);
    void destruction_algue(int j);
    const double calcul_angle(size_t i, int j);
    const unsigned int creation_id();
    void apparition_aleatoire_algue();
    void super_maj_cor(size_t i);
    bool not_intersec_others(size_t k, S2d fin);
    void batterie_tests(size_t i,double vieil_angle, bool& test, 
                        std::vector <bool>& coll);
    void free_sca_creation (std::vector<Sca*> &free_scavengers);
    void dead_free_corals_creation (std::vector <Corail> &dead_free_corals);
    

    void sca_target_attribution(std::vector <Corail> &dead_free_corals, 
                                std::vector <Sca*> &free_scavengers);

    double calc_dist_coral_sca(size_t i, size_t j, 
                               std::vector<Corail> dead_free_corals, 
                               std::vector<Sca*> free_scavengers);
    void destruction_sca(int j);
    void super_maj_sca();
    Corail& find_target(Sca scavenger);
    double dist_calc(Sca scavenger, Corail coral);
    void suppr_cor(Sca scavenger);
    
};

#endif
