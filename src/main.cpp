/*
 * Maze Game
 * An ISO maze game
 * 22/08/16
 * Allan Legemaate
 */

#include <allegro.h>
#include <alpng.h>
#include <math.h>
#include <iostream>

#include "tools.h"
#include "colors.h"

#define MAP_WIDTH 1001
#define MAP_LENGTH 1001
#define MAP_HEIGHT 10

#define TILE_WIDTH 32
#define TILE_LENGTH 32

#define NUM_COLORS 10

// All tiles
BITMAP *cursor;

BITMAP *tile_types[NUM_COLORS];
int maze[MAP_WIDTH][MAP_LENGTH];

// Buffer
BITMAP *buffer;

BITMAP *player;

// Map Offset
int offset_x;
int offset_y;

// Player pos
int player_x;
int player_y;



void draw();

//  Carve the maze starting at x, y.
// 0 = wall, 1 = untraced floor, 2 = traced floor
int CarveMaze( int (*_maze)[MAP_WIDTH][MAP_LENGTH], int width, int  height, int _x, int _y) {
  // Start x
  int itr_x = _x;
  int itr_y = _y;

  // Check against width and height
  if( !(itr_x > 0 && itr_x < width && itr_y > 0 && itr_y < height))
    return -1;

  // Delta x/y
  int delta_x = 0;
  int delta_y = 0;

  // Init maze to 0 (floor)
  for( int i = 0; i < width; i ++){
    for( int t = 0; t < height; t ++){
      (*_maze)[i][t] = 0;
    }
  }

  // First spot is floor
  (*_maze)[itr_x][itr_y] = 2;

  // Run till return
  while( !key[KEY_ESC]){
    // Set random dirs index
    // 0 UP, 1 RIGHT, 2 DOWN, 3 LEFT
    int dirs[4] = { 0, 1, 2, 3};

    // Shuffle dirs 4 times (its indexed so it can choose 1 of 4 directions,
    // but we know when all have been choosen which would mean a dead end)
    for( int i = 0; i < 4; i ++){
      for( int t = 0; t < 4; t ++){
        int old_val = dirs[i];
        int new_index = random( 0, 3);

        dirs[i] = dirs[new_index];
        dirs[new_index] = old_val;
      }
    }

    // Find a direction to go
    for( int i = 0; i < 4; i ++){
      // Change in x and y depending on dir
      delta_x = sin( float(dirs[i])/2 * M_PI) * 2;
      delta_y = cos( float(dirs[i])/2 * M_PI) * 2;

      // Check the direction
      // In bounds
      if( itr_x + delta_x < width && itr_y + delta_y < height && itr_x + delta_x > 0 && itr_y + delta_y > 0){
        // Make sure its not going to pass through any walls
        if( (*_maze)[itr_x + delta_x/2][itr_y + delta_y/2] == 0 && (*_maze)[itr_x + delta_x][itr_y + delta_y] == 0){
          // Draw it into map
          (*_maze)[itr_x + delta_x/2][itr_y + delta_y/2] = 1;
          (*_maze)[itr_x + delta_x  ][itr_y + delta_y  ] = 1;
          itr_x += delta_x;
          itr_y += delta_y;
          break;
        }
      }

      // No direction can fix this problem! Backtrace til it can
      if( i == 3){
        // Find which way the path leads
        for( int i = 0; i < 4; i ++){
          // Change in x and y depending on dir
          delta_x = sin( float(i)/2 * M_PI);
          delta_y = cos( float(i)/2 * M_PI);

          // Path found
          if( (*_maze)[itr_x + delta_x][itr_y + delta_y] == 1){
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
          if( i == 3){
            return 0;
          }
        }
      }
    }

    // Visualize
    if( offset_x < -(itr_x - itr_y) * TILE_WIDTH/2 + SCREEN_W/2)
      offset_x += 16;
    else
      offset_x -= 16;

    if( offset_y < -(itr_x + itr_y) * TILE_LENGTH/4 + SCREEN_H/2)
      offset_y += 16;
    else
      offset_y -= 16 ;

    offset_x = -(itr_x - itr_y) * TILE_WIDTH/2 + SCREEN_W/2;
    offset_y = -(itr_x + itr_y) * TILE_LENGTH/4 + SCREEN_H/2;

    draw();

    rest( 50);

    /*while( !key[KEY_SPACE]){}
    while( key[KEY_SPACE]){}*/
  }
  return 0;
}


// Cube generator
BITMAP *generate_cube( int _color, int _width, int _height, int _angle){
  // Temp cube
  BITMAP *temp_cube = create_bitmap( _width, _height);

  // New colors
  int top_color = makecol( clip( getr(_color) - 30, 0, 255), clip( getg(_color) - 30, 0, 255), clip( getb(_color) - 30, 0, 255));
  int left_color = makecol( clip( getr(_color) + 10, 0, 255), clip( getg(_color) + 10, 0, 255), clip( getb(_color) + 10, 0, 255));
  int right_color = makecol( clip( getr(_color) + 0, 0, 255), clip( getg(_color) + 0, 0, 255), clip( getb(_color) + 0, 0, 255));

  // Fill transparent
  rectfill( temp_cube, 0, 0, _width, _height, makecol( 255, 0, 255));

  // Draw outlines
  // Center
  line( temp_cube, _width/2, _height*0.50, _width/2, _height     , left_color );
  // Bottom
  line( temp_cube, _width/2, _height     , 0       , _height*0.75, right_color);
  line( temp_cube, _width/2, _height     , _width  , _height*0.75, left_color );
  // Sides
  line( temp_cube, 0       , _height*0.25, 0       , _height*0.75, right_color);
  line( temp_cube, _width-1, _height*0.25, _width-1, _height*0.75, left_color );
  // Top
  line( temp_cube, _width/2, 0           , 0       , _height*0.25, top_color  );
  line( temp_cube, _width/2, 0           , _width  , _height*0.25, top_color  );
  line( temp_cube, _width/2, _height*0.50, 0       , _height*0.25, top_color  );
  line( temp_cube, _width/2, _height*0.50, _width  , _height*0.25, top_color  );

  // Fill!
  // Top
  floodfill( temp_cube, _width / 2    , _height * 0.50 - 1, top_color);
  // Left
  floodfill( temp_cube, _width / 2 + 1, _height * 0.50 + 1, left_color);
  // Right
  floodfill( temp_cube, _width / 2 - 1, _height * 0.50 + 1, right_color);

  // Return the cube
  return temp_cube;
}

// Draw cube at
void draw_cube_at( BITMAP* _buffer, BITMAP* _image, int _x, int _y, int _z, int _offset_x, int _offset_y){
  draw_sprite( _buffer, _image, ((_x - _y) * TILE_WIDTH/2) + _offset_x, ((_x + _y) * TILE_LENGTH/4 - _z * TILE_LENGTH/2) + _offset_y);
}

// Init game
void setup(){
  // Init allegro
  allegro_init();

  install_keyboard();
  install_mouse();
  install_timer();
  install_sound( DIGI_AUTODETECT, MIDI_AUTODETECT, ".");

  set_color_depth( 32);

  int s_width;
  int s_height;
  get_desktop_resolution( &s_width, &s_height);
  set_gfx_mode( GFX_AUTODETECT, s_width, s_height, 0, 0);

  //set_gfx_mode( GFX_AUTODETECT_WINDOWED, 1280, 960, 0, 0);



  set_window_title("isoMaze");

  // Init our colors
  colors::initColors();

  // Make the buffer
  buffer = create_bitmap( SCREEN_W, SCREEN_H);

  srand(time(NULL));
  random( 0, 10);

  // Map offset
  offset_x = 18;
  offset_y =  -2;

  // Player position
  player_x = 1;
  player_y = 1;

  // Make cursor
  cursor = create_bitmap( 16, 16);
  rectfill( cursor, 0, 0, cursor -> w, cursor -> h, makecol( 255, 0, 255));
  ellipse( cursor, cursor -> w/2, cursor -> h/2, cursor -> w/2 - 1, cursor -> h/2 - 1, makecol( 0, 255, 0));

  // Make a cube
  // 0 = NO COLOR
  tile_types[0] = generate_cube( colors::Transparent, TILE_WIDTH, TILE_LENGTH, 30);
  // 1 = GRAY
  tile_types[1] = generate_cube( colors::Silver, TILE_WIDTH, TILE_LENGTH, 30);

  // Rest = RANDOM
  for( int i = 2; i < NUM_COLORS; i++){
    int random_num = random( 0, 6);
    int random_color = 0;

    switch( random_num){
      case 0:
        random_color = colors::Turquoise;
        break;
      case 1:
        random_color = colors::Emerald;
        break;
      case 2:
        random_color = colors::BelizeHole;
        break;
      case 3:
        random_color = colors::Amethyst;
        break;
      case 4:
        random_color = colors::SunFlower;
        break;
      case 5:
        random_color = colors::Carrot;
        break;
      case 6:
        random_color = colors::Pomegranate;
        break;

    }
    tile_types[i] = generate_cube( random_color, TILE_WIDTH, TILE_LENGTH, 30);
  }

  // Player
  player = generate_cube( colors::MidnightBlue, TILE_WIDTH, TILE_LENGTH, 30);
}

// Run logic loop
void update(){
  // Move map
  if( key[KEY_LEFT])
    offset_x ++;
  else if( key[KEY_RIGHT])
    offset_x--;

  if( key[KEY_UP])
    offset_y++;
  else if( key[KEY_DOWN])
    offset_y--;

  //. Move player
  int delta_x = 0;
  int delta_y = 0;
  if( key[KEY_W])
    delta_y = -1;
  else if( key[KEY_A])
    delta_y = 1;
  else if( key[KEY_Q])
    delta_x = -1;
  else if( key[KEY_S])
    delta_x = 1;

  // Check if u can move
  // Bounds
  if( player_x + delta_x < MAP_WIDTH && player_x + delta_x >= 0 && player_y + delta_y < MAP_WIDTH && player_y + delta_y >= 0){
    // Map
    if( maze[player_x + delta_x][player_y + delta_y] != 0){
      player_x += delta_x;
      player_y += delta_y;
      rest( 70);
    }
  }

  // Rebuild maze
  if( key[KEY_R])
    // Make maze
    CarveMaze( &maze, MAP_WIDTH - 1, MAP_LENGTH - 1, 1 , 1);
}

// Draw
void draw(){
  // Background
  rectfill( buffer, 0, 0, buffer -> w, buffer -> h, colors::Clouds);

  // Draw the map
  for( int i = 0; i < MAP_WIDTH; i++)
    for( int t = 0; t < MAP_LENGTH; t++)
      if( maze[i][t] != 0)
        draw_cube_at( buffer, tile_types[maze[i][t]], i, t, 0, offset_x, offset_y);

  // Player
  draw_cube_at( buffer, player, player_x, player_y, 1, offset_x, offset_y);

  // Cursor
  draw_sprite( buffer, cursor, mouse_x, mouse_y);

  // Draw buffer
  draw_sprite( screen, buffer, 0, 0);
}

// Main
int main(){
  // Setup
  setup();

  // Run game loops
  while( !key[KEY_ESC]){
    update();
    draw();
  }

  return 0;
}
END_OF_MAIN();
