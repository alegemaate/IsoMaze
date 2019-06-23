#include "tools.h"

#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// Random number generator. Use int random(highest,lowest);
int random (int newLowest, int newHighest) {
  int lowest = newLowest, highest = newHighest;
  int range = (highest - lowest) + 1;
  int randomNumber = lowest + (int) (range * rand() / (RAND_MAX + 1.0));
  return randomNumber;
}

// Clamp values
int clip (int n, int lower, int upper) {
  return MAX (lower, MIN (n, upper));
}
