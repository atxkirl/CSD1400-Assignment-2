#pragma once

#include <stdio.h>

#define ARRAY_SIZE(x) (int)(sizeof(x)/sizeof((x)[0]))
//float epsilon
#define FLT_EPS 0.00001f
//Is between inclusive
#define IS_BETWEEN_INC(a,x,b) (a <= x && x <= b)