#include "cube.h"
#include "constants.h"
#include "helpers.h"

// Cube generator
BITMAP* generate_cube(int _color, int _width, int _height) {
  // Temp cube
  BITMAP* temp_cube = create_bitmap(_width, _height);

  // New colors
  int top_color =
      makecol(clip(getr(_color) - 30, 0, 255), clip(getg(_color) - 30, 0, 255),
              clip(getb(_color) - 30, 0, 255));
  int left_color =
      makecol(clip(getr(_color) + 10, 0, 255), clip(getg(_color) + 10, 0, 255),
              clip(getb(_color) + 10, 0, 255));
  int right_color =
      makecol(clip(getr(_color) + 0, 0, 255), clip(getg(_color) + 0, 0, 255),
              clip(getb(_color) + 0, 0, 255));

  // Fill transparent
  rectfill(temp_cube, 0, 0, _width, _height, makecol(255, 0, 255));

  // Draw outlines
  // Center
  line(temp_cube, _width / 2, _height * 0.50, _width / 2, _height, left_color);
  // Bottom
  line(temp_cube, _width / 2, _height, 0, _height * 0.75, right_color);
  line(temp_cube, _width / 2, _height, _width, _height * 0.75, left_color);
  // Sides
  line(temp_cube, 0, _height * 0.25, 0, _height * 0.75, right_color);
  line(temp_cube, _width - 1, _height * 0.25, _width - 1, _height * 0.75,
       left_color);
  // Top
  line(temp_cube, _width / 2, 0, 0, _height * 0.25, top_color);
  line(temp_cube, _width / 2, 0, _width, _height * 0.25, top_color);
  line(temp_cube, _width / 2, _height * 0.50, 0, _height * 0.25, top_color);
  line(temp_cube, _width / 2, _height * 0.50, _width, _height * 0.25,
       top_color);

  // Fill!
  // Top
  floodfill(temp_cube, _width / 2, _height * 0.50 - 1, top_color);
  // Left
  floodfill(temp_cube, _width / 2 + 1, _height * 0.50 + 1, left_color);
  // Right
  floodfill(temp_cube, _width / 2 - 1, _height * 0.50 + 1, right_color);

  // Return the cube
  return temp_cube;
}

// Draw cube at
void draw_cube_at(BITMAP* _buffer,
                  BITMAP* _image,
                  int _x,
                  int _y,
                  int _z,
                  int _offset_x,
                  int _offset_y) {
  draw_sprite(_buffer, _image, ((_x - _y) * TILE_WIDTH / 2) + _offset_x,
              ((_x + _y) * TILE_LENGTH / 4 - _z * TILE_LENGTH / 2) + _offset_y);
}
