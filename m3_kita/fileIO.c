#include "fileIO.h"

void searchFile(char* dir, char* fileName,int*index, int* success,char parent){
  char filename[MAXDFN];
  int i;
  *success = FALSE;
  //cari file nya
  for(i=0; i<MaxDF*Line && !(*success); i+=Line){
    copy(dir, filename, i+2,MAXDFN);
    if(dir[i]==parent && dir[i+1] != Dir && isEqual(filename, fileName)){
      *success=TRUE;
      *index= dir[i+1];
    }
  }

}

void writeFile(char *buffer, char *path, int *sectors, char parentIndex){
  char df[2*SectorSize], map[SectorSize], sec[SectorSize];
  char dirPath[MaxDF*(Line)];
  char fileName[MAXDFN];
  char buff[SectorSize];
  int i, j, count, idx,idxSec;
  int lenBuf,lenFile;
  char succ;
  int parent;
  int needSec;

  //Baca semua sektor
  readSector(df,DFSector);
  readSector(map, MapSector);
  readSector(sec, SectorSector);

  //Hitung jumlah sektor kosong di map
  count =0;
  for(i=0; i<SectorSize;++i){
    if(map[i]==0x00){
      count++;
    }
  }

  //hitung butuh berapa sector
  lenBuf = len(buffer);
  needSec = 0;
  while(needSec*SectorSize < lenBuf ){
    ++needSec;
  }

  if(count>needSec){
    //Cek adakah entri kosong dalam dir.img
    succ = FALSE;
    i=0;
    do {
      if (df[i] == '\0') {
				succ = TRUE;
				idx = div(i, 16);
      }
      i+=Line;
    }while (!succ&&i<SectorSize);
    if(succ){
      splitPath(path, dirPath,fileName);

      searchDir(df,dirPath,&parent,&succ,parentIndex);

      if(succ){
        searchFile(df,fileName, &i, &succ, parent);
      
        //Jika ada nama file yg sama
        if(succ){
          *sectors =ALREADY_EXIST;
        }
        else{

          df[idx*Line] = parent;
          //cari sektor yg kosng d sector.img
          for(i=0;i<SectorSize;i+=Line){
            if (sec[i] == '\0') {
              idxSec =div(i, Line);
              df[idx*Line+1] = idxSec;
              break;
            }
          }
          lenFile= len(fileName);
          
          for(i=0;i<lenFile;++i){
            df[idx *Line+i+2]=fileName[i];
          }
          for(i=0;i<needSec;++i){
            //Cari sektor kosong d system.img buat nulisnys
            for (j=0; j<SectorSize&& map[j]!=0x00;++j){
            }
            map[j] = 0xFF;
            //masukkan ke line yg dipilih d sektor.img lokasi sektornya
            sec[idxSec*Line+i] = j;
            copy(buffer,buff,i*SectorSize,SectorSize);
            writeSector(buff, j);
          }
          writeSector(df,DFSector);
          writeSector(map,MapSector);
          writeSector(sec,SectorSector);
          *sectors = SUCCESS;//asumsi aa klo sukses returnnya 1
        }
      }else{
        *sectors = DIR_NOT_FOUND;
      }
    }else{
      *sectors = INSUFFICIENT_FILES_ENTRY;
    }


  }else{
    *sectors =INSUFFICIENT_SECTOR;
  }
}

void readFile(char *buffer, char *path, int *result, char parentIndex) {
  // deklarasi variabel-variabel yang dibutuhkan
  char df[2 * SectorSize], sec[SectorSize], map[SectorSize];
  char dirPath[MaxDF * Line];
  char fileName[MAXDFN];
  char curSec[SectorSize];
  int parent, idx, i, j;
  int found;
	//printStringln("masuk sinu");
  // melakukan pembacaan semua sektor
  readSector(df, DFSector);
  readSector(map, MapSector);
  readSector(sec, SectorSector);

  splitPath(path, dirPath, fileName);
  searchDir(df, dirPath, &parent, &found, parentIndex);
  
  if(found) { // pengecekan pertama
  	
    searchFile(df, fileName, &idx, &found, parent);
    //printStringln("masuk sine");
    if(found) { // pengecekan kedua
    	//printStringln("masuk sinueee");
      for (i = 0; i < 16 && sec[idx * Line + i] != '\0'; ++i) {
      	// sektor baca dulu ke current sector. trus satu persatu current sector tambahin ke buffer
        //printStringln("ko gamasuk masuk sino");
        readSector(curSec, sec[idx * Line + i]);
        j = 0;
        for(j = 0;j < SectorSize && curSec[j] != '\0'; ++j) {
        	buffer[i * SectorSize + j] = curSec[j];
        }
        //printStringln("masuk sini");
      }
      *result = SUCCESS; // asumsi aja kalo berhasil return nya 1
    }
    else {
    	//printStringln("nyebelin ke szini");
      *result = File_NOT_FOUND; // kalau nggak ketemu pada saat pengecekan pertama ini return aja -1 sesuai preprocessor keyword
    }
  }
  else {
    *result= File_NOT_FOUND; // kalau nggak ketemu pada saat pengecekan kedua ini return juga -1
  }
}

/*execute program*/
void executeProgram(int segment, char *path, int *result, char parentIndex){
	char buff[SectorSize*Line];
	int i;
  printStringln("uwu");
	readFile(buff,path,result,parentIndex);
	if(*result==TRUE){
    printStringln("uwu");
    //printStringln(buff);
		for(i=0;i<SectorSize*Line;++i){
			putInMemory(segment,i,buff[i]);
		}
		launchProgram(segment);
	}else{
    printStringln("gagal");
  }

}


void deleteFile(char *path, int *success, char parentIndex){
  char df[2 * SectorSize], sec[SectorSize], map[SectorSize];
  char dirPath[MaxDF * Line];
  char fileName[MAXDFN], curName[MAXDFN];
  char curSec[SectorSize];
  int parent, idx, i, j,k;
  int found;
  //printStringln("masuk sinu");
  // melakukan pembacaan semua sektor
  readSector(df, DFSector);
  readSector(map, MapSector);
  readSector(sec, SectorSector);

  splitPath(path, dirPath, fileName);
  searchDir(df, dirPath, &parent, &found, parentIndex);
  
  if(found) { // pengecekan pertama
    
    searchFile(df, fileName, &idx, &found, parent);
    //printStringln("masuk sine");
    if(found) { // pengecekan kedua
      //printStringln("masuk sinueee");
      for (i = 0; i < Line&& sec[idx * Line + i] != '\0'; ++i) {
        // sektor baca dulu ke current sector. trus satu persatu current sector tambahin ke buffer
        //printStringln("ko gamasuk masuk sino");
        k=sec[idx * Line + i];
        sec[idx * Line + i] = '\0';
        map[k] = '\0';
        readSector(curSec, k);
        clear(curSec,SectorSize);
        writeSector(curSec,k);
      }
      found = FALSE;
      //cari file nya
      for(i=0; i<MaxDF*Line && !(found); i+=Line){
        copy(df, curName, i+2,MAXDFN);
        if(df[i]==parent && df[i+1] != Dir && isEqual(curName, fileName)){
          found=TRUE;
          for(j=i; j<i+Line;++j){
            df[j]='\0';
          }
        }
      }
      writeSector(df,DFSector);
      writeSector(map,MapSector);
      writeSector(sec,SectorSector);
      *success = TRUE;
    }
  }
}


void copyFile(char *path1, char *path2, int *success, char parentIndex){
  char df[2*SectorSize], buff[SectorSize*Line];
  int i;
  int succ1, succ2;
  char dirPath[MaxDF*(Line)];
  char fileName[MAXDFN];
  int parent;
  *success = FALSE;
  readSector(df,DFSector);
  splitPath(path1, dirPath, fileName);
  readFile(buff, path1,&succ1, parentIndex);
  searchDir(df, path2, &parent, &succ2, parentIndex);
  if(succ1==1 && succ2){
    writeFile(buff,fileName,&succ1,parent );
    if(succ1){;
      *success =TRUE;
    }
  }
}