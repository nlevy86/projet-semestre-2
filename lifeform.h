#include <iostream>
#include <cmath>
#include <vector>
#include "message.h"
#include "shape.h"


class Algue{

public:
	bool lifeform_in(S2d position); 
	bool positive_age(double age);
private:
	S2d pos_algue;
	double age_algue;
};

class corail{
public:
	bool corail_in(std::vector<Segment> seg);
	bool positive_age(double age);
	bool segment_length_in(std::vector<Segment> seg);
	bool segment_angle_in(std::vector<Segment> seg);
	bool segment_not_superpo(std::vector<Segment> seg);
	bool segment_not_coll_him(std::vector<Segment> seg);
private:
	double age_corail;
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
	bool positive_age(double age);
	bool ray_in(double ray);
private:
	S2d pos_sca;
	double age_sca;
	double ray_sca;
	Status_sca status_scaven;
	unsigned int cor_id_cible;
};

