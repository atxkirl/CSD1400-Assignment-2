#include <string.h>
#include "../CProcessing/inc/cprocessing.h"

// type|xpos|ypos\n

struct StoreMap
{
	int* iPosX;
	int* iPosY;
	int* iType;

	void (*ReadFile)();
	void (*WriteFile)(char *cFileName);
} *Map;

void WriteFile();
void WriteFile(char* cFileName);