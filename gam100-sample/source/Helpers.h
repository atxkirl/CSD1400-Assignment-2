/*
@file		Helpers.h
@author		Adrian Tan (t.xingkhiangadrian)
@coauthor	Ow Hong Yu (ow.h)
@course		CSD1400 Software Engineering Project 1
@Team		BananaBoi
@date		06/10/2021
@brief		Contains helpful functions to calculate various things, like
			random values, or the sizes of arrays.
*//*______________________________________________________________________*/

#pragma once

#include <stdio.h>
#include <stdlib.h>

// float epsilon
#define FLT_EPS 0.00001f
// Infinite Loop Detector
#define INFINITE_LOOP 100 // Change value to find compromise between searching alot of things, vs lag caused by stuck loops.

// Array Size
#define ARRAY_SIZE(x) (int)(sizeof(x)/sizeof((x)[0]))
// Is between inclusive
#define IS_BETWEEN_INC(a,x,b) (a <= x && x <= b)
// Random within Range
#define RAND(min, max) (rand() % (max - min + 1) + min)
#define FRAND(min, max) (float)(rand() % (max - min + 1) + min)

float GetTileScale();
