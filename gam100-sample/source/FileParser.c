#include "FileParser.h"
#include <stdio.h>
#include <stdlib.h>

Map* new_Map()
{
	Map* goMap = malloc(sizeof(Map));

	return goMap;
}

void ReadFromFile(char* cFileName, Map* mMap)
{
	// type,xpos,ypos\n

	FILE* fFile = malloc(sizeof(FILE));
	char cFileRead[900];

	errno_t eError;
	eError = fopen_s(&fFile, cFileName, "r"); // "r" opens the file for reading

	if (eError == 0 && fFile != NULL)
	{
		if (fgets(cFileRead, 900, fFile) != NULL) // check if the file is not null before parsing it to char*
			puts(cFileRead);
		fclose(fFile);
	}

	int iObjNum = 0;
	if (cFileRead != NULL)
	{
		for (int i = 0; i < sizeof(cFileRead);)
		{
			// int = char - 48
			char* cTempX, * cTempY, * cTempType;
			cTempX = malloc(sizeof(char));
			cTempY = malloc(sizeof(char));
			cTempType = malloc(sizeof(char));

			int j = 0, k = 0, l = 0;
			cTempX = "0";
			cTempY = "0";
			cTempType = "0";

			while (cFileRead[i] != ',') // find the type first
			{
				strncat_s(cTempType, 1, &cFileRead[i], 2);
				i++;
				j++;
			}
			i++;

			while (cFileRead[i] != ',') // find xpos
			{
				strncat_s(cTempX, 1, &cFileRead[i], 2);
				i++;
				k++;
			}
			i++;

			while (cFileRead[i] != '\\0') // find y pos, which is before new line
			{
				strncat_s(cTempY, 1, &cFileRead[i], 2);
				i++;
				l++;
			}
			i++;

			//atoi converts a char* to int
			if (cTempType != NULL)
				mMap->fObjList[iObjNum]->iType = atoi(cTempType);

			if (cTempX != NULL)
				mMap->fObjList[iObjNum]->iPosX = atoi(cTempX);

			if (cTempY != NULL)
				mMap->fObjList[iObjNum]->iPosY = atoi(cTempY);
			iObjNum++;
		}
	}
}

void WriteToFile(char* cFileName, char* cToAdd)
{
	FILE* fFile;
	errno_t eError;

	eError = fopen_s(&fFile, cFileName, "a"); // "a" opens the file for appending. if the file doesnt exist it will create it.

	if (eError == 0 && fFile != NULL)
	{
		fputs(cToAdd, fFile);
		printf("Saved!! \n");
		fclose(fFile);
	}
	else if (eError != 0)
	{
		printf("Error opening file! %s\n", cFileName);
	}
	else
	{
		printf("File does not exist!\n");
	}
}