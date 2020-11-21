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

// All tiles
BITMAP* cursor;
BITMAP* cube_types[NUM_COLORS];
int maze[MAP_WIDTH][MAP_LENGTH];

// Buffer
BITMAP* buffer;
BITMAP* player;

// Map Offset
int offset_x;
int offset_y;

// Declaration
void draw();

// Map scrolling
void scroll_map() {
  offset_x += key[KEY_LEFT] ? 4 : key[KEY_RIGHT] ? -4 : 0;
  offset_y += key[KEY_UP] ? 4 : key[KEY_DOWN] ? -4 : 0;
}

//  Carve the maze starting at x, y.
// 0 = wall, 1 = untraced floor, 2 = traced floor
int carve_maze(int (*_maze)[MAP_WIDTH][MAP_LENGTH],
               int width,
               int height,
               int _x,
               int _y) {
  // Start x
  int itr_x = _x;
  int itr_y = _y;

  // Check against width and height
  if (!(itr_x > 0 && itr_x < width && itr_y > 0 && itr_y < height))
    return -1;

  // Init maze to 0 (floor)
  for (int i = 0; i < width; i++) {
    for (int t = 0; t < height; t++) {
      (*_maze)[i][t] = 0;
    }
  }

  // First spot is floor
  (*_maze)[itr_x][itr_y] = 2;

  // Run till return
  while (!key[KEY_ESC]) {
    // Set random dirs index
    // 0 UP, 1 RIGHT, 2 DOWN, 3 LEFT
    int dirs[4] = {0, 1, 2, 3};

    // Shuffle dirs 4 times (its indexed so it can choose 1 of 4 directions,
    // but we know when all have been choosen which would mean a dead end)
    for (int i = 0; i < 4; i++) {
      for (int t = 0; t < 4; t++) {
        int old_val = dirs[i];
        int new_index = random(0, 3);

        dirs[i] = dirs[new_index];
        dirs[new_index] = old_val;
      }
    }

    // Find a direction to go
    for (int i = 0; i < 4; i++) {
      // Change in x and y depending on dir
      int delta_x = sin((float)dirs[i] / 2 * M_PI) * 2;
      int delta_y = cos((float)dirs[i] / 2 * M_PI) * 2;

      // Check the direction
      // In bounds
      if (itr_x + delta_x < width && itr_y + delta_y < height &&
          itr_x + delta_x > 0 && itr_y + delta_y > 0) {
        // Make sure its not going to pass through any walls
        if ((*_maze)[itr_x + delta_x / 2][itr_y + delta_y / 2] == 0 &&
            (*_maze)[itr_x + delta_x][itr_y + delta_y] == 0) {
          // Draw it into map
          (*_maze)[itr_x + delta_x / 2][itr_y + delta_y / 2] = 1;
          (*_maze)[itr_x + delta_x][itr_y + delta_y] = 1;
          itr_x += delta_x;
          itr_y += delta_y;
          break;
        }
      }

      // No direction can fix this problem! Backtrace until it can
      if (i == 3) {
        // Find which way the path leads
        for (int t = 0; t < 4; t++) {
          // Change in x and y depending on dir
          delta_x = sin((float)t / 2 * M_PI);
          delta_y = cos((float)t / 2 * M_PI);

          // Path found
          if ((*_maze)[itr_x + delta_x][itr_y + delta_y] == 1) {
            // Backtraced paths are 2 (or this will never end!
            (*_maze)[itr_x][itr_y] = 2;
            (*_maze)[itr_x + delta_x][itr_y + delta_y] = 2;

            // Change x
            itr_x += delta_x * 2;
            itr_y += delta_y * 2;

            // Found!
            break;
          }

          // Done, maze should be finished
          if (t == 3) {
            return 0;
          }
        }
      }
    }

    // Move map
    scroll_map();

    // Draw
    draw();
  }

  return 0;
}

// Run logic loop
void update() {
  // Move map
  scroll_map();

  // Rebuild maze
  if (key[KEY_R])
    carve_maze(&maze, MAP_WIDTH - 1, MAP_LENGTH - 1, 1, 1);
}

// Draw
void draw() {
  // Background
  rectfill(buffer, 0, 0, buffer->w, buffer->h, COLOUR_CLOUDS);

  // Draw the map
  for (int i = 0; i < MAP_WIDTH; i++)
    for (int t = 0; t < MAP_LENGTH; t++)
      if (maze[i][t] != 0)
        draw_cube_at(buffer, cube_types[maze[i][t]], i, t, 0, offset_x,
                     offset_y);

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

  // Rest = RANDOM
  for (int i = 2; i < NUM_COLORS; i++) {
    const int index = random(0, 6);
    const int colors[7] = {
        COLOUR_TURQUOISE, COLOUR_EMERALD, COLOUR_BELIZEHOLE, COLOUR_AMETHYST,
        COLOUR_SUNFLOWER, COLOUR_CARROT,  COLOUR_POMEGRANITE};
    cube_types[i] = generate_cube(colors[index], TILE_WIDTH, TILE_LENGTH);
  }
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
