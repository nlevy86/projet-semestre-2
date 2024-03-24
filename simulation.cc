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
                Corail *current = new_coral(x, y, age, id, status_cor, dir_rot, status_dev, nb_seg);
                
                for(int i(0); i < nb_seg; ++i) {
                    std::getline(fichier, buffer);
                    
                    if (buffer[0] == '#' or buffer.empty() or std::is_blank(buffer.c_str())){
                        continue;
                    }

                    std::sstream ligne_seg(buffer);

                    double angle = 0;
                    double len = 0;
                    ligne_seg >> angle >> len;

                    new_segment(angle, len, current);
                }

                --nb;
            } else {
                ligne >> nb;
                section = Section::SCAVENGER;
            }

            break;
        case SCAVENGER:
            if(nb > 0){
                double x = 0;
                double y = 0;
                int age = 0;
                double rayon = 0; 
                int status_sca = 0;
                int target_id = -1;

                //OPTION QUI PRENDRAIT COMPTE DU CAS OU IL N Y A PAS DE TARGET ID
                ligne >> x >> y >> age >> rayon >> status_sca;
                if(status_sca){
                    ligne >> target_id;    
                }
                new_sca(x, y, age, rayon, status_sca, target_id);
                
                --nb;
            }
            break;
        default:
            break;
        }
    }

    fichier.close();
}

void Simulation::new_alga(double x, double y, int age){
    algae.emplace_back(S2d{x,y}, age);
    
}

void Simulation::new_sca(double x, double y, int age, double rayon, int status_sca, int target_id){
    scavengers.emplace_back(S2d(x,y), age, rayon, status_sca ? EATING : FREE, target_id);

}

void Simulation::new_segment(double age, double length, Corail *current){
    current->add_segment(age, length);

}

Corail* Simulation::new_coral(double x, double y, int age, unsigned int id, int status_cor, int dir_rot, int status_dev, int nb_seg){
    corals.emplace_back(S2d(x,y), age, id, status_cor ? ALIVE : DEAD, dir_rot ? INVTRIGO : TRIGO, status_dev ? REPRO : EXTEND, nb_seg);
    
    return &(corals.back());
}
