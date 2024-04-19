// Talia Meyer
// Noam Lévy

#include <iostream>
#include <cmath>
#include <string>
#include "lifeform.h"

using namespace std;

Lifeform::Lifeform(S2d _pos_lifeform, int _age_lifeform) :
	pos_lifeform(_pos_lifeform), age_lifeform(_age_lifeform) {}

Algue::Algue(S2d _pos_lifeform, int _age_lifeform) : 
	Lifeform(_pos_lifeform, _age_lifeform) {}

Corail::Corail(S2d _pos_lifeform, int _age_lifeform, unsigned int _id, 
		Status_cor _status_corail, Dir_rot_cor _dir_rot_corail, 
		Status_dev _status_develo, int _nb_segment) :
			Lifeform(_pos_lifeform, _age_lifeform),
			id(_id),
			status_corail(_status_corail),
			dir_rot_corail(_dir_rot_corail),
			status_develo(_status_develo),
			nb_segment(_nb_segment)  {}

Sca::Sca(S2d _pos_lifeform, int _age_lifeform, double _ray_sca, 
		Status_sca _status_scaven, int _cor_id_cible) :
			Lifeform(_pos_lifeform, _age_lifeform),
			ray_sca(_ray_sca),
			status_scaven(_status_scaven),
			cor_id_cible(_cor_id_cible) {}

const bool Lifeform::lifeform_in() {
	if (pos_lifeform.x < 1 or pos_lifeform.x > dmax-1 or  pos_lifeform.y < 1 or
		pos_lifeform.y > dmax-1){
		cout << message::lifeform_center_outside(pos_lifeform.x, pos_lifeform.y);
		return false;
	}
	return true;
}

const bool Lifeform::positive_age(){
	if (age_lifeform>0){
		return true;
	}
	cout << message::lifeform_age(age_lifeform);
	return false;
}
			
const bool Corail::corail_in(){
	for (int i(0); i<= nb_segment-1; ++i){
		S2d fin;
		fin.x = cor[i].base.x + cor[i].longueur * cos(cor[i].angle);
		fin.y = cor[i].base.y + cor[i].longueur * sin(cor[i].angle);
		if ((fin.x < 0 or fin.x > dmax or 
			fin.y < 0 or fin.y > dmax)){
			cout << message::lifeform_computed_outside(id, fin.x, fin.y);
			return false;
		}
	}
	return true;
}

const bool Corail::segment_length_in(){
	for (int i(0); i<= nb_segment-1; ++i){
		if (cor[i].longueur >= l_repro or cor[i].longueur < l_repro - l_seg_interne){
			cout<< message::segment_length_outside(id, cor[i].longueur);
			return false;
		}
	}
	return true;
}

const bool Corail::segment_angle_in(){
	for (int i(0); i<= nb_segment-1; ++i){
		if (cor[i].angle > M_PI or cor[i].angle < -M_PI){
			cout<<message::segment_angle_outside(id, cor[i].angle);
			return false;
		}
	}
	return true;
}

const bool Corail::segment_not_superpo(bool is_epsil_zero){
	for (int i(0); i<= nb_segment-1; ++i){
		if (superpo_commun(cor[i],cor[i+1], is_epsil_zero)){
			cout<<message::segment_superposition(id, i, i+1);
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
				return false;
			}
		}
	}
	return true;
}

const void Corail::add_segment(double angle, double length) {
	double x = pos_lifeform.x;
	double y = pos_lifeform.y;

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
	return false;
}

const int Sca::get_id_cible(){
	return cor_id_cible;
}

bool Algue::maj_algue(){
	++age_lifeform;
	return (age_lifeform >= max_life_alg);
}
	
