#include "teks.h"

// void printPathDir(char* dir, int index);
// void clear(char *buffer, int length);
// void searchDir(char*dir, char * path, int* index, int* success, char parent);
// void split(char* string, char* left, char* right, char separator);
// int isEqual(char *s1, char*s2);
// void copy(char* string1,char* string2, int begin, int length);
// int len(char *string);
// int div(int a, int b);



int main(){
  printStringln("plis lah");

}

// int main(){
// 	int done;
//     char input[256],left[256],right[256];
//     int cd;
//     int i, succ, curidx, leng;
//     char dir[SectorSize*2];
//     done =FALSE;
//     curidx =0xFF;
//     interrupt(0x21, 0x2, dir, DFSector, 0);
//     while(!done){
//         //nampilin dolar kaya
//         clear(left,256);
//         clear(right,256);
//         interrupt(0x21, 0x0, "Neity:-", 0, 0);
//         printPathDir(dir, curidx);
//         interrupt(0x21, 0x0, "$ ", 0, 0);

//         interrupt(0x21, 0x1, input, 0,0);
//         split(input,  left,right, 0x20);   //bagi dua dari spasi
//         leng = len(left);
//         if(left[0]=='c' && left[1]=='d' &&leng==2){
//             searchDir(dir, right, &i, &succ ,curidx);
//             if(succ){
//                 curidx= i;
                
//             }else{
//                 interrupt(0x21, 0x0, "Dir not found!! \n\r", 0, 0);
                
//             }
//         } else{
//             interrupt(0x21, 0x0, "Command not found!! \n\r", 0, 0);
//         }
//     }
// }

// /*Menggunakan rekursif*/
// void printPathDir(char* dir, int index){
// 	char name[MAXDFN];
// 	if((index >  0 && index < MaxDF)|| index<0){
// 		printPathDir(dir, dir[index*Line]);
// 		interrupt(0x21, 0x0, "/", 0, 0);
// 		copy(dir,name, index*Line+2, MAXDFN);
// 		interrupt(0x21, 0x0, name, 0, 0);
// 	}
// }

// void clear(char* buffer, int length) {
// 	int i;
// 	for(i = 0; i < length; ++i) {
// 		buffer[i] = 0x0;
// 	}
// }

// //Membandingkan dua buah string
// int isEqual(char *s1, char*s2){
//   int i, eq;
//   eq= TRUE;
//   i=0;
//   while(s1[i] != '\0' && s2[i] != '\0' && eq){
//     if(s1[i]!=s2[i]){
//       eq=FALSE;
//     }
//     i++;
//   }
//   if(s1[i] != '\0' || s2[i] != '\0'){
//     eq=FALSE;
//   }
//   return eq;
// }

// //Mengcopy string1 dari index begin sepanjang length ke string2
// void copy(char* string1,char* string2, int begin, int length){
//   	int i;
//   	clear(string2,len(string2));
// 	for (i=0; i< length ;++i){
// 	  string2[i]=string1[begin+i];
// 	}
// }

// //Membagi dua string yang dipisahkan separator
// void split(char* string, char* left, char* right, char separator){
//   int i,j,k;
//   int found = 0;
//   i=0;
//   j=0;
//   k=0;
//   clear(left,len(left));
//   clear(right,len(right));
//   while(string[i]!='\0'){
//     if (found){
//       right[k] = string[i];
//       k++;
//       i++;
//     }else if(string[i]==separator){
//       i++;
//       found = 1;
//     }else{
//     	//selama blm ada separator, masuk sini dlu
//       left[j] = string[i];
//       i++;
//       j++;
//     }
//   }
// }


// //Mencari direktori. diimplementasikan, pake rekursif
// void searchDir(char*dir, char * path, int* index, int* success, char parent){
//   char left[MaxDF*(Line)], right[MaxDF*(Line)];
//   int i;
//   char dirname[MAXDFN];
//   int found;
//   if(path[0]=='\0'){
//   	//basis
//   	//kalo udah kosng stringnya
//     *index = parent;
//     *success =1;
//   }else{
//     split(path, left,right,'/');
//     if(left[0]=='.'&&left[1]=='.'){
//     	if(parent==0xFF){
//     		*success = 0;
//     	}else{
//     		searchDir(dir, right, index, success, dir[parent*Line]);

//     	}
//     }else{

// 	    //cari dir nya di dir.img
// 	    found =FALSE;
// 	    for(i=0; i<MaxDF && !found; i+=Line){
// 	      copy(dir, dirname, i+2,MAXDFN);
// 	      if(dir[i]==parent&&dir[i+1]==Dir&&isEqual(dirname, left)){
// 	        found= TRUE;
// 	        //jadiin index ini jadi parent selanjutnya
// 	        parent  = div(i,Line);
// 	        searchDir(dir, right, index, success, parent);//rekursif
// 	      }
// 	    }
// 	    if(!found){
// 	      *success = 0;
// 	    }
// 	   }
//   }
// }

// int len(char* buff){
//   int length = 0;
// 	while (buff[length] != '\0') {
// 		length++;
// 	}
//   return length;
// }

// int div(int a, int b){
// 	int result=0;
// 	while(a>=b){
// 		a-=b;
// 		result +=1;
// 	}
// 	return result;
// }