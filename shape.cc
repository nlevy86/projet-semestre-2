#include <iostream>
#include <cmaths>
#include "shape.h"
#include "constantes.h"
#include "message.h"

constexpr double epsil_zero(0.5) ;

using namespace std;

void shape::setbase(abs,ord){
  if ((abs>=1 and abs<=max-1) and (ord>=1 and ord<=max-1){
    S2d.x=abs;
    S2d.y=ord;
  } else { lifeform_center_outside(abs, ord);}
};

void shape::setangle(rad){
  if (rad<=M_PI and rad>=M_PI){
    angle=rad;
  }else{
};

void shape::setlongueur(taille){
  longueur=taille;
};
