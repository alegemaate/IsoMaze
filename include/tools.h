#ifndef TOOLS_H
#define TOOLS_H

#define BACKGROUND false
#define FOREGROUND true
#define MAIN_MENU 0
#define HELP 1
#define OPTIONS 2
#define STORY 3

// Random number generator. Use int random(highest,lowest);
extern int random (int newLowest, int newHighest);

// Clamp values
extern int clip (int n, int lower, int upper);

#endif // TOOLS_H
