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
    if(scavengers.back().id >= 0){
        unsigned int pos_target_id(scavengers.back().id); //implicit cast
        if(!id_match(pos_target_id)){
            cout << message::lifeform_invalid_id(pos_target_id);
            std::exit(EXIT_FAILURE);
        }
    }

}

void Simulation::new_segment(double age, double length, Corail *current){
    current->add_segment(age, length);
    S2d fin_c{};
    fin_c.x = current->cor.back().base.x + current->cor.back().longueur * cos(current->cor.back().angle);
	fin_c.y = current->cor.back().base.y + current->cor.back().longueur * sin(current->cor.back().angle);

    for(size_t i(0); i < corals.size(); ++i){ //i indice du corail i en cours
       for(size_t j(0); j < corals[i].cor.size(; ++j)){ //j indice du segment j en cours du corail i en cours
			S2d fin_j{};
			fin_j.x = corals[i].cor[j].base.x + corals[i].cor[j].longueur * cos(corals[i].cor[j].angle);
			fin_j.y = corals[i].cor[j].base.y + corals[i].cor[j].longueur * sin(corals[i].cor[j].angle);
            if(do_intersect(current->cor.back().base, fin_c, corals[i].cor[j].base, fin_j, false)){
                cout << message::segment_collision(current->id, (current->cor.size() - 1), corals.[i].id, j);
                std::exit(EXIT_FAILURE);
            }
       } 
    }


}

Corail* Simulation::new_coral(double x, double y, int age, unsigned int id, int status_cor, int dir_rot, int status_dev, int nb_seg){
    corals.emplace_back(S2d(x,y), age, id, status_cor ? ALIVE : DEAD, dir_rot ? INVTRIGO : TRIGO, status_dev ? REPRO : EXTEND, nb_seg);
    if(id_match(corals.back().id)){
        cout << message::lifeform_duplicated_id(corals.back().id);
        std::exit(EXIT_FAILURE);
    }
    return &(corals.back());
}


bool Simulation::id_match(unsigned int tested_id){
    for(size_t i(0); i < corals.size(); ++i){

        if(corals[i].id == tested_id ){
            return true;
        }
    }
    return false;
}
