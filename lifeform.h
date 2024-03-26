#ifndef LIFEFORM_H
#define LIFEFORM_H

#include <iostream>
#include <cmath>
#include <vector>
#include "message.h"
#include "constantes.h"

class Algue{

public:
	Algue(S2d _pos_algue, int _age_algue) : pos_algue(_pos_algue), age_algue(_age_algue) {}
	const bool lifeform_in(S2d position); 
	const bool positive_age(int age);
private:
	S2d pos_algue;
	int age_algue;
};

class Corail{
public:
	Corail(S2d _pos_cor, int _age_corail, unsigned int _id, Status_cor _status_corail, Dir_rot_cor _dir_rot_corail, Status_dev _status_develo, int _nb_segment) : 
			pos_cor(_pos_cor), age_corail(_age_corail), id(_id), status_corail(_status_corail), dir_rot_corail(_dir_rot_corail), status_develo(_status_develo), nb_segment(_nb_segment) {}

	const bool corail_in();
	const bool positive_age(int age);
	const bool segment_length_in();
	const bool segment_angle_in();
	const bool segment_not_superpo(bool is_epsil_zero);
	const bool segment_not_coll_him(bool is_epsil_zero);
	const void add_segment(double angle, double length);
private:
	S2d pos_cor;
	int age_corail;
	unsigned int id;
	Status_cor status_corail;
	Dir_rot_cor dir_rot_corail;
	Status_dev status_develo;
	int nb_segment;
	std::vector<Segment> cor {};
	
};

class Sca{

public:
	Sca(S2d _pos_sca, int _age_sca, double _ray_sca, Status_sca _status_scaven, int _cor_id_cible = -1 ) : 
			pos_sca(_pos_sca), age_sca(_age_sca), status_scaven(_status_scaven), cor_id_cible(_cor_id_cible) {} 

	const bool lifeform_in(S2d position); 
	const bool positive_age(int age);
	const bool ray_in(double ray);
private:
	S2d pos_sca;
	int age_sca;
	double ray_sca;
	Status_sca status_scaven;
	int cor_id_cible;
};

#endif
