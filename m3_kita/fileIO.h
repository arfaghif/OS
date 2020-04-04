#include "teks.h"
#define SectorSize 512
#define MapSector 0x100
#define DFSector 0x101	
#define SectorSector 0x103	
#define ALREADY_EXIST -1
#define INSUFFICIENT_FILES_ENTRY -2
#define INSUFFICIENT_SECTOR -3
#define DIR_NOT_FOUND -4
#define SUCCESS 1
#define File_NOT_FOUND -1



void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
void readFile(char *buffer, char *path, int *result, char parentIndex);
void searchFile(char* dir, int*index, int* success,char parent);
void executeProgram(int segment, char *path, int *result, char parentIndex);
void deleteFile(char *path, int *success, char parentIndex);
void copyFile(char *path1, char *path2, int *success, char parentIndex);
void move(char *path1, char *path2, int *success, char parentIndex, int aFile);

