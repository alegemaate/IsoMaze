/*
 * Maze Game
 * An ISO maze game
 * 22/08/16
 * Allan Legemaate
 */

#include <allegro.h>
#include <math.h>

#include "colors.h"
#include "constants.h"
#include "cube.h"
#include "helpers.h"

// Travelling directions
enum Direction { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN, DIRECTION_LEN };

// All tiles
BITMAP* cursor;
BITMAP* cube_types[3];
int maze[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];

// Buffer
BITMAP* buffer;
BITMAP* player;

// Map Offset
int offset_x;
int offset_y;

// Cube colours
const int cube_colors[NUM_CUBE_COLORS] = {
    COLOUR_TURQUOISE, COLOUR_EMERALD, COLOUR_BELIZEHOLE, COLOUR_AMETHYST,
    COLOUR_SUNFLOWER, COLOUR_CARROT,  COLOUR_POMEGRANITE};

// Declaration
void draw();

// Map scrolling
void scroll_map() {
  offset_x += key[KEY_LEFT] ? 4 : key[KEY_RIGHT] ? -4 : 0;
  offset_y += key[KEY_UP] ? 4 : key[KEY_DOWN] ? -4 : 0;
}

// Is in bounds
int in_bounds(int x, int y, int z, int width, int length, int height) {
  return x < width && x >= 0 && y < length && y >= 0 && z < height && z >= 0;
}

//  Carve the maze starting at x, y.
// 0 = wall, 1 = untraced floor, 2 = traced floor
void carve_maze(int (*_maze)[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT],
                int width,
                int length,
                int height,
                int _x,
                int _y,
                int _z) {
  // Check against width and length
  if (!in_bounds(_x, _y, _z, width, length, height)) {
    return;
  }

  // Iterator
  int itr_x = _x;
  int itr_y = _y;
  int itr_z = _z;

  // Init maze to 0 (floor)
  memset(*_maze, 0, sizeof *_maze);

  // First spot is floor
  (*_maze)[itr_x][itr_y][itr_z] = 2;

  // Path found
  char path_found = 1;

  // Run till escape or path not found
  while (!key[KEY_ESC] && path_found) {
    // Assume no path exists
    path_found = 0;

    // Set random dirs index
    enum Direction dirs[DIRECTION_LEN] = {FORWARD, RIGHT, BACKWARD,
                                          LEFT,    UP,    DOWN};

    // Shuffle dirs 6 times (its indexed so it can choose 1 of 6 directions,
    // but we know when all have been choosen which would mean a dead end)
    for (int i = 0; i < DIRECTION_LEN; i++) {
      int old_val = dirs[i];
      int new_index = random(0, DIRECTION_LEN - 1);

      dirs[i] = dirs[new_index];
      dirs[new_index] = old_val;
    }

    // Find a direction to go
    for (int i = 0; i < DIRECTION_LEN; i++) {
      // Reset deltas
      int delta_x = 0;
      int delta_y = 0;
      int delta_z = 0;

      // Calc deltas
      if (dirs[i] < 2) {
        delta_x = dirs[i] == 0 ? 1 : -1;
      } else if (dirs[i] < 4) {
        delta_y = dirs[i] == 2 ? 1 : -1;
      } else {
        delta_z = dirs[i] == 4 ? 1 : -1;
      }

      // New position
      int x_1 = itr_x + delta_x * 2;
      int y_1 = itr_y + delta_y * 2;
      int z_1 = itr_z + delta_z * 2;
      int x_2 = itr_x + delta_x;
      int y_2 = itr_y + delta_y;
      int z_2 = itr_z + delta_z;

      // Check the direction is in bounds (we take 2 steps)
      if (in_bounds(x_1, y_1, z_1, width, length, height)) {
        // Make sure its not going to pass through any walls
        if ((*_maze)[x_2][y_2][z_2] == 0 && (*_maze)[x_1][y_1][z_1] == 0) {
          // Add it to the map
          (*_maze)[x_2][y_2][z_2] = 1;
          (*_maze)[x_1][y_1][z_1] = 1;

          // Increment iterator
          itr_x = x_1;
          itr_y = y_1;
          itr_z = z_1;

          // Path found!
          path_found = 1;

          break;
        }
      }
    }

    // No direction can fix this problem! Backtrace until it can
    if (!path_found) {
      // Find which way the path leads
      for (int t = 0; t < DIRECTION_LEN; t++) {
        int delta_x = 0;
        int delta_y = 0;
        int delta_z = 0;

        // Calc deltas
        if (t < 2) {
          delta_x = t == 0 ? 1 : -1;
        } else if (t < 4) {
          delta_y = t == 2 ? 1 : -1;
        } else {
          delta_z = t == 4 ? 1 : -1;
        }

        // New position
        int x_1 = itr_x + delta_x;
        int y_1 = itr_y + delta_y;
        int z_1 = itr_z + delta_z;

        // Path found
        if (in_bounds(x_1, y_1, z_1, width, length, height)) {
          if ((*_maze)[x_1][y_1][z_1] == 1) {
            // Backtraced paths are 2 (or this will never end!
            (*_maze)[itr_x][itr_y][itr_z] = 2;
            (*_maze)[x_1][y_1][z_1] = 2;

            // Change x
            itr_x += delta_x * 2;
            itr_y += delta_y * 2;
            itr_z += delta_z * 2;

            // Set path found
            path_found = 1;

            // Found!
            break;
          }
        }
      }
    }

    // Move map
    scroll_map();

    // Draw
    draw();
  }
}

// Run logic loop
void update() {
  // Move map
  scroll_map();

  // Rebuild maze
  if (key[KEY_R]) {
    // New colour
    destroy_bitmap(cube_types[2]);
    cube_types[2] = generate_cube(cube_colors[random(0, NUM_CUBE_COLORS - 1)],
                                  TILE_WIDTH, TILE_LENGTH);

    // Gen
    carve_maze(&maze, MAP_WIDTH, MAP_LENGTH, MAP_HEIGHT, 0, 0, 0);
  }
}

// Draw
void draw() {
  // Background
  rectfill(buffer, 0, 0, buffer->w, buffer->h, COLOUR_CLOUDS);

  // Draw the map
  for (int i = 0; i < MAP_WIDTH; i++) {
    for (int t = 0; t < MAP_LENGTH; t++) {
      for (int j = 0; j < MAP_HEIGHT; j++) {
        if (maze[i][t][j] != 0) {
          draw_cube_at(buffer, cube_types[maze[i][t][j]], i, t, j, offset_x,
                       offset_y);
        }
      }
    }
  }

  // Cursor
  draw_sprite(buffer, cursor, mouse_x, mouse_y);

  // Draw buffer
  draw_sprite(screen, buffer, 0, 0);
}

// Init game
void setup() {
  // Init allegro
  allegro_init();
  install_keyboard();
  install_mouse();

  // Color depth and screen mode
  set_color_depth(32);
  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1280, 960, 0, 0);
  set_window_title("isoMaze");

  // Make the buffer
  buffer = create_bitmap(SCREEN_W, SCREEN_H);

  // Set up random
  srand(time(NULL));
  random(0, 10);

  // Map offset
  offset_x = SCREEN_W / 2;
  offset_y = TILE_LENGTH * 4;

  // Make cursor
  cursor = create_bitmap(16, 16);
  rectfill(cursor, 0, 0, cursor->w, cursor->h, makecol(255, 0, 255));
  ellipse(cursor, cursor->w / 2, cursor->h / 2, cursor->w / 2 - 1,
          cursor->h / 2 - 1, makecol(0, 255, 0));

  // Make a cube
  cube_types[0] = generate_cube(COLOUR_TRANSPARENT, TILE_WIDTH, TILE_LENGTH);
  cube_types[1] = generate_cube(COLOUR_SILVER, TILE_WIDTH, TILE_LENGTH);
  cube_types[2] = generate_cube(cube_colors[random(0, NUM_CUBE_COLORS - 1)],
                                TILE_WIDTH, TILE_LENGTH);
}

// Main
int main() {
  // Setup
  setup();

  // Run game loops
  while (!key[KEY_ESC]) {
    update();
    draw();
  }

  return 0;
}
END_OF_MAIN()
