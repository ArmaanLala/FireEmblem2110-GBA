#ifndef GAME_H
#define GAME_H

#include "gba.h"

// The default player and enemy struct 
typedef struct character
{
     int row;
     int col;
     int health;
     int male;
     int female;
     unsigned short *sprite;
     unsigned short sprite1[256];
     unsigned short sprite2[256];
     unsigned short sprite3[256];
     int counter;

} Character;

// Had to create a seperate struct for the boss as the sprite sizes were different
typedef struct boss
{
     int row;
     int col;
     int health;
     int male;
     int female;
     unsigned short *sprite;
     unsigned short sprite1[1054];
     unsigned short sprite2[1054];
     unsigned short sprite3[1054];
     int counter;

} Boss;

// Used for menu selection
typedef struct arrow
{
     int row;
     int col;
} ArrowSelect;


#endif
