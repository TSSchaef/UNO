#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>

#define DECK_SIZE 108
#define CARD_SUIT 0
#define CARD_TYPE 1

/*
 * R - Red
 * Y - Yellow
 * B - Blue
 * G - Green
 * 
 * (R, Y, B, G) Number - Standard colored/numbered card
 *
 *  *S - Skip
 *  *R - Reverse
 *  *D - Draw two
 *
 *  W - Wild
 *  W0 - Standard Wild
 *  W4 - Wild and Draw 4
 */
#define DECK {"W4", "W4", "W4", "W4",\
              "W0", "W0", "W0", "W0",\
              "R0", "R1", "R1", "R2", "R2",\
              "R3", "R3", "R4", "R4", "R5",\
              "R5", "R6", "R6", "R7", "R7",\
              "R8", "R8", "R9", "R9", "RR",\
              "RR", "RS", "RS", "RD", "RD",\
              "G0", "G1", "G1", "G2", "G2",\
              "G3", "G3", "G4", "G4", "G5",\
              "G5", "G6", "G6", "G7", "G7",\
              "G8", "G8", "G9", "G9", "GR",\
              "GR", "GS", "GS", "GD", "GD",\
              "B0", "B1", "B1", "B2", "B2",\
              "B3", "B3", "B4", "B4", "B5",\
              "B5", "B6", "B6", "B7", "B7",\
              "B8", "B8", "B9", "B9", "BR",\
              "BR", "BS", "BS", "BD", "BD",\
              "Y0", "Y1", "Y1", "Y2", "Y2",\
              "Y3", "Y3", "Y4", "Y4", "Y5",\
              "Y5", "Y6", "Y6", "Y7", "Y7",\
              "Y8", "Y8", "Y9", "Y9", "YR",\
              "YR", "YS", "YS", "YD", "YD"}

//Number of points: *0 *1 *2 *3 *4 *5 *5 *7 *8 *9  *R  *S  *D  W0  W4
#define CARD_POINTS {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 20, 20, 20, 25, 50}
#define NUM_CARD_POINTS 15

#define NUM_STARTING_CARDS 7

#define NUM_SHUFFLE_SWAPS 10000

#endif
