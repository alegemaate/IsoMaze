#include "colors.h"

// Defs
int colors::Transparent;
int colors::Black;
int colors::White;
int colors::Turquoise;
int colors::GreenSea;
int colors::Emerald;
int colors::Nephritis;
int colors::PeterRiver;
int colors::BelizeHole;
int colors::Amethyst;
int colors::Wisteria;
int colors::WetAsphalt;
int colors::MidnightBlue;
int colors::SunFlower;
int colors::Orange;
int colors::Carrot;
int colors::Pumpkin;
int colors::Alizarin;
int colors::Pomegranate;
int colors::Clouds;
int colors::Silver;
int colors::Concrete;
int colors::Asbestos;

// Init all of our colors
colors::colors(){

}

// Init colors
void colors::initColors(){
  // Init colors

  // Basic
  Transparent = makecol(255, 0, 255);
  Black = makecol(0, 0, 0);
  White = makecol(255, 255, 255);

  // Flat ui
  Turquoise = makecol(26, 188, 156);
  GreenSea = makecol(22, 160, 133);
  Emerald = makecol(46, 204, 113);
  Nephritis = makecol(39, 174, 96);
  PeterRiver = makecol(52, 152, 219);
  BelizeHole = makecol(41, 128, 185);
  Amethyst = makecol(155, 89, 182);
  Wisteria = makecol(142, 68, 173);
  WetAsphalt = makecol(52, 73, 94);
  MidnightBlue = makecol(44, 62, 80);
  SunFlower = makecol(241, 196, 15);
  Orange = makecol(243, 156, 18);
  Carrot = makecol(230, 126, 34);
  Pumpkin = makecol(211, 84, 0);
  Alizarin = makecol(231, 76, 60);
  Pomegranate = makecol(211, 84, 0);
  Clouds = makecol(236, 240, 241);
  Silver = makecol(189, 195, 199);
  Concrete = makecol(149, 165, 166);
  Asbestos = makecol(127, 140, 141);
}

colors::~colors(){
  //dtor
}
