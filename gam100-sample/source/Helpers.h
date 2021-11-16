#pragma once

#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE(x) (int)(sizeof(x)/sizeof((x)[0]))
//float epsilon
#define FLT_EPS 0.00001f
//Is between inclusive
#define IS_BETWEEN_INC(a,x,b) (a <= x && x <= b)

#define RAND(min, max) (rand() % (max - min + 1) + min)

float GetTileScale();
