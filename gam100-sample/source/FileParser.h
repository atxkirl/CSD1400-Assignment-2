#include <string.h>
#include "../CProcessing/inc/cprocessing.h"

struct StoreMap
{
	void (*ReadFile)(char* cFileName);
	void (*WriteFile)(char* cFileName, char* cToAdd);
	struct FileObj** fObjList;

} *Map;

struct FileObj
{
	int* iPosX;
	int* iPosY;
	int* iType;
};

void ReadFile(char* cFileName);
void WriteFile(char* cFileName, char* cToAdd);