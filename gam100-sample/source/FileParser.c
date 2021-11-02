#include "FileParser.h"
#include <stdio.h>
#include <stdlib.h>

Map* new_Map()
{
	Map* goMap = (Map*)malloc(sizeof(Map));

	if (goMap)
	{
		goMap->fObjList = (FileObj**)malloc(sizeof(FileObj*) * 900);
		goMap->iSize = 0;

		if (goMap->fObjList)
		{
			for (int i = 0; i < 900; i++)
			{
				goMap->fObjList[i] = malloc(sizeof(FileObj));

				if (goMap->fObjList[i])
				{
					goMap->fObjList[i]->iPosX = 0;
					goMap->fObjList[i]->iPosY = 0;
					goMap->fObjList[i]->iType = 0;
					goMap->fObjList[i]->iDir = 0;
				}
			}
		}
	}

	return goMap;
}

/*!
@brief Reads the level from file and saves the info into the map*.

@param char* - File name
@param Map* - Pointer of type Map that points to the object list which stores the info that is read from file
@return void
*/
void ReadLevelFromFile(char* cFileName, Map* mMap)
{
	// type,xpos,ypos\n

	FILE* fFile = malloc(sizeof(FILE));
	char cFileRead[900];

	errno_t eError;
	eError = fopen_s(&fFile, cFileName, "r"); // "r" opens the file for reading

	int iObjNum = 0;
	if (eError == 0 && fFile != NULL)
	{
		while (fgets(cFileRead, 900, fFile))
		{
			printf("%s", cFileRead);

			if (cFileRead != NULL)
			{
				// int = char - 48
				char cTempX[10] = { "" }, 
					cTempY[10] = { "" },
					cTempType[10] = { "" },
					cTempDir[10] = { "" };

				int i = 0;
				int j = 0;

				while (cFileRead[i] != ',') // find the type first
				{
					i++;
				}
				strncpy_s(cTempType, 10, cFileRead, i - j);
				strcat_s(cTempType, 10, "\0");
				i += 1;
				j = i;

				while (cFileRead[i] != ',') // find xpos
				{
					i++;
				}
				strncpy_s(cTempX, 10, cFileRead+ j, i - j);
				strcat_s(cTempX, 10, "\0");
				i += 1;
				j = i;

				while (cFileRead[i] != ',') // find xpos
				{
					i++;
				}
				strncpy_s(cTempY, 10, cFileRead + j, i - j);
				strcat_s(cTempY, 10, "\0");
				i += 1;
				j = i;
		
				while (cFileRead[i] != '\n') // find y pos, which is before new line
				{
					i++;
				}
				strncpy_s(cTempDir, 10, cFileRead+ j, i - j);
				strcat_s(cTempDir, 10, "\0");

				if (mMap->fObjList[iObjNum])
				{
					//atoi converts a char* to int
					mMap->fObjList[iObjNum]->iType = atoi(cTempType);
					mMap->fObjList[iObjNum]->iPosX = atoi(cTempX);
					mMap->fObjList[iObjNum]->iPosY = atoi(cTempY);
					mMap->fObjList[iObjNum]->iDir = atoi(cTempDir);
				}
				iObjNum++;

			}
			mMap->iSize = iObjNum;

		}
		fclose(fFile);
	}
	else if (eError != 0)
	{
		printf("Error opening file! %s\n", cFileName);
		return;
	}
	else
	{
		printf("File does not exist!\n");
		return;
	}
}

void ReadObjectivesFromFile(char* cFileName, char** cOutputObjectives)
{
	FILE* fFile = malloc(sizeof(FILE));
	char cFileRead[900];

	errno_t eError;
	eError = fopen_s(&fFile, cFileName, "r"); // "r" opens the file for reading

	int iObjNum = 0;
	if (eError == 0 && fFile != NULL)
	{
		while (fgets(cFileRead, 900, fFile))
		{
			int i = 0, j = 0;

			printf("%s", cFileRead);

			if (cFileRead != NULL)
			{
				// id,text
				while (cFileRead[i] != ',') // find the type first
				{
					i++;
				}
				i += 1;
				j = i; // cFileRead[j] is at the first character after ','

				while (cFileRead[i] != '\n')
				{
					i++;
				}
				if (cOutputObjectives)
				{
					strncpy_s(cOutputObjectives[iObjNum], 100, cFileRead + j, i - j); // at this point, i is the length of string. i - j will return everything from the first character from the ',' to the last character.
					strcat_s(cOutputObjectives[iObjNum], 100, "\0");
				}
				iObjNum++;
			}

		}
		fclose(fFile);
	}
	else if (eError != 0)
	{
		printf("Error opening file! %s\n", cFileName);
		return;
	}
	else
	{
		printf("File does not exist!\n");
		return;
	}
}

/*!
@brief Write stuff to file based on the given file name.

@param char* - File name
@param char* - Stuff to add to the file
@return void
*/
void WriteToFile(char* cFileName, char** cToAdd)
{
	FILE* fFile;
	errno_t eError;

	eError = fopen_s(&fFile, cFileName, "w"); // "w" will overwrite the file if it exists.

	if (eError == 0 && fFile != NULL)
	{
		for (int i = 0; i < 450; i++)
		{
			fputs(cToAdd[i], fFile);
		}
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