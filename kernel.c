#define MaxByte 256
#define SectorSize 512
#define MaxDF 64		
#define MAXDFN 14	
#define MapSector 0x100	
#define DFSector 0x101	
#define SectorSector 0x103	 
#define Root 0xFF		
#define Dir 0xFF
#define Line 16
#define TRUE 1
#define FALSE 0
#define ALREADY_EXIST -1
#define INSUFFICIENT_FILES_ENTRY -2
#define INSUFFICIENT_SECTOR -3
#define DIR_NOT_FOUND -4
#define SUCCESS 1
#define File_NOT_FOUND -1



/* Ini deklarasi fungsi */
void handleInterrupt21 (int AX, int BX, int CX, int DX);
void printString(char *string);
void readString(char *string);
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);
int stringComp(char* buffer, char* fileName);
void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
void readFile(char *buffer, char *path, int *result, char parentIndex);
void clear(char *buffer, int length); //Fungsi untuk mengisi buffer dengan 0
void executeProgram(char *filename, int segment, int *success);
int mod(int a, int b);
int len(char *string);
int div(int a, int b);
void bacaLogo();
int isEqual(char *s1, char*s2);
void splitPath(char *path,char* dirPath, char* filename);
void searchDir(char*dir, char * path, char* index, char* success, char parent);
void copy(char* string1,char* string2, int begin, int length);
void searchFile(char* dir, char*index, char* success,char parent);
void split(char* string, char* left, char* right, char separator);

int main() {

  // /char ready[512];//buffer
  // int success;
  //readString(ready);
  //int sector ;
  char ready[512];//buffer
  int success;
  int *sector = 1;
  printString("HELLO WORLD\n\r");
  printString("Naufal\n\r");
  printString("Vincent\n\r");
  printString("Falah\n\r");
  // if(success){
  //       printString("Alhamdulillah\n\r");
  //   }
  // printString("isi file milestone1: ");
  // printString(ready);
  printString("\n\r");
  printString("arkavidia\n\r");
  bacaLogo();

  //writeFile("aku suka diaaaa","suka.txt",sector);
  makeInterrupt21();
  while (1);

  //writeFile("aku suka diaaaa","suka.txt",sector);
 	//writeFile(ready, "yeay", &sector,0XFF);
  //makeInterrupt21();
  while (1);

}

void handleInterrupt21 (int AX, int BX, int CX, int DX){
   char AL, AH;
   AL = (char) (AX);
   AH = (char) (AX >> 8);
   switch (AL) {
      case 0x00:
         printString(BX);
         break;
      case 0x01:
         readString(BX);
         break;
      case 0x02:
         readSector(BX, CX);
         break;
      case 0x03:
         writeSector(BX, CX);
         break;
      case 0x04:
         readFile(BX, CX, DX, AH);
         break;
      case 0x05:
         writeFile(BX, CX, DX, AH);
         break;
      case 0x06:
         executeProgram(BX, CX, DX, AH);
         break;
      default:
         printString("Invalid interrupt");
   }
}

void printString(char *string){ 
    int i = 0;
    while(string[i] != '\0'){
            int character = string[i];
            // akan melakukan interrupt AX, BX, CX, DX
            interrupt(0x10, 0xe*256+character, 0, 0, 0);
            i++;
    }
}


void readString(char *string){
  int dashn = 0xa;
  int dashr = 0xd;
    int _end = 0x0;
    int enter = 0xd;
    int b_space = 0x8;
    int count = 0;
    while(1){
            /* Call interrupt 0x16 */
            /* interrupt #, AX, BX, CX, DX */
            int ascii = interrupt(0x16,0,0,0,0);
            if (ascii == dashr){              
				string[count] = _end;
				//supaya pada layar new line dan balik lagi ke pojok kiri
                return;
            }
            else if (ascii == b_space){
                if (count >= 1){
                    string[count] = _end;//Menghapus char terakhir dari string
                    count--;
                    interrupt(0x10,0xe*256+b_space,0,0,0);//geser satu huruf ke kiri
                    count++;
                    interrupt(0x10,0xe*256+_end,0,0,0);//menghapus huruf yg ditampilkan pada layar
                    count--;
                    interrupt(0x10,0xe*256+b_space,0,0,0);//geser lagi ke kiri satu huruf
                    
               }
            }
            else{
                string[count] = ascii;
                interrupt(0x10, 0xe*256+ascii, 0, 0, 0);
                count++;
            }     
    }
}

void readSector(char *buffer, int sector) {
	//Dari spek
	interrupt(0x13, 0x201, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

void writeSector(char *buffer, int sector) {
	//Dari spek
	interrupt(0x13, 0x301, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

void clear(char* buffer, int length) {
	int i;
	for(i = 0; i < length; ++i) {
		buffer[i] = 0x0;
	}
}

void executeProgram(char *filename, int segment, int *success) {
  int i;
  char buffer[512];

  readFile(buffer,filename,success);
  if (*success){
    for (i=0; i<512;i++){
      putInMemory(segment,i,buffer[i]);
    } 

    launchProgram(segment);
  }
}

int mod(int a, int b){
	while (a>=b){
		a-=b;
	}
	return a;
}
int div(int a, int b){
	int result=0;
	while(a>=b){
		a-=b;
		result +=1;
	}
	return result;
}

void bacaLogo() {

    printString ("                        ,,                                             \n\r");
    printString ("`7MN.   `7MF'           db    mm                 .g8u8q.    .Muuuubgd  \n\r");
    printString ("  MMN.    M                   MM               .dP'    `YM. ,MI    uY  \n\r");
    printString ("  M YMb   M   .gPuYa  `7MM  mmMMmm  `7M'   `MF`dM`      `MM `MMb.      \n\r");
    printString ("  M  `MN. M  ,M'   Yb   MM    MM      VA   ,V  MM        MM   `YMMNq.  \n\r");
    printString ("  M   `MM.M  8Muuuuuu   MM    MM       VA ,V   MM.      ,MP .     `MM  \n\r");
    printString ("  M     YMM  YM.    ,   MM    MM        VVV    `Mb.    ,dP' Mb     dM  \n\r");
    printString (".JML.    YM   `Mbmmd' .JMML.  `Mbmo     ,V       `ubmmdu'   PuYbmmdu   \n\r");
    printString ("                                       ,V                              \n\r");
    printString ("                                      OO                               \n\r");

}



int isEqual(char *s1, char*s2){
  int i, eq;
  eq= TRUE;
  i=0;
  while(s1[i] != '\0' && s2[i] != '\0' && eq){
    if(s1[i]!=s2[i]){
      eq=FALSE;
    }
    i++;
  }
  if(s1[i] != '\0' || s2[i] != '\0'){
    eq=FALSE;
  }
  return eq;
}

int len(char* buff){
  int length = 0;
	while (buff[length++] != '\0') {
	}
  return length;
}

void splitPath(char *path,char* dirPath, char* filename){
  int i ,j;
  i =len(path)-1;
  j=0;
  while(i>=0 && path[i]!= '/'){
    i--;
  }
  copy(path,dirPath,0, i);
  copy(path, filename, i+1, len(path)-i-1);
}


void copy(char* string1,char* string2, int begin, int length){
  int i;
  //hapus dlu
  clear(string2,len(string2));
  //mulai kopi
  if(begin <len(string1)){
    for (i=begin; i<length+begin && string1[i] != '\0';++i){
      string2[i-begin]=string1[i];
    }
  }
}


void searchDir(char*dir, char * path, char* index, char* success, char parent){
  char left[MaxDF*(Line)], right[MaxDF*(Line)];
  int i;
  char dirname[MAXDFN];
  int found;
  if(path[0]=='\0'){
    *index = parent;
    *success =TRUE;
  }else{
    split(path, left,right,'/');
    //samakan yg kiri dengan nama dir yg tersedia
    for(i=0; i<MaxDF && !found; i+=Line){
      copy(dir, dirname, i+2,MAXDFN);
      if(i==parent&&i+1==Dir&&isEqual(dirname, left)){
        found= TRUE;
        parent  = div(i,Line);
        searchDir(dir, right, index, success, parent);//rekursif
      }
    }
    if(!found){
      *success = FALSE;
    }
  }
}

void searchFile(char* dir, char* fileName,char*index, char* success,char parent){
  char filename[MAXDFN];
  int found,i;
  for(i=0; i<MaxDF && !found; i+=Line){
    copy(dir, filename, i+2,MAXDFN);
    if(i==parent&&i+1!=Dir&&isEqual(filename, fileName)){
      found= TRUE;
      *success=TRUE;
      *index= i+1;
    }
  }
    if(!found){
      *success = FALSE;
    }
}

void split(char* string, char* left, char* right, char separator){
  int i,j,k;
  int found = FALSE;
  i=0;
  j=0;
  k=0;
  clear(left,MaxDF*Line);
  clear(right,MaxDF*Line);
  while(string[i]!='\0'){
    if (found){
      right[k] = string[i];
      k++;
      i++;
    }else if(string[i]==separator){
      i++;
      found = TRUE;
    }else{
      left[j] = string[i];
      i++;
      j++;
    }
  }
}


void writeFile(char *buffer, char *path, int *sectors, char parentIndex){
  char df[SectorSize], map[SectorSize], sec[SectorSize];
  char dirPath[MaxDF*(Line)];
  char fileName[MaxDF];
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

      searchDir(df,dirPath,&parent,&succ,parentIndex);//TO DO

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
            }
          }
          lenFile= len(fileName);
          
          for(i=0;i<lenFile;++i){
            df[idx *Line+i+2]=fileName[i];
          }
          for(i=0;i<needSec;++i){
            //Cari sektor kosong
            for (j=0; j<SectorSize&& map[j]!=0x00;++j){
            }
            map[j] = 0xFF;
            sec[idxSec*Line+i] = j;
            copy(buffer,buff,i*SectorSize,SectorSize);
            writeSector(buff, j);
          }
          writeSector(df,DFSector);
          writeSector(map,MapSector);
          writeSector(sec,SectorSector);
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
  char df[SectorSize], sec[SectorSize], map[SectorSize];
  char dirPath[MaxDF*Line];
  char fileName[MAXDFN];
  int parent,idx,i;
  int found;

  //Baca semua sektor
  readSector(df,DFSector);
  readSector(map, MapSector);
  readSector(sec, SectorSector);
  splitPath(path, dirPath,fileName);
  searchDir(df,dirPath,&parent,&found,parentIndex);
  if(found){
    searchFile(df,fileName,&idx,&found,parent);
    if(found){
      for(i=0;i<16&&sec[idx*Line + i]!='\0';++i){
        readSector(buffer+i*SectorSize,sec[idx*Line+i]);
      }
      *result = SUCCESS;
    }
    else{
      *result = File_NOT_FOUND;
    }
  }else{
    *result= File_NOT_FOUND;
  }
}












