// Talia Meyer 80 %
// Noam Lévy   20 %

#include "simulation.h"


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

//Premiere fonction/methode de plus de 40 lignes
void Simulation::lecture(const string& nom_fichier, bool is_testing, bool epsil_zero)
{
	bool test = true;
	if (is_testing){
		e.seed(1);
	}
    ifstream fichier(nom_fichier.c_str());
    Section section = Section::NONE;
    string buffer{};
    int nb;
    while(getline(fichier, buffer)){
        if (buffer[0] == '#' or buffer.size() < 1) continue;     
        if (buffer.find_first_not_of(' ') == std::string::npos) continue;
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
                new_alga(x, y, age, test);
                --nb;
            } else {
                ligne >> nb;
                section = Section::CORAL;
            }
            break;
        case CORAL:
            if(nb > 0)
            {
                double x(0), y(0);
                int age(0), status_cor(0),dir_rot(0), status_dev(0),nb_seg(0);
                unsigned int id = 0;
				ligne >> x >> y >> age >> id >> status_cor >> dir_rot >> status_dev 
				      >> nb_seg;
                Corail *current = new_coral(x, y, age, id, status_cor, 
											dir_rot, status_dev, nb_seg, test);               
                for(int i(0); i < nb_seg; ++i) 
                {
                    getline(fichier, buffer);                    
                    if (buffer[0] == '#' or buffer.empty()) continue;
					istringstream ligne_seg(buffer);
                    double angle(0), len(0);
                    ligne_seg >> angle >> len;

                    new_segment(angle, len, current, test);
                }
				test = test and corals.back().corail_in();
				test = test and corals.back().segment_length_in();
				test = test and corals.back().segment_angle_in();
				test = test and corals.back().segment_not_superpo(epsil_zero);
				test = test and corals.back().segment_not_coll_him(epsil_zero);
				--nb;
            } else {
                ligne >> nb;
                section = Section::SCAVENGER;
            }
            break;
        case SCAVENGER:
            if(nb > 0)
            {
                double x(0), y(0), rayon(0);
                int age(0), status_sca(0), target_id(-1);
                ligne >> x >> y >> age >> rayon >> status_sca;
                if(status_sca) ligne >> target_id;   
                new_sca(x, y, age, rayon, status_sca, target_id, test);               
                --nb;
            }
            break;
        default:
            break;
        }
    }
    fichier.close();
    if (not test and is_testing){
    	reinit();
    }
}

void Simulation::file_writing(string filename)
{
	ofstream file;
	file.open(filename, ofstream::trunc);
	file.imbue(locale::classic());
    file << "# Etat courant de la simulation :" << endl << algae.size() << endl;

    for (size_t i(0); i < algae.size(); ++i) 
    {
        file << "\t" << algae[i].get_lifeform_pos().x << " " 
            << algae[i].get_lifeform_pos().y << " " 
            << algae[i].get_lifeform_age() << endl;
    }
    file << endl << corals.size() << endl;
    
    for (size_t i(0); i < corals.size(); ++i) 
    {
        file << "\t" << corals[i].get_lifeform_pos().x << " " 
        << corals[i].get_lifeform_pos().y << " " << corals[i].get_lifeform_age() 
        << " " << corals[i].get_cor_id() << " " << corals[i].get_cor_status() 
        << " " << corals[i].get_cor_dir() << " " << corals[i].get_cor_dev() 
        << " " << corals[i].get_cor_size() << endl;
        for (size_t j(0); j < corals[i].get_cor_size(); ++j) 
        {
            file << "\t\t" << corals[i].get_cor_element(j).angle << " "
            << corals[i].get_cor_element(j).longueur << endl;
        }
    }
    file << endl << scavengers.size() <<endl;

    for(size_t i(0); i < scavengers.size(); ++i)
    {
        file << "\t" << scavengers[i].get_lifeform_pos().x << " " 
        <<scavengers[i].get_lifeform_pos().y << " " <<scavengers[i].get_lifeform_age()
        << " " << scavengers[i].get_ray() << " " << scavengers[i].get_status() 
        << " " ;
        if (scavengers[i].get_id_cible() != -1) {file << scavengers[i].get_id_cible();}
        file << endl;
    }
    file.close();
}



void Simulation::new_alga(double x, double y, int age, bool& test)
{
    S2d pos{};
    pos.x = x;
    pos.y = y;
    algae.emplace_back(pos, age);
    test = test and algae.back().lifeform_in();
    test = test and algae.back().positive_age();
    
}

void Simulation::new_sca(double x, double y, int age,double rayon,
	                     int status_sca, int target_id, bool& test)
{
    S2d pos{};
    pos.x = x;
    pos.y = y;
    scavengers.emplace_back(pos, age, rayon, status_sca ? EATING : FREE, target_id);
	test = test and scavengers.back().lifeform_in();
    test = test and scavengers.back().positive_age();
    test = test and scavengers.back().ray_in();

    if(scavengers.back().get_id_cible() >= 0)
    {
        unsigned int pos_target_id(scavengers.back().get_id_cible()); //implicit cast
        if(!id_match(pos_target_id, test)){
            cout << message::lifeform_invalid_id(pos_target_id);
            test = false;
        }
    }

}

void Simulation::new_segment(double age, double length, Corail *current, bool& test)
{
    current->add_segment(age, length);
    S2d fin_c{};
	size_t cor_size(current->get_cor_size());
    fin_c.x = current->get_cor_element(cor_size -1).base.x + 
		      current->get_cor_element(cor_size -1).longueur * 
		      cos(current->get_cor_element(cor_size -1).angle);
		      
	fin_c.y = current->get_cor_element(cor_size -1).base.y + 
		      current->get_cor_element(cor_size -1).longueur * 
		      sin(current->get_cor_element(cor_size -1).angle);

    for(size_t i(0); i < corals.size() - 1; ++i)
    { 
       for(size_t j(0); j < corals[i].get_cor_size(); ++j)
       { 
			S2d fin_j{};
			fin_j.x = corals[i].get_cor_element(j).base.x + 
					  corals[i].get_cor_element(j).longueur * 
					  cos(corals[i].get_cor_element(j).angle);
					
			fin_j.y = corals[i].get_cor_element(j).base.y + 
					  corals[i].get_cor_element(j).longueur * 
					  sin(corals[i].get_cor_element(j).angle);
					
            if(do_intersect(current->get_cor_element(cor_size - 1).base, 
			   fin_c, corals[i].get_cor_element(j).base, fin_j, false))
			   {
                cout << message::segment_collision(current->get_cor_id(), 
					                               (cor_size - 1), 
					                               corals[i].get_cor_id(), j);
                test = false;
            }
       } 
    }
}

Corail* Simulation::new_coral(double x, double y, int age, unsigned int id, 
				              int status_cor, int dir_rot, int status_dev, int nb_seg, 
				              bool& test)
{
    S2d pos{};
    pos.x = x;
    pos.y = y;

    corals.emplace_back(pos, age, id, status_cor ? ALIVE : DEAD, 
					    dir_rot ? INVTRIGO : TRIGO, status_dev ? REPRO : EXTEND, 
					    nb_seg);
	
	test = test and corals.back().lifeform_in();
	test = test and corals.back().positive_age();

	if(id_match(corals.back().get_cor_id(), test))
	{
        cout << message::lifeform_duplicated_id(corals.back().get_cor_id());
        test = false;
    }
    return &(corals.back());
}


bool Simulation::id_match(unsigned int tested_id, bool& test)
{
    for(size_t i(0); i < corals.size() - 1; ++i)
    {
        if(corals[i].get_cor_id() == tested_id)
        {
            return true;
        }
    }
    return false;
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

const void Simulation::dessin(int width, int height){
	for (size_t i(0); i<algae.size(); ++i){
		circle(algae[i].get_lifeform_pos().x,algae[i].get_lifeform_pos().y , r_alg, 
		       0, 1, 0);
	}
	
	for (size_t i(0); i<scavengers.size(); ++i){
		circle(scavengers[i].get_lifeform_pos().x, 
			   scavengers[i].get_lifeform_pos().y, scavengers[i].get_ray(), 1, 0, 0);
	}
	
	for (size_t i(0); i<corals.size(); ++i){
		if (corals[i].get_cor_status() == 1){
			square(corals[i].get_lifeform_pos().x, corals[i].get_lifeform_pos().y, 
				   d_cor, 0, 0, 1);
		} else {
			square(corals[i].get_lifeform_pos().x, corals[i].get_lifeform_pos().y, 
				   d_cor, 0, 0, 0);
		}
		for (size_t j(0); j<corals[i].get_cor_size(); ++j){
			if (corals[i].get_cor_status() == 1){
				draw_line(corals[i].get_cor_element(j).base.x, 
						  corals[i].get_cor_element(j).base.y, 
						  corals[i].get_cor_element(j).longueur, 
						  corals[i].get_cor_element(j).angle, 0, 0, 1);
			} else {
				draw_line(corals[i].get_cor_element(j).base.x, 
						  corals[i].get_cor_element(j).base.y, 
						  corals[i].get_cor_element(j).longueur, 
						  corals[i].get_cor_element(j).angle, 0, 0, 0);
			}
		}
	}
}

const int Simulation::algue_proche(size_t i){
	double angle(delta_rot);
	int index(-1);
	for (size_t j(0); j < algae.size(); ++j){
		double vect_x(algae[j].get_lifeform_pos().x - 
					  corals[i].get_cor_element(corals[i].get_cor_size()-1).base.x);
		double vect_y(algae[j].get_lifeform_pos().y - 
					  corals[i].get_cor_element(corals[i].get_cor_size()-1).base.y);
		double new_angle(delta_rot);
		if (sqrt(pow(vect_x,2)+pow(vect_y, 2)) < 
			corals[i].get_cor_element(corals[i].get_cor_size()-1).longueur){
			if (corals[i].get_cor_dir() == Dir_rot_cor::TRIGO){
				if (vect_y >= 0 or (vect_y <= 0 and corals[i].get_cor_element
					(corals[i].get_cor_size()-1).angle < 0)){
					new_angle = atan2(vect_y, vect_x) - corals[i].get_cor_element
						        (corals[i].get_cor_size()-1).angle;
				} else {
					new_angle = 2*M_PI + atan2(vect_y, vect_x) - 
								corals[i].get_cor_element
								(corals[i].get_cor_size()-1).angle;
				}
				if (new_angle > 0 and new_angle < angle){
					angle = new_angle; index = j;
				}
			} else {
				if ((vect_y >= 0 and corals[i].get_cor_element
				    (corals[i].get_cor_size()-1).angle > 0) or 
				    (vect_y <= 0)){
					new_angle = corals[i].get_cor_element
								(corals[i].get_cor_size()-1).angle -
								atan2(vect_y, vect_x);
				} else {
					new_angle = 2*M_PI + corals[i].get_cor_element
								(corals[i].get_cor_size()-1).angle - 
								atan2(vect_y, vect_x);
				}
				if (new_angle > 0 and new_angle< angle){
					angle = new_angle; index = j;
				}
			}
		}
	}
	return index;
}

void Simulation::destruction_algue(int j){
	if (j!= -1){
		Algue alga(algae[j]);
		algae[j] = algae.back();
		algae.back() = alga;
		algae.pop_back();
	}
}

const double Simulation::calcul_angle(size_t i, int j){
	double angle;
	if (j!= -1){
		double vect_x(algae[j].get_lifeform_pos().x - 
					  corals[i].get_cor_element(corals[i].get_cor_size()-1).base.x);
		double vect_y(algae[j].get_lifeform_pos().y - 
					  corals[i].get_cor_element(corals[i].get_cor_size()-1).base.y);
		if (corals[i].get_cor_dir() == Dir_rot_cor::TRIGO){
			if (vect_y >= 0 or (vect_y <= 0 and corals[i].get_cor_element
				(corals[i].get_cor_size()-1).angle < 0)){
				angle = atan2(vect_y, vect_x) - corals[i].get_cor_element
				(corals[i].get_cor_size()-1).angle;
			} else {
				angle = 2*M_PI + atan2(vect_y, vect_x) - 
							corals[i].get_cor_element
							(corals[i].get_cor_size()-1).angle;
			}
		} else {
			if ((vect_y >= 0 and corals[i].get_cor_element
				(corals[i].get_cor_size()-1).angle > 0) or 
				(vect_y <= 0)){
				angle = corals[i].get_cor_element
						(corals[i].get_cor_size()-1).angle -
						atan2(vect_y, vect_x);
			} else {
				angle = 2*M_PI + corals[i].get_cor_element
						(corals[i].get_cor_size()-1).angle - 
						atan2(vect_y, vect_x);
			}
		}
	} else {
		angle = delta_rot;
	}
	if (angle < 0){
		angle = delta_rot;
	}
	return angle;	
}

const unsigned int Simulation::creation_id(){
	unsigned int id(69);
	bool correct_id_found = false;
	while (not correct_id_found){
		id += 1;
		for (size_t i(0); i < corals.size(); ++i){
			if (id == corals[i].get_cor_id()){
				break;
			} else if (i == corals.size() - 1){
				correct_id_found = true;
			}
		}
	}
	return id;
}

void Simulation::apparition_aleatoire_algue(){
	double p(alg_birth_rate);
	bernoulli_distribution b(p);
	if (b(e)){
		uniform_int_distribution<unsigned> u(1,dmax-1);
		bool test (true);
		new_alga(u(e), u(e), 1, test);
	}
}

void Simulation::super_maj_cor(size_t i){
	int j(algue_proche(i)); //*
	double angle(calcul_angle(i, j)); //*
	
	destruction_algue(j); //*
	
	if (corals[i].maj_corail(angle, j)){
		
		// Creates new coral and adds it to simulation
		S2d pos{corals[i].get_cor_element(corals[i].get_cor_size()-1).base.x + 
				l_seg_interne*cos(corals[i].get_cor_element
				(corals[i].get_cor_size()-1).angle), 
				corals[i].get_cor_element(corals[i].get_cor_size()-1).base.y + 
				l_seg_interne*sin(corals[i].get_cor_element
				(corals[i].get_cor_size()-1).angle)};
				
		Corail corail(pos, 1, creation_id(), Status_cor::ALIVE, 
					  Dir_rot_cor::TRIGO, Status_dev::EXTEND, 1);
					  
		corail.add_segment(corals[i].get_cor_element
						   (corals[i].get_cor_size()-1).angle, 
						   l_repro - l_seg_interne);
		corals.emplace_back(corail);
	}
}

const int Simulation::get_nb_sim(){
	return nb_sim;
}

void Simulation::set_nb_sim(){
	nb_sim = 0;
}

void Simulation::maj(bool creation_algue){
	++nb_sim;
	// cout << nb_sim << " ";
	file_writing("mise à jour préc.txt");
	for (size_t i(0); i < algae.size(); ++i){
		if (algae[i].maj_algue()){
			destruction_algue(i);
		}
	}
	
	if (creation_algue){
		apparition_aleatoire_algue();
	}
	
	for (size_t i(0); i < corals.size(); ++i){
		double vieil_angle(0);
		if (corals[i].get_cor_size() > 1){
			vieil_angle = ecart_angulaire(corals[i].get_cor_element
			(corals[i].get_cor_size() - 1), corals[i].get_cor_element
			(corals[i].get_cor_size() - 2));
		}
		
		if (corals[i].get_cor_status() == Status_cor::ALIVE){
			super_maj_cor(i);
			batterie_tests(i, vieil_angle);
		}
		
	}
}

int Simulation::not_intersec_others(size_t k, S2d fin){
	for(size_t i(0); i < corals.size() ; ++i){
		if (i != k){
			for(size_t j(0); j < corals[i].get_cor_size(); ++j){ 
				S2d fin_j{};
				fin_j.x = corals[i].get_cor_element(j).base.x + 
						  corals[i].get_cor_element(j).longueur * 
					      cos(corals[i].get_cor_element(j).angle);
					
				fin_j.y = corals[i].get_cor_element(j).base.y + 
						  corals[i].get_cor_element(j).longueur * 
						  sin(corals[i].get_cor_element(j).angle);
					
				if(do_intersect(corals[k].get_cor_element
				   (corals[k].get_cor_size() - 1).base, fin, 
				   corals[i].get_cor_element(j).base, fin_j, false)){
					if (j == corals[i].get_cor_size() - 1){
						return i+2;
					} else {
						return 1;
					}
				}   
			} 
		}
	}
	
	return 0;
}

void Simulation::batterie_tests(size_t i, double vieil_angle){
	S2d fin{};
	fin.x = corals[i].get_cor_element(corals[i].get_cor_size()-1).base.x + 
			corals[i].get_cor_element(corals[i].get_cor_size()-1).longueur * 
			cos(corals[i].get_cor_element(corals[i].get_cor_size()-1).angle);
					
	fin.y = corals[i].get_cor_element(corals[i].get_cor_size()-1).base.y + 
			corals[i].get_cor_element(corals[i].get_cor_size()-1).longueur * 
			sin(corals[i].get_cor_element(corals[i].get_cor_size()-1).angle);
	
	int intersec(not_intersec_others(i, fin));
	if (not(corals[i].not_superpo_active(vieil_angle) and 
	    corals[i].segment_not_coll_him(false) and intersec == 0 and corals[i].in_bord(fin))){
		--nb_sim;
		int dir_rot(corals[i].get_cor_dir());
		int dir_rot_2;
				
		if (intersec != 1){
			dir_rot_2 = corals[intersec-2].get_cor_dir();
		}
		
		// Resets simulation
		reinit();
		
		// Reload from previous cycle
		lecture("mise à jour préc.txt", false, true);
		
		// Change dir rot from previous
		corals[i].change_dir(dir_rot);
		
		if (intersec != 1){
			corals[intersec - 2].change_dir(dir_rot_2);
		}
	}
}
