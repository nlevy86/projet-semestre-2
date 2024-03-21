#include <iostream>
#include <cmath>
#include <vector>
#include "message.h"
#include "constantes.h"

class Algue{

public:
	bool lifeform_in(S2d position); 
	bool positive_age(int age);
private:
	S2d pos_algue;
	int age_algue;
};

class Corail{
public:
	bool corail_in(std::vector<Segment> seg);
	bool positive_age(int age);
	bool segment_length_in(std::vector<Segment> seg);
	bool segment_angle_in(std::vector<Segment> seg);
	bool segment_not_superpo(std::vector<Segment> seg);
	bool segment_not_coll_him(std::vector<Segment> seg);
private:
	int age_corail;
	unsigned int id;
	Status_cor status_corail;
	Dir_rot_cor dir_rot_corail;
	Status_dev status_develo;
	int nb_segment;
	std::vector<Segment> cor;
	
};

class Sca{

public:
	bool lifeform_in(S2d position); 
	bool positive_age(int age);
	bool ray_in(double ray);
private:
	S2d pos_sca;
	int age_sca;
	double ray_sca;
	Status_sca status_scaven;
	unsigned int cor_id_cible;
};

