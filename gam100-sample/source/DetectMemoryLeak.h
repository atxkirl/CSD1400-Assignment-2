/*
* @file		DetectMemoryLeak.c
* @author	Adrian Tan (t.xingkhiangadrian)
* @course	CSD1400 Software Engineering Project 1
* @Team		BananaBoi
* @date		12/10/2021
* @brief	Contains preprocessor definitions to add Microsoft's own memory leak
*			detector calls. Call _CRT_DumpMemoryLeaks() at the end of the game
*			to display all leaks.
*//*--------------------------------------------------------------------------*/

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
