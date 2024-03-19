#ifdef _PARSER
#define _PARSER

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// Structure to store information about algae
struct Algae {
    double x;
    double y;
    int age;
};

// Structure to store information about coral segments
struct CoralSegment {
    double x;
    double y;
};

// Structure to store information about corals
struct Coral {
    std::vector<CoralSegment> segments;
    double x;
    double y;
    int age;
    int id;
    int status;
    int rotationDirection;
    int anotherStatus;
};

// Structure to store information about scavengers
struct Scavenger {
    double x;
    double y;
    int age;
    int rayon;
    int status;
    int id;
};

// Vectors to store parsed data
std::vector<Algae> algaeList;
std::vector<Coral> coralList;
std::vector<Scavenger> scavengerList;

// Variables to store parsed data
int numAlgae = 0;
int numCorals = 0;
int numScavengers = 0;

// Variables to store current section being parsed
enum class Section { ALGAE, CORAL, SCAVENGER, NONE };
Section currentSection = Section::NONE;

void parse_file(const std::string& file);

#endif
