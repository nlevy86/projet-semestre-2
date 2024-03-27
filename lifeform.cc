#include <iostream>
#include <cmath>
#include <string>
#include "lifeform.h"

using namespace std;

const bool Algue::lifeform_in() {
	if (pos_algue.x < 1 or pos_algue.x > dmax-1 or  pos_algue.y < 1 or
		pos_algue.y > dmax-1){
		cout << message::lifeform_center_outside(pos_algue.x, pos_algue.y);
		exit(EXIT_FAILURE);
		return false;
	}
	return true;
}

const bool Sca::lifeform_in(){
	if (pos_sca.x < 1 or pos_sca.x > dmax-1 or 
		pos_sca.y < 1 or pos_sca.y > dmax-1){
		cout << message::lifeform_center_outside(pos_sca.x, pos_sca.y);
		exit(EXIT_FAILURE);
		return false;
	}
	return true;
}

const bool Algue::positive_age(){
	if (age_algue>0){
		return true;
	}
	cout << message::lifeform_age(age_algue);
	exit(EXIT_FAILURE);
	return false;
}

const bool Corail::positive_age(){
	if (age_corail>0){
		return true;
	}
	cout << message::lifeform_age(age_corail);
	exit(EXIT_FAILURE);
	return false;
}

const bool Sca::positive_age(){
	if (age_sca>0){
		return true;
	}
	cout << message::lifeform_age(age_sca);
	exit(EXIT_FAILURE);
	return false;
}

const bool Corail::center_corail(){
	if (pos_cor.x < 1 or pos_cor.x > dmax-1 or 
		pos_cor.y < 1 or pos_cor.y > dmax-1){
		cout << message::lifeform_center_outside(pos_cor.x, pos_cor.y);
		exit(EXIT_FAILURE);
		return false;
	}
	return true;
}
			
const bool Corail::corail_in(){
	for (int i(0); i<= nb_segment-1; ++i){
		S2d fin;
		fin.x = cor[i].base.x + cor[i].longueur * cos(cor[i].angle);
		fin.y = cor[i].base.y + cor[i].longueur * sin(cor[i].angle);
		if ((fin.x < 0 or fin.x > dmax or 
			fin.y < 0 or fin.y > dmax)){
			cout << message::lifeform_computed_outside(id, fin.x, fin.y);
			exit(EXIT_FAILURE);
			return false;
		}
	}
	return true;
}

const bool Corail::segment_length_in(){
	for (int i(0); i<= nb_segment-1; ++i){
		if (cor[i].longueur >= l_repro or cor[i].longueur < l_repro - l_seg_interne){
			cout<< message::segment_length_outside(id, cor[i].longueur);
			exit(EXIT_FAILURE);
			return false;
		}
	}
	return true;
}

const bool Corail::segment_angle_in(){
	for (int i(0); i<= nb_segment-1; ++i){
		if (cor[i].angle > M_PI or cor[i].longueur < -M_PI){
			cout<<message::segment_angle_outside(id, cor[i].angle);
			exit(EXIT_FAILURE);
			return false;
		}
	}
	return true;
}

const bool Corail::segment_not_superpo(bool is_epsil_zero){
	for (int i(0); i<= nb_segment-1; ++i){
		if (superpo_commun(cor[i],cor[i+1], is_epsil_zero)){
			cout<<message::segment_superposition(id, i, i+1);
			exit(EXIT_FAILURE);
			return false;
		}
	}
	return true;
}

const bool Corail::segment_not_coll_him(bool is_epsil_zero){
	if (nb_segment < 2){return true;}
	for (int i(0); i<= nb_segment-3; ++i){
		for (int j(i+2); j<= nb_segment-1; ++j){
			S2d fin_i;
			fin_i.x = cor[i].base.x + cor[i].longueur * cos(cor[i].angle);
			fin_i.y = cor[i].base.y + cor[i].longueur * sin(cor[i].angle);
			S2d fin_j;
			fin_j.x = cor[j].base.x + cor[j].longueur * cos(cor[j].angle);
			fin_j.y = cor[j].base.y + cor[j].longueur * sin(cor[j].angle);
			if (do_intersect(cor[i].base, fin_i, cor[j].base, fin_j, is_epsil_zero)){
				cout<<message::segment_collision(id, i, id, j);
				exit(EXIT_FAILURE);
				return false;
			}
		}
	}
	return true;
}

const void Corail::add_segment(double angle, double length) {
	double x = pos_cor.x;
	double y = pos_cor.y;

	if(!cor.empty()) {
		Segment tail = cor.back();

		x = tail.base.x + tail.longueur*cos(tail.angle);
		y = tail.base.y + tail.longueur*sin(tail.angle);
	}

	S2d base{};
	base.x = x;
	base.y = y;
	cor.emplace_back(base, angle, length);
}

const Segment Corail::get_cor_element(int i){
	return cor[i];
}

const size_t Corail::get_cor_size(){
	return cor.size();
}

const unsigned int Corail::get_cor_id(){
	return id;
}


const bool Sca::ray_in(){
	if (ray_sca>=r_sca and ray_sca<r_sca_repro){
		return true;
	}

	cout << message::scavenger_radius_outside(ray_sca);
	exit(EXIT_FAILURE);
	return false;
}

const int Sca::get_id_cible(){
	return cor_id_cible;
}
