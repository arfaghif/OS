#define TRUE 1
#define FALSE 0
#define MaxDF 64		
#define MAXDFN 14
#define Dir 0xFF
#define Line 16




void printString(char *string);
void enter();
void printStringln(char* string);
void readString(char *string);
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);
void clear(char *buffer, int length);
int len(char *string);
void bacaLogo();
int isEqual(char *s1, char*s2);
void splitPath(char *path,char* dirPath, char* filename);
void copy(char* string1,char* string2, int begin, int length);
void split(char* string, char* left, char* right, char separator);
void searchDir(char*dir, char * path, int* index, int* success, char parent);

