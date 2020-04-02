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
void printPathDir(char* dir, int index);
void printString(char *string);
void readString(char *string);
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);
void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
void readFile(char *buffer, char *path, int *result, char parentIndex);
void clear(char *buffer, int length); //Fungsi untuk mengisi buffer dengan 0
int mod(int a, int b);
int len(char *string);
int div(int a, int b);
void bacaLogo();
int isEqual(char *s1, char*s2);
void splitPath(char *path,char* dirPath, char* filename);
void searchDir(char*dir, char * path, int* index, int* success, char parent);
void copy(char* string1,char* string2, int begin, int length);
void searchFile(char* dir, int*index, int* success,char parent);
void split(char* string, char* left, char* right, char separator);
void enter();
void printStringln(char* string);
void executeProgram(int segment, char *path, int *result, char parentIndex);
void deleteFile(char *path, int *success, char parentIndex);
void deleteDir(char *path, int *success, char parentIndex);
void makeDir(char *path, int *success, char parentIndex);
void move(char *path1, char *path2, int *success, char parentIndex);
void copyFile(char *path1, char *path2, int *success, char parentIndex);
void copyDir(char *path1, char *path2, int *success, char parentIndex);
void listDir(char parentIndex);
int main() {

   //char ready[512*16];//buffer
 //  char nama[14*64];
 //  char nama1[14*64];
 //  char map[512];
 //  char d[512*16];
 //  char f[512*16];
 //  int success,i,res;
 //  int *sector = 1;

 //   printString("nama file:");
 //   readString(nama);
 //  printString("isi file:");
 //   readString(ready);
 //  writeFile(ready,nama,&success,0xFF);

 //  readFile(d,nama,&success,0xFF);
 //  printStringln("d");
 //  printString(d);
 //  clear(d, 512*16);
 //  clear(nama, MAXDFN);
 //  clear(ready,512*16);
 // printString("nama file:");
 //   readString(nama);
 //  printString("isi file:");
 //   readString(ready);
 //  writeFile(ready,nama,&success,0xFF);

 //  readFile(d,nama,&success,0xFF);
 //  printStringln("d");
 //  printString(d);
    int done;
    char ready[512*16];
    char input[256],args1[256],args2[256],args3[256],args4[256];
    int cd;
    int i, succ, curidx, leng;
    char dir[SectorSize*2];
    makeInterrupt21();
    done =FALSE;
    curidx =0xFF;
    
    while(!done){
        //nampilin dolar kaya
        //interrupt(0x21, 0x2, dir, DFSector, 0);
        readSector(dir,DFSector);

        clear(input,256);
        // for (i=0;i<4;++i){
        //   ready[i] = dir[4*Line+i+2];
        // }
        //printStringln(ready);
        interrupt(0x21, 0x0, "Neity:-", 0, 0);
        printPathDir(dir, curidx);
        
        interrupt(0x21, 0x0, "$ ", 0, 0);

        interrupt(0x21, 0x1, input, 0,0);
        split(input,  args1,args3, 0x20);   //bagi dua dari spasi
        split(args3,  args2,input, 0x20);
        split(input,  args3,args4, 0x20);

        // printStringln(args1);
        //  printStringln(args2);
        //   printStringln(args3);
        //    printStringln(args4);


        if(isEqual(args1,"cd")&&!isEqual(args2,"")&&isEqual(args3,"")){
            readSector(dir,DFSector);

            searchDir(dir, args2, &i, &succ ,curidx);

            if(succ){
                curidx= i;
                
            }else{
                interrupt(0x21, 0x0, "Dir not found!! \n\r", 0, 0);
                
            }
        } 
        else if(isEqual(args1,"mkdr")&&!isEqual(args2,"")&&isEqual(args3,"")){
            makeDir(args2,&succ,curidx);
            if(succ){
                interrupt(0x21, 0x0, "Dir created jeeng\n\r", 0, 0);
                clear(dir,512*2);
                readSector(dir,DFSector);


                
            }else{
                interrupt(0x21, 0x0, "Dir not found!! \n\r", 0, 0);
                
            }

        }
        else if(isEqual(args1,"cat")&&!isEqual(args2,"")&&isEqual(args3,"")){
            readFile(ready,args2,&succ,curidx);
            printStringln(ready);
            clear(ready,512*16);
            if(succ==1){
                //interrupt(0x21, 0x0, "cacreated jeeng\n\r", 0, 0);
                clear(dir,512*2);


                
            }else{
                interrupt(0x21, 0x0, "fail! \n\r", 0, 0);
                
            }
        }

        //  else if(isEqual(args1,"rm")&&!isEqual(args2,"")&&isEqual(args3,"")){
        //   if(args2[0]=='/'){
        //     copy(args2,input,1,255);
        //     deleteDir(input,&succ,curidx);
        //   }else{
        //     deleteFile(args2,&succ,curidx);
        //   }
        //   if(succ){
        //         interrupt(0x21, 0x0, "Delete succ\n\r", 0, 0);
        //         clear(dir,512*2);

                
        //     }else{
        //         interrupt(0x21, 0x0, "Del failed!! \n\r", 0, 0);
                
        //     }
        // }
        else if(isEqual(args1,"ls")&&isEqual(args2,"")){
           listDir(curidx);
         }
        //  else if(isEqual(args1,"mv")&&!isEqual(args2,"")&&!isEqual(args3,"")&&isEqual(args4,"")){
        //   if(isEqual(args3,".")){
        //     args3[0]="\0";
        //   }
        //   move(args2,args3,&succ,curidx);
        //   if(succ){
        //         interrupt(0x21, 0x0, "movesucc\n\r", 0, 0);
        //         clear(dir,512*2);


                
        //     }else{
        //         interrupt(0x21, 0x0, "mv failed!! \n\r", 0, 0);
                
        //   }
        // }
        // else if(isEqual(args1,"cp")&&!isEqual(args2,"")&&!isEqual(args3,"")&&isEqual(args4,"")){
        //   if(isEqual(args3,".")){
        //     args3[0]="\0";
        //   }
        //   if(args2[0]=='/'){
        //     copy(args2,input,1,255);
        //     //printStringln(input);
        //     copyDir(input,args3,&succ,curidx);
        //   }else{
        //     copyFile(args2,args3,&succ,curidx);
        //   }
          
        //   if(succ){
        //         interrupt(0x21, 0x0, "copysucc\n\r", 0, 0);
        //         clear(dir,512*2);


                
        //     }else{
        //         interrupt(0x21, 0x0, "cp failed!! \n\r", 0, 0);
                
        //   }
        // }


        else{
            interrupt(0x21, 0x0, "Command not found!! \n\r", 0, 0);
        }
    }
  // int res;

  // printStringln("oyyy");
  // //printStringln(nama);
  // readFile(ready,"a.txt",&res,0xFF);
  // //executeProgram(0x2000,"shell",&res, 0xFF);
  // printStringln(ready);
  // makeInterrupt21();
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
      case 0x07:
          printStringln(BX);
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

void enter(){
	printString("\n\r");
}
void printStringln(char* string){
	printString(string);
	enter();
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
				interrupt(0x10,0xe*256+dashr,0,0,0);//geser satu huruf ke kiri
				interrupt(0x10,0xe*256+dashn,0,0,0);//geser satu huruf ke kiri
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


//Membandingkan dua buah string
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


//Mencari panjang suatu string
int len(char* buff){
  int length = 0;
	while (buff[length] != '\0') {
		length++;
	}
  return length;
}

//Digunakan dalam split nama file dengan pathnya
void splitPath(char *path,char* dirPath, char* filename){
  int i ,j;
  i =len(path)-1;
  j=0;
  //cari letak dash - 1
  clear(dirPath,len(dirPath));
  clear(filename,MAXDFN);
  while(i>=0 && path[i]!= '/'){
    i--;
  }
  copy(path,dirPath,0, i);
  copy(path, filename, i+1, len(path)-i-1);
}

//Mengcopy string1 dari index begin sepanjang length ke string2
void copy(char* string1,char* string2, int begin, int length){
  	int i;
  	clear(string2,len(string2));
	for (i=0; i< length ;++i){
	  string2[i]=string1[begin+i];
	}
}


//Mencari direktori. diimplementasikan, pake rekursif
void searchDir(char*dir, char * path, int* index, int* success, char parent){
  char left[MaxDF*(Line)], right[MaxDF*(Line)];
  int i;
  char dirname[MAXDFN];
  int found;
  // char ready[512*16];
  // printString("path2");
  // printStringln(path);
  // for (i=0;i<4;++i){
  //   ready[i] = dir[4*Line+i+2];
  // }
  // printStringln(ready);
  // if (parent==0x1){
  //   printStringln("dah bener");
  // }
  if(path[0]=='\0'){
  	//basis
  	//kalo udah kosng stringnya
    *index = parent;
    *success =1;
  }else{
    split(path, left,right,'/');
    // printString(path);
    // printString(left);
    // printStringln(right);
    if(left[0]=='.'&&left[1]=='.'){
    	if(parent==0xFF){
    		*success = 0;
    	}else{
    		searchDir(dir, right, index, success, dir[parent*Line]);

    	}
    }else{

	    //cari dir nya di dir.img
	    found =FALSE;
	    for(i=0; i<MaxDF*Line && !found; i+=Line){
	      copy(dir, dirname, i+2,MAXDFN);
        // if(dir[i+1]==Dir){
        //   printString(left);
        //   printStringln(dirname);
        // }

	      if(dir[i]==parent&&dir[i+1]==Dir&&isEqual(dirname, left)){
	        found= TRUE;
	        //jadiin index ini jadi parent selanjutnya
          
	        parent  = div(i,Line);
	        searchDir(dir, right, index, success, parent);//rekursif
	      }
	    }
	    if(!found){
	      *success = 0;
	    }
	   }
  }
}

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


//Membagi dua string yang dipisahkan separator
void split(char* string, char* left, char* right, char separator){
  int i,j,k;
  int found = 0;
  i=0;
  j=0;
  k=0;
  clear(left,256);
  clear(right,256);
  while(string[i]!='\0'){
    if (found){
      right[k] = string[i];
      k++;
      i++;
    }else if(string[i]==separator){
      i++;
      found = 1;
    }else{
    	//selama blm ada separator, masuk sini dlu
      left[j] = string[i];
      i++;
      j++;
      // printString("left");
      // printStringln(left);
    }
  }
  // printString("left");
      // printStringln(left);

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

// void move(char *path1, char *path2, int *success, char parentIndex){
//   char df[2*SectorSize];
//   int i;
//   int succ1;
//   int succ2;
//   char dirPath[MaxDF*(Line)];
//   char name[MAXDFN], curName[MAXDFN];
//   int parent1,parent2;
//   *success =FALSE;
//   readSector(df,DFSector);
//   splitPath(path1, dirPath, name);
//   searchDir(df, dirPath, &parent1, &succ1, parentIndex);
//   searchDir(df, path2, &parent2, &succ2, parentIndex);
//   if(succ1 && succ2){
//     succ1 = FALSE;
//     for(i=0; i<MaxDF*Line && !(succ1); i+=Line){
//       copy(df, curName, i+2,MAXDFN);
//       // if(df[i]==parent1){
//       //   printString(curName);
//       //   printStringln(name);
//       // }
//       if(df[i]==parent1 && isEqual(curName, name)){
//         succ1 =TRUE;
//         df[i] = parent2;
//       }
//     }
//     if(succ1){
//       writeSector(df,DFSector);
//       *success = TRUE;
//     }
//   }
// }

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
  // succ = FALSE;
  // *success =FALSE;
  // if (succ){
  //   splitPath(path, dirPath,dirName);
  //   succ = FALSE;
  //   searchDir(df,dirPath,&parent,&succ,parentIndex);
  //   if (succ){
  //     //cari dir
  //     succ = FALSE;
  //     for(i=0; i<MaxDF*Line && !(succ); i+=Line){
  //       copy(df, curName, i+2,MAXDFN);
  //       if(df[i]==parent && df[i+1] == Dir && isEqual(curName, dirName)){
  //         succ=TRUE;
  //         idx = div(i,Line);
  //       }
  //     }
  //     if(succ){//foldernya belum ada
  //       for(i=0; i<MaxDF*Line && !(succ); i+=Line){
  //       copy(df, curName, i+2,MAXDFN);
  //       if(df[i]==idx){
  //         if(df[i+1]==Dir){
  //           printString("/");
  //         }
  //         printStringln(curName);
  //       }
  //     }
  //   }
  // }
}