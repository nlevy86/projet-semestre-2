// Talia Meyer
// Noam Lévy

#ifndef LIFEFORM_H
#define LIFEFORM_H

#include <iostream>
#include <cmath>
#include <vector>
#include "message.h"
#include "constantes.h"

class Lifeform{
	
public:
	Lifeform(S2d _pos_lifeform, int _age_lifeform);
	const bool lifeform_in();
	const bool positive_age();
	const S2d get_lifeform_pos();
	const int get_lifeform_age();
protected:
	S2d pos_lifeform;
	int age_lifeform;
};


class Algue: public Lifeform{
public:
	Algue(S2d _pos_lifeform, int _age_lifeform);
	bool maj_algue();
};

class Corail: public Lifeform{
public:
	Corail(S2d _pos_lifeform, int _age_lifeform, unsigned int _id, 
			Status_cor _status_corail, Dir_rot_cor _dir_rot_corail, 
			Status_dev _status_develo, int _nb_segment);
	const bool corail_in();
	const bool segment_length_in();
	const bool segment_angle_in();
	const bool segment_not_superpo(bool is_epsil_zero);
	const bool segment_not_coll_him(bool is_epsil_zero);
	const void add_segment(double angle, double length);
	const Segment get_cor_element(int i);
	const size_t get_cor_size();
	const unsigned int get_cor_id();
	const int get_cor_status();
	const int get_cor_dir();
	const int get_cor_dev();
private:
	unsigned int id;
	Status_cor status_corail;
	Dir_rot_cor dir_rot_corail;
	Status_dev status_develo;
	int nb_segment;
	std::vector<Segment> cor {};
	
};

class Sca: public Lifeform {

public:
	Sca(S2d _pos_lifeform, int _age_lifeform, double _ray_sca,
		Status_sca _status_scaven, int _cor_id_cible = -1 );

	const bool ray_in();
	const int get_id_cible();
	const double get_ray();
	const int get_status();
private:
	double ray_sca;
	Status_sca status_scaven;
	int cor_id_cible;
};

#endif
