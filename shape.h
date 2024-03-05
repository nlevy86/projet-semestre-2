#include <iostream>

struct S2d {double x=0; doube y=0;};

class Segment {
public:
  void setbase (double abs, double ord);
private:
  S2d base;
  double angle;
  unsigned double longueur;                
};
