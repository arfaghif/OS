// loadFile.c
// Michael Black  Modification to file system tubes 2 OS 2020
// Loads a file into the file system
// This should be compiled with gcc and run outside of the OS
#define MapSector 0x100
#define DFSector 0x101	
#define SectorSector 0x103
#define SectorSize 512	 
#define Line 16
#define MaxDFName 14

#include <stdio.h>

void main(int argc, char* argv[]) {
  int i;

  if (argc < 2) {
    printf("Specify file name to load\n");
    return;
  }

  // open the source file
  FILE* loadFil;
  loadFil = fopen(argv[1], "r");
  if (loadFil == 0) {
    printf("File not found\n");
    return;
  }

  // open the floppy image
  FILE* floppy;
  floppy = fopen("system.img", "r+");
  if (floppy == 0) {
    printf("system.img not found\n");
    return;
  }

  // load the disk map
  char map[SectorSize];
  fseek(floppy, SectorSize*MapSector, SEEK_SET);
  for (i = 0; i < SectorSize; i++) map[i] = fgetc(floppy);

  // load the directory
  char dir[SectorSize];
  fseek(floppy, SectorSize * DFSector, SEEK_SET);
  for (i = 0; i < SectorSize*2; i++) dir[i] = fgetc(floppy);
  
  // load the sector
  char sector[SectorSize];
  fseek(floppy, SectorSize * SectorSector, SEEK_SET);
  for(i = 0; i < SectorSize; i++) sector[i] = fgetc(floppy);

  // find a free entry in the directory
  for (i = 0; i < SectorSize*2; i = i +Line){
    if (dir[i] == 0 && dir[i+1]==0 && dir[i+2]==0) break;
  }
  if (i == SectorSize*2) {
    printf("Not enough room in directory\n");
    return;
  }
  int dirindex = i;

  // fill the name field with 00s first
  for (i = 0; i < MaxDFName; i++) dir[dirindex + i+2] = 0x00;
  // copy the name over
  for (i = 0; i < MaxDFName; i++) {
    if (argv[1][i] == 0) break;
    dir[dirindex + i+2] = argv[1][i];
  }

  //dirindex = dirindex + 12;

  //find free space in sectors.img
  for(i=0;i<32;++i){
    if(sector[i*Line]==0) break;
  }
  if(i==32){
    printf("No enough room in sectors\n");
    return;
  }
  int secidx = i*Line;
  //set dir folder and sector
  dir[dirindex] = 0xFF;
  dir[dirindex+1] = secidx;

  // find free sectors and add them to the file
  int sectcount = 0;
  while (!feof(loadFil)) {
    if (sectcount == 16) {
      printf("Not enough space in directory entry for file\n");
      return;
    }

    // find a free map entry
    for (i = 0; i < SectorSize; i++)
      if (map[i] == 0) break;
    if (i == SectorSize) {
      printf("Not enough room for file\n");
      return;
    }

    // mark the map entry as taken
    map[i] = 0xFF;

    // mark the sector in the directory entry
    // dir[dirindex] = i;
    // dirindex++;
    sector[secidx++]=i;
    sectcount++;

    printf("Loaded %s to sector %d\n", argv[1], i);

    // move to the sector and write to it
    fseek(floppy, i * 512, SEEK_SET);
    for (i = 0; i < 512; i++) {
      if (feof(loadFil)) {
        fputc(0x0, floppy);
        break;
      } else {
        char c = fgetc(loadFil);
        fputc(c, floppy);
      }
    }
  }

  // write the map and directory back to the floppy image
  fseek(floppy, SectorSize*MapSector, SEEK_SET);
  for (i = 0; i < 512; i++) fputc(map[i], floppy);

  fseek(floppy, 512 * DFSector, SEEK_SET);
  for (i = 0; i < 512; i++) fputc(dir[i], floppy);

  fseek(floppy, 512 * DFSector, SEEK_SET);
  for (i = 0; i < 512; i++) fputc(dir[i], floppy);

  fclose(floppy);
  fclose(loadFil);
}
