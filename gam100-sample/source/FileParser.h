/*
@copyright	All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file       FileParser.h
@author     Lim Guan Sheng, Marcus (l.guanshengmarcus)
@course     CSD1400 Software Engineering Project 1
@Team       BananaBoi
@brief      This file contains the functions for handling the reading and writing to file.
*//*________________________________________________________________________________________*/

#include <string.h>
#include "../CProcessing/inc/cprocessing.h"

typedef struct FileObj
{
	int iPosX;
	int iPosY;
	int iType;
	int iDir;
	char* cTag;
} FileObj;

typedef struct StoreMap
{
	FileObj** fObjList;
	int iSize;
} Map;

/// <summary>
/// Returns a Map* variable.
/// </summary>
/// <returns>Map*</returns>
Map* new_Map();

/// <summary>
/// Frees a Map* variable.
/// </summary>
/// <param name="mToFree"></param>
void free_Map(Map* mToFree);

/// <summary>
/// Read the Map Tiles based on cFileName.txt
/// </summary>
/// <param name="cFileName"></param>
/// <param name="mMap - contains all the info of the level based on the .txt"></param>
void ReadLevelFromFile(char* cFileName, Map* mMap);

/// <summary>
/// Read the Map Objectives based on cFileName.txt
/// </summary>
/// <param name="cFileName"></param>
/// <param name="cOutputObjectives - Stores the values from the .txt"></param>
void ReadObjectivesFromFile(char* cFileName, char** cOutputObjectives);

/// <summary>
/// Read the Player Controls based on cFileName.txt
/// </summary>
/// <param name="cFileName"></param>
/// <param name="cText - Store the .txt values"></param>
void ReadControlsFromFile(char* cFileName, char* cText);

/// <summary>
/// Write to file cFileName.txt
/// </summary>
/// <param name="cFileName"></param>
/// <param name="cToAdd - The stuff to add to file."></param>
/// <param name="iSize - Size of Buffer"></param>
void WriteToFile(char* cFileName, char** cToAdd, int iSize); 

/// <summary>
/// Write Controls to file cFileName.txt
/// </summary>
/// <param name="cFileName"></param>
/// <param name="cToAdd - The stuff to add to file."></param>
/// <param name="iSize - Size of Buffer"></param>
void WriteControlsToFile(char* cFileName, char* cToAdd, int iSize);