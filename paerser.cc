#include "parser.h"

void parser_file(const std::string& file) {
    std::ifstream file("t11.txt");
    std::string line;

    //TODO : check if file is actually opened
  
    while (std::getline(file, line)) {
        // Skip empty lines and lines starting with '#'
        if (line.empty() || line[0] == '#')
            continue;

        // Parse number of entities
        if (currentSection == Section::NONE) {
            std::istringstream iss(line);
            iss >> numAlgae;
            currentSection = Section::ALGAE;
            continue;
        }

        // Parse algae data
        if (currentSection == Section::ALGAE) {
			std::istringstream iss(line);

			if(numAlgae > 0){
				Algae algae{};
				iss >> algae.x >> algae.y >> algae.age;
				algaeList.push_back(algae);
				numAlgae--;
			} else {
				iss >> numCorals;		
                currentSection = Section::CORAL;
            }
            continue;
        }

        // Parse coral data
      if (currentSection == Section::CORAL) {
			  std::istringstream iss(line);
  			
  			if(numCorals > 0){
    				Coral coral{};
    				int numSegments;
    				iss >> coral.x >> coral.y >> coral.age >> coral.id >> coral.status >> coral.rotationDirection >> coral.anotherStatus >> numSegments;
    				
    				for (int i = 0; i < numSegments; ++i) {
    					  CoralSegment segment{};
      					std::getline(file, line);
      					std::istringstream segmentIss(line);
      					segmentIss >> segment.x >> segment.y;
      					coral.segments.push_back(segment);
    				}
				
    				coralList.push_back(coral);
    				numCorals--;
    			} else {
    				iss >> numScavengers;
    				currentSection = Section::SCAVENGER;
    			}
        
          continue;
        }

        // Parse scavenger data
        if (currentSection == Section::SCAVENGER) {
    			if(numScavengers > 0) {
    				Scavenger scavenger{};
    				std::istringstream iss(line);
    				iss >> scavenger.x >> scavenger.y >> scavenger.age >> scavenger.rayon >> scavenger.status >> scavenger.id;
    				scavengerList.push_back(scavenger);
    				numScavengers--;
    				continue;
    			} else {
    				break;
          }
        }
    }

    // Output parsed data for demonstration
    std::cout << "Algae:" << std::endl;
    for (const auto& algae : algaeList) {
        std::cout << "x: " << algae.x << ", y: " << algae.y << ", age: " << algae.age << std::endl;
    }

    std::cout << "Corals:" << std::endl;
    for (const auto& coral : coralList) {
        std::cout << "x: " << coral.x << ", y: " << coral.y << ", age: " << coral.age << ", id: " << coral.id << ", status: " << coral.status << ", rot dir:" << coral.rotationDirection << ", other: " << coral.anotherStatus << std::endl;
        std::cout << "\t"<< "Segments:" << std::endl;
        for (const auto& segment : coral.segments) {
            std::cout << "\t"<<  "x: " << segment.x << ", y: " << segment.y << std::endl;
        }
    }

    std::cout << "Scavengers:" << std::endl;
    for (const auto& scavenger : scavengerList) {
        std::cout << "x: " << scavenger.x << ", y: " << scavenger.y << ", age: " << scavenger.age << ", rayon: " << scavenger.rayon << ", status: " << scavenger.status << std::endl;
    }
}
