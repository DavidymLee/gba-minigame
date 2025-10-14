#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

// TODO: Create any necessary structs

// Struct to represent the player (swatter)
typedef struct {
  int row;
  int col;
  int width;
  int height;
} Swatter;

// Struct to represent the fly (destination target)
typedef struct {
  int row;
  int col;
  int width;
  int height;
  int active; // 1 = visible, 0 = hidden
} Fly;

#endif
