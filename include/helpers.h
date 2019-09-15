#ifndef TOOLS_H
#define TOOLS_H

// Random number generator. Use int random(highest,lowest);
extern int random(int newLowest, int newHighest);

// Clamp values
extern int clip(int n, int lower, int upper);

#endif // TOOLS_H
