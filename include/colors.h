/*
 * Colors
 * Stores preset colors to be used ingame
 * 30/08/16
 * Allan Legemaate
 */

#ifndef COLORS_H
#define COLORS_H

#include <allegro.h>

// Class colors, stores preset colors for use with allegro
class colors
{
  public:
    colors();
    virtual ~colors();

    // Init colors
    static void initColors();

    // Init colors
    // Basic
    static int Transparent;
    static int Black;
    static int White;

    // Flat ui
    static int Turquoise;
    static int GreenSea;
    static int Emerald;
    static int Nephritis;
    static int PeterRiver;
    static int BelizeHole;
    static int Amethyst;
    static int Wisteria;
    static int WetAsphalt;
    static int MidnightBlue;
    static int SunFlower;
    static int Orange;
    static int Carrot;
    static int Pumpkin;
    static int Alizarin;
    static int Pomegranate;
    static int Clouds;
    static int Silver;
    static int Concrete;
    static int Asbestos;
};

#endif // COLORS_H
