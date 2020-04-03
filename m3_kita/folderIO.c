#include "folderIO.h"


/*Menggunakan rekursif*/
void printPathDir(char* dir, int index){
	char name[MAXDFN];
	if((index >  0 && index < MaxDF)|| index<0){
		printPathDir(dir, dir[index*Line]);
		printString("/");
		copy(dir,name, index*Line+2, MAXDFN);
		printString(name);
	}
}


void makeDir(char *path, int *success, char parentIndex){
  char df[2*SectorSize];
  int i,idx;
  int succ;
  char dirPath[MaxDF*(Line)];
  char dirName[MAXDFN], curName[MAXDFN];
  int parent;
  readSector(df,DFSector);
  i=0;
  succ = FALSE;
  *success =FALSE;
  do {
    if (df[i] == '\0') {
      succ = TRUE;
      idx = div(i, 16);
    }
    i+=Line;
  }while (!succ&&i<SectorSize);
  if (succ){
    splitPath(path, dirPath,dirName);
    succ = FALSE;
    searchDir(df,dirPath,&parent,&succ,parentIndex);
    if (succ){
      //cari dir
      succ = FALSE;
      for(i=0; i<MaxDF*Line && !(succ); i+=Line){
        copy(df, curName, i+2,MAXDFN);
        if(df[i]==parent && df[i+1] == Dir && isEqual(curName, dirName)){
          succ=TRUE;
        }
      }
      if(!succ){//foldernya belum ada
        df[idx*Line] = parent;
        df[idx*Line+1] = 0xFF;
        for(i=0;i<MAXDFN;++i){
          df[idx *Line+i+2]=dirName[i];
        }
        writeSector(df,DFSector);
        *success = TRUE;
      }
    }
  }
}

void deleteDir(char *path, int *success, char parentIndex){
  char df[2*SectorSize];
  int i,j,idx;
  int succ;
  char dirPath[MaxDF*(Line)];
  char dirName[MAXDFN], curName[MAXDFN];
  int parent;
  int dummy;
  readSector(df,DFSector);
  i=0;
  succ = FALSE;
  *success =FALSE;
  splitPath(path, dirPath,dirName);
  succ = FALSE;
  searchDir(df,dirPath,&parent,&succ,parentIndex);
  if (succ){
    //cari dir
    succ = FALSE;
    for(i=0; i<MaxDF*Line && !(succ); i+=Line){
      copy(df, curName, i+2,MAXDFN);
      if(df[i]==parent && df[i+1] == Dir && isEqual(curName, dirName)){
        succ =TRUE;
        idx =i;
        for (j=i; j < i+Line; ++j)
        {
          df[j]='\0';
        }
      }
    }
    if(succ){
      writeSector(df,DFSector);
      *success = TRUE;
      j=div(idx,Line);
      for(i=0; i<MaxDF*Line; i+=Line){
        copy(df, curName, i+2,MAXDFN);
        if(df[i]==j && df[i+1] == Dir){
          deleteDir(curName,&dummy,j);
        }
        if(df[i]==j && df[i+1] != Dir){
          deleteFile(curName,&dummy,j);
        }
      }
    }
  }
}

void move(char *path1, char *path2, int *success, char parentIndex){
  char df[2*SectorSize];
  int i;
  int succ1;
  int succ2;
  char dirPath[MaxDF*(Line)];
  char name[MAXDFN], curName[MAXDFN];
  int parent1,parent2;
  *success =FALSE;
  readSector(df,DFSector);
  splitPath(path1, dirPath, name);
  searchDir(df, dirPath, &parent1, &succ1, parentIndex);
  searchDir(df, path2, &parent2, &succ2, parentIndex);
  if(succ1 && succ2){
    succ1 = FALSE;
    for(i=0; i<MaxDF*Line && !(succ1); i+=Line){
      copy(df, curName, i+2,MAXDFN);
      // if(df[i]==parent1){
      //   printString(curName);
      //   printStringln(name);
      // }
      if(df[i]==parent1 && isEqual(curName, name)){
        succ1 =TRUE;
        df[i] = parent2;
      }
    }
    if(succ1){
      writeSector(df,DFSector);
      *success = TRUE;
    }
  }
}

void copyDir(char *path1, char *path2, int *success, char parentIndex){
  char df[2*SectorSize];
  int i,idx;
  int succ1;
  int succ2;
  char dirPath[MaxDF*(Line)];
  char dirName[MAXDFN], name[MAXDFN],curName[MAXDFN];
  int parent1,parent2;
  *success = FALSE;
  readSector(df,DFSector);
  splitPath(path1, dirPath, dirName);
  printStringln(path1);
  printStringln(dirName);
  printStringln(dirPath);
  searchDir(df, dirPath, &parent1, &succ1, parentIndex);
  searchDir(df, path2, &parent2, &succ2, parentIndex);
  // printString("name:");
  // printStringln(dirName);
  if(succ1 && succ2){
    succ1= FALSE;
    // printString("name:");
    // printStringln(dirName);
    copy(dirName,name,0,MAXDFN);
    for(i=0; i<MaxDF*Line && !(succ1); i+=Line){
      copy(df, curName, i+2,MAXDFN);
      //printStringln("masuk sii");
      // if(df[i]==parent1){
      //   printString(curName);
      //   printString("|");
      //   printStringln(name);
      // }

      if(df[i]==parent1 && df[i+1] == Dir && isEqual(curName, name)){
        succ1 =TRUE;
        idx = div(i,Line);
        makeDir(name, &succ1,parent2);
        searchDir(df, dirPath, &parent1, &succ1, parentIndex);
      }
    }
    if(succ1){
      for(i=0; i<MaxDF*Line && !(succ1); i+=Line){
        copy(df, curName, i+2,MAXDFN);
        if(df[i]== idx){
          if(df[i+1]==Dir){
            copyDir(curName,"",&succ1,parent1);
          }else{
            copyFile(curName,"",&succ1,parent1);
          }
        }
      }

      if(succ1){
        *success = TRUE;
      }else{
        deleteDir(dirName, &succ1,parent2);
      }
    }
  }
}

void listDir( char parentIndex){
  char df[2*SectorSize];
  int i;
  char curName[MAXDFN];
  readSector(df,DFSector);
  for(i=0; i<MaxDF*Line ; i+=Line){
    copy(df, curName, i+2,MAXDFN);
    if(df[i]==parentIndex){
      if(df[i+1]==Dir){
        printString("/");
      }
      printStringln(curName);
    }
  }
 }