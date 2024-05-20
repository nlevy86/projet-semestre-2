// Talia Meyer 80%
// Noam Lévy   20%

#include <iostream>
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
		       
	Lifeform(_pos_lifeform, _age_lifeform), id(_id), 
			 status_corail(_status_corail), dir_rot_corail(_dir_rot_corail),
			 status_develo(_status_develo), nb_segment(_nb_segment)  {}


Sca::Sca(S2d _pos_lifeform, int _age_lifeform, double _ray_sca, 
		Status_sca _status_scaven, int _cor_id_cible) :
		
	Lifeform(_pos_lifeform, _age_lifeform), ray_sca(_ray_sca),
			 status_scaven(_status_scaven), cor_id_cible(_cor_id_cible) {}


const bool Lifeform::lifeform_in(bool output)
{
	if (pos_lifeform.x < 1 or pos_lifeform.x > dmax-1 or  pos_lifeform.y < 1 or
		pos_lifeform.y > dmax-1)
	{
		if (output){
			cout << message::lifeform_center_outside(pos_lifeform.x, pos_lifeform.y);
		}
		return false;
	}
	return true;
}

const bool Lifeform::positive_age(bool output)
{
	if (age_lifeform > 0)
	{
		return true;
	}
	if (output){
		cout << message::lifeform_age(age_lifeform);
	}
	return false;
}

const S2d Lifeform::get_lifeform_pos()
{
	return pos_lifeform;
}		

const int Lifeform::get_lifeform_age()
{
	return age_lifeform;
}

const bool Corail::corail_in(bool output)
{
	for (int i(0); i<= nb_segment-1; ++i)
	{
		S2d fin;
		fin.x = cor[i].base.x + cor[i].longueur * cos(cor[i].angle);
		fin.y = cor[i].base.y + cor[i].longueur * sin(cor[i].angle);
		
		if ((fin.x < 0 or fin.x > dmax or 
			fin.y < 0 or fin.y > dmax))
		{
			if (output){
				cout << message::lifeform_computed_outside(id, fin.x, fin.y);
			}
			return false;
		}
	}
	return true;
}

const bool Corail::segment_length_in(bool output)
{
	for (int i(0); i<= nb_segment-1; ++i)
	{
		if (cor[i].longueur >= l_repro or cor[i].longueur < l_repro - l_seg_interne)
		{
			if (output){
				cout<< message::segment_length_outside(id, cor[i].longueur);
			}
			return false;
		}
	}
	return true;
}

const bool Corail::segment_angle_in(bool output)
{
	for (int i(0); i<= nb_segment-1; ++i)
	{
		if (cor[i].angle > M_PI or cor[i].angle < -M_PI)
		{
			if (output){
				cout<< message::segment_angle_outside(id, cor[i].angle);
			}
			return false;
		}
	}
	return true;
}

const bool Corail::segment_not_superpo(bool is_epsil_zero, bool output){
	for (int i(0); i<= nb_segment-1; ++i){
		if (superpo_commun(cor[i],cor[i+1], is_epsil_zero)){
			if (output){
				cout<<message::segment_superposition(id, i, i+1);
			}
			return false;
		}
	}
	return true;
}

const bool Corail::segment_not_coll_him(bool is_epsil_zero, bool output)
{
	if (nb_segment < 2)
	{
		return true;
	}
	for (int i(0); i<= nb_segment-3; ++i)
	{
		for (int j(i+2); j<= nb_segment-1; ++j)
		{
			S2d fin_i;
			fin_i.x = cor[i].base.x + cor[i].longueur * cos(cor[i].angle);
			fin_i.y = cor[i].base.y + cor[i].longueur * sin(cor[i].angle);
			S2d fin_j;
			fin_j.x = cor[j].base.x + cor[j].longueur * cos(cor[j].angle);
			fin_j.y = cor[j].base.y + cor[j].longueur * sin(cor[j].angle);
			
			if (do_intersect(cor[i].base, fin_i, cor[j].base, fin_j, is_epsil_zero))
			{
				if (output){
					cout<<message::segment_collision(id, i, id, j);
				}
				return false;
			}
		}
	}
	return true;
}

const void Corail::add_segment(double angle, double length) 
{
	double x = pos_lifeform.x;
	double y = pos_lifeform.y;

	if(!cor.empty()) 
	{
		Segment tail = cor.back();

		x = tail.base.x + tail.longueur*cos(tail.angle);
		y = tail.base.y + tail.longueur*sin(tail.angle);
	}

	S2d base{};
	base.x = x;
	base.y = y;
	cor.emplace_back(base, angle, length);
}

const Segment Corail::get_cor_element(int i)
{
	return cor[i];
}

const size_t Corail::get_cor_size()
{
	return cor.size();
}

const unsigned int Corail::get_cor_id()
{
	return id;
}

const int Corail::get_cor_status()
{
	return (status_corail == Status_cor::DEAD) ? 0 : 1;
}

const int Corail::get_cor_dir()
{
	return (dir_rot_corail == Dir_rot_cor::TRIGO) ? 0 : 1;
}

const int Corail::get_cor_dev(){
	return (status_develo == Status_dev::EXTEND) ? 0 : 1;
}


const bool Sca::ray_in(bool output){
	if (ray_sca>=r_sca and ray_sca<r_sca_repro){
		return true;
	}
	if (output){
		cout << message::scavenger_radius_outside(ray_sca);
	}
	return false;
}

const int Sca::get_id_cible()
{
	return cor_id_cible;
}

const double Sca::get_ray()
{
	return ray_sca;
}

const int Sca::get_status()
{
	return (status_scaven == Status_sca::FREE) ? 0 : 1;
}

bool Algue::maj_algue()
{
	++age_lifeform;
	int int_max_life_alg(max_life_alg);
	return (age_lifeform == int_max_life_alg);
}
	
bool Corail::maj_corail(double angle, int j){
	++age_lifeform;
	
	int int_max_life_cor(max_life_cor);
	if (age_lifeform == int_max_life_cor){
		status_corail = Status_cor::DEAD;
	}
	
	// Increase length if there's an alga
	if (j != -1){
		cor.back().longueur += delta_l;
	}
	
	double double_l_repro(l_repro);
	bool is_repro(false);
	
	if (status_corail == Status_cor::ALIVE){
		
		if (cor.back().longueur < double_l_repro){
			
			// Rotate in the right direction
			if (dir_rot_corail == Dir_rot_cor::TRIGO){
				cor.back().angle += angle;
				
				if (cor.back().angle > M_PI){
					cor.back().angle -= 2*M_PI;
				}
			} else {
				cor.back().angle -= angle;
				
				if (cor.back().angle < -M_PI){
					cor.back().angle += 2*M_PI;
				}
			}
		} else {
			if (status_develo == Status_dev::EXTEND){
				S2d pos{cor.back().base.x + cor.back().longueur*cos(cor.back().angle),
						cor.back().base.y + cor.back().longueur*sin(cor.back().angle)};
				Segment seg{pos, cor.back().angle, l_repro - l_seg_interne};
				cor.emplace_back(seg);
				++nb_segment;
				status_develo = Status_dev::REPRO;
			} else {
				cor.back().longueur = l_repro/2;
				is_repro = true;
				status_develo = Status_dev::EXTEND;
			}
		}
		
	}
	return is_repro;
}

bool Corail::not_superpo_active(double vieil_angle){
	if (cor.size() > 1){
		double new_angle(ecart_angulaire(cor.back(), cor[cor.size() - 2]));
		if (vieil_angle < 0 and vieil_angle > -delta_rot and new_angle > 0 and 
		    new_angle < delta_rot){
			return false;
		} else if (vieil_angle > 0 and vieil_angle < delta_rot and new_angle < 0 and 
		           new_angle > -delta_rot){
			return false;
		}
	}
	return true;
}

void Corail::change_dir(){
	if (dir_rot_corail == Dir_rot_cor::TRIGO){
		dir_rot_corail = Dir_rot_cor::INVTRIGO;
	} else {
		dir_rot_corail = Dir_rot_cor::TRIGO;
	}
}
 
const bool Corail::in_bord(S2d fin){
	if (fin.x < epsil_zero or fin.x > dmax-epsil_zero or fin.y < epsil_zero or 
		fin.y > dmax-epsil_zero) {
			return false;
	}
	return true;
}

void Corail::change_last_seg_length(){
	cor.back().longueur -= delta_l;
	
	if (cor.size() > 0 and cor.back().longueur == 0){
		cor.pop_back();
		nb_segment -=1;
	}
}

bool Sca::maj_sca()
{
	++age_lifeform;
	int int_max_life_sca(max_life_sca);
	return (age_lifeform == int_max_life_sca);
}

void Sca::set_status (int k){
	if (k==0) {
		status_scaven = Status_sca::FREE;
	}
	else if (k==1) {
		status_scaven =Status_sca::EATING;
	}
}

void Sca::set_id_cible ( int id){
	cor_id_cible = id;
}

void Sca::move_scavenger_on_target(Corail cor_cible){
	size_t cor_size = cor_cible.get_cor_size();
	double angle = cor_cible.get_cor_element(cor_size - 1).angle;
	pos_lifeform.x -= delta_l*cos(angle);
	pos_lifeform.y -= delta_l*sin(angle);
}

void Sca::set_ray(unsigned l){
	ray_sca = l;
}


void Sca::move_scavenger_to_target(Corail targeted_coral, double dist){
	size_t cor_size = targeted_coral.get_cor_size();

	
	double x_cor = targeted_coral.get_cor_element(cor_size -1).base.x + targeted_coral.get_cor_element(cor_size -1).longueur*cos(targeted_coral.get_cor_element(cor_size -1).angle);
	double y_cor = targeted_coral.get_cor_element(cor_size -1).base.y + targeted_coral.get_cor_element(cor_size -1).longueur*sin(targeted_coral.get_cor_element(cor_size -1).angle);
	double angle = atan2(y_cor - pos_lifeform.y, x_cor - pos_lifeform.x);
	
	double l = dist;
	l = (l >= 0) ? l : 0;
	l = (l <= delta_l) ? l : delta_l;
	
	pos_lifeform.x += l*cos(angle);
	pos_lifeform.y += l*sin(angle);
	
	
}
