#include <string.h>
#include "../CProcessing/inc/cprocessing.h"

typedef struct StoreMap
{
	struct FileObj** fObjList;

} Map;

Map* new_Map();

struct FileObj
{
	int iPosX;
	int iPosY;
	int iType;
};

void ReadLevelFromFile(char* cFileName, Map* mMap);
void WriteToFile(char* cFileName, char* cToAdd);