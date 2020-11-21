/*
 * Cube
 * Cube related functions
 * 15/09/19
 * Allan Legemaate
 */

#ifndef CUBE_H
#define CUBE_H

#include <allegro.h>

// Cube generator
BITMAP* generate_cube(int _color, int _width, int _height);

// Draw cube at
void draw_cube_at(BITMAP* _buffer,
                  BITMAP* _image,
                  int _x,
                  int _y,
                  int _z,
                  int _offset_x,
                  int _offset_y);

#endif
