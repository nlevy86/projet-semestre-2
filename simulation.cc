// Talia Meyer
// Noam Lévy

#include "simulation.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

using namespace std;

static default_random_engine e;

void Simulation::reinit(){
	if (algae.size()>0){
		algae.clear();
	}
	if (scavengers.size()>0){
		scavengers.clear();
	}
	if (corals.size()>0){
		corals.clear();
	}
}

void Simulation::lecture(const string& nom_fichier) {
	bool test = true;
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
			cout<<1;
            if(nb > 0){
                double x(0), y(0);
                int age = 0;
                ligne >> x >> y >> age;
                new_alga(x, y, age, test);
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
												dir_rot, status_dev, nb_seg, test);               
                for(int i(0); i < nb_seg; ++i) {
                    getline(fichier, buffer);                    
                    if (buffer[0] == '#' or buffer.empty()){
                        continue;
                    }
					istringstream ligne_seg(buffer);
                    double angle(0), len(0);
                    ligne_seg >> angle >> len;

                    new_segment(angle, len, current, test);
                }
				test = test and corals.back().corail_in();
				test = test and corals.back().segment_length_in();
				test = test and corals.back().segment_angle_in();
				test = test and corals.back().segment_not_superpo(false);
				test = test and corals.back().segment_not_coll_him(false);
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
                new_sca(x, y, age, rayon, status_sca, target_id, test);               
                --nb;
            }
            break;
        default:
            break;
        }
    }
    fichier.close();
    if (not test){
    	reinit();
    }

}

void Simulation::file_writing(const string& filename){
    ofstream file(filename);

    if (file.fail()){
        exit(1);
    }

    file << "# Etat courant de la simulation :" << endl;
    file << algae.size() << endl;

    for (size_t i(0); i < algae.size(); ++i) {
        file << "\t" << algae[i].get_lifeform_pos().x << " " 
            << algae[i].get_lifeform_pos().y << " " 
            << algae[i].get_lifeform_age() << endl;
    }

    file << endl;
    file << corals.size() << endl;
    
    for (size_t i(0); i < corals.size(); ++i) {
        file << "\t" << corals[i].get_lifeform_pos().x << " " 
        << corals[i].get_lifeform_pos().y << " " << corals[i].get_lifeform_age() 
        << " " << corals[i].get_cor_id() << " " << corals[i].get_cor_status() 
        << " " << corals[i].get_cor_dir() << " " << corals[i].get_cor_dev() 
        << " " << corals[i].get_cor_size() << endl;

        for (size_t j(0); j < corals[i].get_cor_size(); ++j) {
            file << "\t\t" << corals[i].get_cor_element(j).angle << " "
            << corals[i].get_cor_element(j).longueur << endl;
        }

    }

    file << endl;
    file << scavengers.size() << endl;

    for(size_t i(0); i < scavengers.size(); ++i){
        file << scavengers[i].get_lifeform_pos().x << " " 
        <<scavengers[i].get_lifeform_pos().y << " " <<scavengers[i].get_lifeform_age()
        << " " << scavengers[i].get_ray() << " " << scavengers[i].get_status() 
        << " " << scavengers[i].get_id_cible() << endl;
    }

    file.close();
}



void Simulation::new_alga(double x, double y, int age, bool& test){
    S2d pos{};
    pos.x = x;
    pos.y = y;
    algae.emplace_back(pos, age);
    test = test and algae.back().lifeform_in();
    test = test and algae.back().positive_age();
    
}

void Simulation::new_sca(double x, double y, int age,double rayon,
	int status_sca, int target_id, bool& test){
    S2d pos{};
    pos.x = x;
    pos.y = y;
    scavengers.emplace_back(pos, age, rayon, status_sca ? EATING : FREE, target_id);
	test = test and scavengers.back().lifeform_in();
    test = test and scavengers.back().positive_age();
    test = test and scavengers.back().ray_in();

    if(scavengers.back().get_id_cible() >= 0){
        unsigned int pos_target_id(scavengers.back().get_id_cible()); //implicit cast
        if(!id_match(pos_target_id, test)){
            cout << message::lifeform_invalid_id(pos_target_id);
            test = false;
        }
    }

}

void Simulation::new_segment(double age, double length, Corail *current, bool& test){
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
                test = false;
            }
       } 
    }
}

Corail* Simulation::new_coral(double x, double y, int age, unsigned int id, 
				int status_cor, int dir_rot, int status_dev, int nb_seg, bool& test){
    S2d pos{};
    pos.x = x;
    pos.y = y;

    corals.emplace_back(pos, age, id, status_cor ? ALIVE : DEAD, 
					dir_rot ? INVTRIGO : TRIGO, status_dev ? REPRO : EXTEND, nb_seg);
	
	test = test and corals.back().lifeform_in();
	test = test and corals.back().positive_age();

	if(id_match(corals.back().get_cor_id(), test)){
        cout << message::lifeform_duplicated_id(corals.back().get_cor_id());
        test = false;
    }
    return &(corals.back());
}


bool Simulation::id_match(unsigned int tested_id, bool& test){
    for(size_t i(0); i < corals.size() - 1; ++i){
        if(corals[i].get_cor_id() == tested_id){
            return true;
        }
    }
    return false;
}

void Simulation::maj(bool creation_algue){
	for (size_t i(0); i < algae.size(); ++i){
		if (algae[i].maj_algue()){
			Algue alga(algae[i]);
			algae[i] = algae.back();
			algae.back() = alga;
			algae.pop_back();
			--i;
		}
	}
	if (creation_algue){
		e.seed(1);
		double p(alg_birth_rate);
		bernoulli_distribution b(p);
		if (b(e)){
			uniform_int_distribution<unsigned> u(1,dmax-1);
			bool test (true);
			new_alga(u(e), u(e), 0, test); // vérifier age
		}
	}
}

const string Simulation::get_size_algae(){
	return to_string(algae.size());
}
const string Simulation::get_size_corals(){
	return to_string(corals.size());
}

const string Simulation::get_size_scavengers(){
	return to_string(scavengers.size());
}

