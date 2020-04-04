
#include "folderIO.h"
void handleInterrupt21 (int AX, int BX, int CX, int DX);



/* Ini deklarasi fungsi */


 //Fungsi untuk mengisi buffer dengan 0

int main(){
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
    bacaLogo();
    
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
        clear(input, 256);

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
                interrupt(0x21, 0x0, "Directory not found!! \n\r", 0, 0);
                
            }
        } 
        else if(isEqual(args1,"mkdir")&&!isEqual(args2,"")&&isEqual(args3,"")){
            makeDir(args2,&succ,curidx);
            if(succ){
                interrupt(0x21, 0x0, "Dir created\n\r", 0, 0);
                clear(dir,512*2);
                readSector(dir,DFSector);


                
            }else{
                interrupt(0x21, 0x0, "failed create directory!! \n\r", 0, 0);
                
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
                interrupt(0x21, 0x0, "fail cat! \n\r", 0, 0);
                
            }
        }

         else if(isEqual(args1,"rm")&&!isEqual(args2,"")&&isEqual(args3,"")){
          if(args2[0]=='/'){
            copy(args2,input,1,255);
            deleteDir(input,&succ,curidx);
          }else{
            deleteFile(args2,&succ,curidx);
          }
          if(succ){
                interrupt(0x21, 0x0, "Delete success\n\r", 0, 0);
                clear(dir,512*2);

                
            }else{
                interrupt(0x21, 0x0, "Delete failed!! \n\r", 0, 0);
                
            }
        }
        else if(isEqual(args1,"ls")&&isEqual(args2,"")){
           listDir(curidx);
         }
         else if(isEqual(args1,"mv")&&!isEqual(args2,"")&&!isEqual(args3,"")&&isEqual(args4,"")){
          if(isEqual(args3,".")){
            clear(args3,1);
          }
          if(args2[0]=='/'){
            copy(args2,input,1,255);
            move(input,args3,&succ,curidx,FALSE);
          }else{
            move(args2,args3,&succ,curidx,TRUE);
          }
          if(succ){
                printStringln("move success");
                clear(dir,512*2);


                
            }else{
                printStringln("move failed");
                
          }
        }
        else if(isEqual(args1,"cp")&&!isEqual(args2,"")&&!isEqual(args3,"")&&isEqual(args4,"")){
          if(isEqual(args3,".")){
            clear(args3,1);
          }
          if(args2[0]=='/'){
            copy(args2,input,1,255);
            copyDir(input,args3,&succ,curidx);
          }else{
            copyFile(args2,args3,&succ,curidx);
          }
          
          if(succ){
                //interrupt(0x21, 0x0, "copy succ\n\r", 0, 0);
                clear(dir,512*2);


                
            }else{
                interrupt(0x21, 0x0, "copy failed!! \n\r", 0, 0);
                
          }
        }
        else if(args1[0]=='.'&& args1[1]=='/'&&isEqual(args2,"")){
          copy(args1,input,1,255);
          executeProgram(0x2000, input,&succ,curidx);
          if(succ){
                  clear(dir,512*2);


                
            }else{
                printStringln("run failed");

          }
        }


        else{
            printStringln("Command not found");
        }
    }
//        int res;

//   // printStringln("oyyy");
//   // //printStringln(nama);
//   // readFile(ready,"a.txt",&res,0xFF);
//        makeInterrupt21();
//  executeProgram(0x1600,"shell",&res, 0xFF);
// //  // printStringln(ready);
//  makeInterrupt21();
//   while (1);
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

