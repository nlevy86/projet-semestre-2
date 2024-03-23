#include "simulation.h"

void Simulation::lecture(const std::string& nom_fichier) {
    std::ifstream fichier(nom_fichier);
    
    if (!fichier.open()){
        // ca retourne une erreur ?
    }

    Section section = Section::NONE;
    std::string buffer();
    int nb();

    while(std::getline(fichier, buffer)){
        if (buffer[0] == '#' or buffer.empty() or std::is_blank(buffer.c_str())){
            continue;
        }

        std::sstream ligne(buffer);

        switch(section) {
        case NONE:
            ligne >> nb;
            section = Section::ALGA;
            break; 
        case ALGA:
            if(nb > 0){
                double x = 0;
                double y = 0;
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
                double x = 0;
                double y = 0;
                int age = 0;
                unsigned int id = 0;
                int status_cor = 0;
                int dir_rot = 0;
                int status_dev = 0;
                int nb_seg = 0;

                ligne >> x >> y >> age >> id >> status_cor >> dir_rot >> status_dev >> nb_seg;
                new_coral(x,y,age,id,status_cor,dir_rot,status_dev,nb_seg);
                for(int i(0); i<nb_seg; ++i) {
                    
                }

                --nb;
            } else {
                ligne >> nb;
                section = Section::CORAL;
            }



            break;

        default:
            break;
        }


    }

    fichier.close();
   
}