/*!
@file            FileParser.c
@author          Lim Guan Sheng, Marcus (l.guanshengmarcus)
@course          CSD1400 Software Engineering Project 1
@Team            BananaBoi
@brief           This file contains the functions for handling the reading
				 and writing to file.
*//*______________________________________________________________________*/

#include "FileParser.h"
#include <stdio.h>
#include <stdlib.h>

/// <summary>
/// Returns a Map* variable.
/// </summary>
/// <returns>Map*</returns>
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
					goMap->fObjList[i]->cTag = malloc(sizeof(char) * 30);
				}
			}
		}
	}

	return goMap;
}

void free_Map(Map* mToFree)
{
	if (mToFree)
	{
		if (mToFree->fObjList)
		{
			for (int i = 0; i < 900; i++)
			{
				if (mToFree->fObjList[i])
				{
					free(mToFree->fObjList[i]->cTag);
				}
				free(mToFree->fObjList[i]);
			}
			free(mToFree->fObjList);
		}
		free(mToFree);
	}
}

/// <summary>
/// Read the Map Tiles based on cFileName.txt
/// </summary>
/// <param name="cFileName"></param>
/// <param name="mMap - contains all the info of the level based on the .txt"></param>
void ReadLevelFromFile(char* cFileName, Map* mMap)
{
	// type,xpos,ypos\n

	FILE* fFile;
	char cFileRead[900];

	errno_t eError;
	eError = fopen_s(&fFile, cFileName, "r"); // "r" opens the file for reading

	int iObjNum = 0;
	if (eError == 0 && fFile != NULL)
	{
		while (fgets(cFileRead, 900, fFile) != NULL)
		{
			if (cFileRead[0] == '\n')
				break;

			printf("%s", cFileRead);

			if (cFileRead != NULL)
			{
				// int = char - 48
				char cTempX[10] = { "" },
					cTempY[10] = { "" },
					cTempType[10] = { "" },
					cTempDir[10] = { "" },
					cTempTag[30] = { "" };

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

				while (cFileRead[i] != ',') // find xpos
				{
					i++;
				}
				strncpy_s(cTempDir, 10, cFileRead + j, i - j);
				strcat_s(cTempDir, 10, "\0");
				i += 1;
				j = i;
		
				while (cFileRead[i] != '\n') // find y pos, which is before new line
				{
					i++;
				}
				strncpy_s(cTempTag, 30, cFileRead+ j, i - j);
				strcat_s(cTempTag, 30, "\0");

				if (mMap->fObjList[iObjNum])
				{
					//atoi converts a char* to int
					mMap->fObjList[iObjNum]->iType = atoi(cTempType);
					mMap->fObjList[iObjNum]->iPosX = atoi(cTempX);
					mMap->fObjList[iObjNum]->iPosY = atoi(cTempY);
					mMap->fObjList[iObjNum]->iDir = atoi(cTempDir);
					strcpy_s(mMap->fObjList[iObjNum]->cTag, 30, cTempTag);
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

/// <summary>
/// Read the Map Objectives based on cFileName.txt
/// </summary>
/// <param name="cFileName"></param>
/// <param name="cOutputObjectives - Stores the values from the .txt"></param>
void ReadObjectivesFromFile(char* cFileName, char** cOutputObjectives)
{
	FILE* fFile;
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

/// <summary>
/// Read the Player Controls based on cFileName.txt
/// </summary>
/// <param name="cFileName"></param>
/// <param name="cText - Store the .txt values"></param>
void ReadControlsFromFile(char* cFileName, char* cText)
{
	FILE* fFile;

	errno_t eError;
	eError = fopen_s(&fFile, cFileName, "r"); // "r" opens the file for reading

	int iTextNum = 0;
	if (eError == 0 && fFile != NULL)
	{
		//int iLetterRead;
		//while ((iLetterRead = fgetc(fFile)) != '\0')
		//{
		//	if (feof(fFile))
		//		break;

		//	printf("%c\n", iLetterRead);

		//	cText[iTextNum] = (char)iLetterRead;
		//	iTextNum++;
		//}

		char temp[6] = { 0 };
		fgets(temp, 6, fFile); //read wasde
		printf("%s\n", temp);
		for (int i = 0; i < 5; i++)
		{
			cText[iTextNum++] = temp[i];
		}
		fgetc(fFile); //get rid of \n
		fgets(temp, 6, fFile); //read music vol
		printf("%s", temp);
		cText[iTextNum++] = (char)atoi(temp);
		fgets(temp, 6, fFile); //read sfx vol
		printf("%s\n", temp);
		cText[iTextNum++] = (char)atoi(temp);

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

/// <summary>
/// Write to file cFileName.txt
/// </summary>
/// <param name="cFileName"></param>
/// <param name="cToAdd - The stuff to add to file."></param>
/// <param name="iSize - Size of Buffer"></param>
void WriteToFile(char* cFileName, char** cToAdd, int iSize)
{
	FILE* fFile;
	errno_t eError;

	eError = fopen_s(&fFile, cFileName, "w"); // "w" will overwrite the file if it exists.

	if (eError == 0 && fFile != NULL)
	{
		for (int i = 0; i < iSize; i++)
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

/// <summary>
/// Write Controls to file cFileName.txt
/// </summary>
/// <param name="cFileName"></param>
/// <param name="cToAdd - The stuff to add to file."></param>
/// <param name="iSize - Size of Buffer"></param>
void WriteControlsToFile(char* cFileName, char* cToAdd, int iSize)
{
	FILE* fFile;
	errno_t eError;

	eError = fopen_s(&fFile, cFileName, "w"); // "w" will overwrite the file if it exists.

	if (eError == 0 && fFile != NULL)
	{
		//for (int i = 0; i < iSize; i++)
		//{
		//	fputc(cToAdd[i], fFile);
		//}
		//printf("Saved!! \n");
		char temp[6] = { 0 };
		for (int i = 0; i < 5; ++i)
			temp[i] = cToAdd[i];
		fputs(temp, fFile);
		fputc((int)'\n', fFile);
		char temp2[4] = { 0 };
		sprintf_s(temp2, 4, "%d", (int)cToAdd[5]);
		fputs(temp2, fFile);
		fputc((int)'\n', fFile);
		sprintf_s(temp2, 4, "%d", (int)cToAdd[6]);
		fputs(temp2, fFile);

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