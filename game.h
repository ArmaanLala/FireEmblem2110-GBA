#ifndef GAME_H
#define GAME_H

#include "gba.h"

                    /* TODO: */

            // Create any necessary structs //

typedef struct character {
     int row;
     int col;
     int health;
     int male;
     int female;
     unsigned short * sprite;
     unsigned short sprite1[256];
     unsigned short sprite2[256];
     unsigned short sprite3[256];
     int counter;

} Character;

typedef struct arrow {
     int row;
     int col;
} ArrowSelect;

/*
* For example, for a Snake game, one could be:
*
* typedef struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* } Snake;
*
*
* 
*
*
* Example of a struct to hold state machine data:
* 
* typedef struct state {
*   int currentState;
*   int nextState;
* } State
*
*/

#endif
