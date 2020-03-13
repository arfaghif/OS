#define TRUE 1
#define FALSE 0
#define DirSize 1024
#define DirSector 0x101


int main(){
    int done;
    char input[256],left[256],right[256];
    char cd[2];
    int i, succ, curidx;
    char dir[DirSize];
    cd[0] = "c";
    cd[2] ="d";
    done =FALSE;
    curidx =0xFF;
    while(!done){
        //nampilin dolar kaya
        interrupt(0x21, 0x0, "$ ", 0, 0);
        interrupt(0x21, 0x1, input, 0,0);
        split(input,  left,right, 0x20);   //bagi dua dari spasi
        if(IsEqual(cd, left)){
            interrupt(0x21, 0x2, dir, DirSector, 0);
            sarchDir(dir, right, &i, &succ ,curidx);
            if(succ){
                curidx= i;
            }else{
                interrupt(0x21, 0x0, "Dir not found!! \n\r", 0, 0);
                printPathDir(curidx)//TO DO
            }
        } else{
            interrupt(0x21, 0x0, "Command not found!! \n\r", 0, 0);
        }
    }
}