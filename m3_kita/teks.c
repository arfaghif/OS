#include "teks.h"


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
  // printStringln(path);
  // printStringln(filename);
}

//Mengcopy string1 dari index begin sepanjang length ke string2
void copy(char* string1,char* string2, int begin, int length){
  	int i;
  	clear(string2,length);
	for (i=0; i< length ;++i){
	  string2[i]=string1[begin+i];
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

