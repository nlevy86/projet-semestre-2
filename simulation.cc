// Talia Meyer
// Noam Lévy

#include "simulation.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
	

void Simulation::lecture(const string& nom_fichier) {
    ifstream fichier(nom_fichier.c_str());
    Section section = Section::NONE;
    string buffer{};
    int nb{};
    while(getline(fichier, buffer)){
        if (buffer[0] == '#' or buffer.size() < 1){
			continue;
        }
        istringstream ligne(buffer);
		switch(section) {
        case NONE:
            ligne >> nb;
            section = Section::ALGA;
            break; 
        case ALGA:
            if(nb > 0){
                double x(0), y(0);
                int age = 0;
                ligne >> x >> y >> age;
                new_alga(x, y, age);
                --nb;
            } else {
                ligne >> nb;
                section = Section::CORAL;
            }
            break;
        case CORAL:
            if(nb > 0){
                double x(0), y(0);
                int age(0), status_cor(0),dir_rot(0), status_dev(0),nb_seg(0);
                unsigned int id = 0;
				ligne >> x >> y >> age >> id >> status_cor >> dir_rot >> status_dev 
				>> nb_seg;
                Corail *current = new_coral(x, y, age, id, status_cor, 
														dir_rot, status_dev, nb_seg);               
                for(int i(0); i < nb_seg; ++i) {
                    getline(fichier, buffer);                    
                    if (buffer[0] == '#' or buffer.empty()){
                        continue;
                    }
					istringstream ligne_seg(buffer);
                    double angle(0), len(0);
                    ligne_seg >> angle >> len;

                    new_segment(angle, len, current);
                }
				corals.back().corail_in();
				corals.back().segment_length_in();
				corals.back().segment_angle_in();
				corals.back().segment_not_superpo(false);
				corals.back().segment_not_coll_him(false);
				--nb;
            } else {
                ligne >> nb;
                section = Section::SCAVENGER;
            }
            break;
        case SCAVENGER:
            if(nb > 0){
                double x(0), y(0), rayon(0);
                int age(0), status_sca(0), target_id(-1);
                ligne >> x >> y >> age >> rayon >> status_sca;
                if(status_sca){
                    ligne >> target_id;    
                }
                new_sca(x, y, age, rayon, status_sca, target_id);               
                --nb;
            }
            break;
        default:
            break;
        }
    }
    fichier.close();
}




void Simulation::new_alga(double x, double y, int age){
    S2d pos{};
    pos.x = x;
    pos.y = y;
    algae.emplace_back(pos, age);
    algae.back().lifeform_in();
    algae.back().positive_age();
    
}

void Simulation::new_sca(double x, double y, int age,
	double rayon, int status_sca, int target_id){
    S2d pos{};
    pos.x = x;
    pos.y = y;
    scavengers.emplace_back(pos, age, rayon, status_sca ? EATING : FREE, target_id);
	scavengers.back().lifeform_in();
    scavengers.back().positive_age();
    scavengers.back().ray_in();

    if(scavengers.back().get_id_cible() >= 0){
        unsigned int pos_target_id(scavengers.back().get_id_cible()); //implicit cast
        if(!id_match(pos_target_id)){
            cout << message::lifeform_invalid_id(pos_target_id);
            exit(EXIT_FAILURE);
        }
    }

}

void Simulation::new_segment(double age, double length, Corail *current){
    current->add_segment(age, length);
    S2d fin_c{};
	size_t cor_size(current->get_cor_size());
    fin_c.x = current->get_cor_element(cor_size -1).base.x + 
		current->get_cor_element(cor_size -1).longueur * 
		cos(current->get_cor_element(cor_size -1).angle);
	fin_c.y = current->get_cor_element(cor_size -1).base.y + 
		current->get_cor_element(cor_size -1).longueur * 
		sin(current->get_cor_element(cor_size -1).angle);

    for(size_t i(0); i < corals.size() - 1; ++i){ 
       for(size_t j(0); j < corals[i].get_cor_size(); ++j){ 
			S2d fin_j{};
			fin_j.x = corals[i].get_cor_element(j).base.x + 
					corals[i].get_cor_element(j).longueur * 
					cos(corals[i].get_cor_element(j).angle);
					
			fin_j.y = corals[i].get_cor_element(j).base.y + 
					corals[i].get_cor_element(j).longueur * 
					sin(corals[i].get_cor_element(j).angle);
					
            if(do_intersect(current->get_cor_element(cor_size - 1).base, 
				fin_c, corals[i].get_cor_element(j).base, fin_j, false)){
                cout << message::segment_collision(current->get_cor_id(), 
					(cor_size - 1), corals[i].get_cor_id(), j);
                exit(EXIT_FAILURE);
            }
       } 
    }
}

Corail* Simulation::new_coral(double x, double y, int age, unsigned int id, 
						int status_cor, int dir_rot, int status_dev, int nb_seg){
    S2d pos{};
    pos.x = x;
    pos.y = y;

    corals.emplace_back(pos, age, id, status_cor ? ALIVE : DEAD, 
					dir_rot ? INVTRIGO : TRIGO, status_dev ? REPRO : EXTEND, nb_seg);
	
	corals.back().center_corail();
	corals.back().positive_age();

	if(id_match(corals.back().get_cor_id())){
        cout << message::lifeform_duplicated_id(corals.back().get_cor_id());
        exit(EXIT_FAILURE);
    }
    return &(corals.back());
}


bool Simulation::id_match(unsigned int tested_id){
    for(size_t i{0}; i < corals.size() - 1; ++i){
        if(corals[i].get_cor_id() == tested_id){
            return true;
        }
    }
    return false;
}
