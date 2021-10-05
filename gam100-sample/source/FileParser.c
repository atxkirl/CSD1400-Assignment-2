#include "FileParser.h"
#include <stdio.h>
#include <stdlib.h>

const char EOL = '\\0';

void ReadFile(char* cFileName)
{
	// type,xpos,ypos\n

	FILE* fFile;
	char* cFileRead;
	
	fFile = fopen(cFileName, "r"); // "r" opens the file for reading

	if (fFile != NULL)
	{
		if(fgets(cFileRead, 900, fFile) != NULL) // check if the file is not null before parsing it to char*
			puts(cFileRead);
	}
	fclose(fFile);

	int iObjNum = 0;
	for (int i = 0; i < sizeof(cFileRead) / sizeof(cFileRead[0]);)
	{
		// int = char - 48
		char* cTempX, *cTempY, *cTempType;
		int j = 0, k = 0, l = 0;

		while (cFileRead[i] != ',') // find the type first
		{
			cTempType[j] = cFileRead[i];
			i++;
			j++;
		}
		i ++;

		while (cFileRead[i] != ',') // find xpos
		{
			cTempX[k] = cFileRead[i];
			i++;
			k++;
		}
		i ++;

		while (cFileRead[i] != EOL) // find y pos, which is before new line
		{
			cTempY[l] = cFileRead[i];
			i++;
			l++;
		}
		i++;

		//atoi converts a char* to int
		Map->fObjList[iObjNum]->iType = atoi(cTempType);
		Map->fObjList[iObjNum]->iPosX = atoi(cTempX);
		Map->fObjList[iObjNum]->iPosY = atoi(cTempY);
		iObjNum++;
	}
}

void WriteFile(char* cFileName, char* cToAdd)
{
	FILE* fFile;
	fFile = fopen(cFileName, "a"); // "a" opens the file for appending. if the file doesnt exist it will create it.

	if (fFile != NULL)
	{
		fputs(cToAdd, fFile);
	}
	fclose(fFile);
}