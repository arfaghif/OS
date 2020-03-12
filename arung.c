// load the disk map
char map[512];
fseek(floppy, 512 * 0x100, SEEK_SET);
for(i = 0; i < 512; i++) map[i] = fgetc(floppy);

// load the directory
char dir[1024];
fseek(floppy, 512 * 0x101, SEEK_SET);
for(i = 0; i < 1024; i++) dir[i] = fgetc(floppy);

// load the sector map
char sector[512];
fseek(floppy, 512 * 0x103, SEEK_SET);
for(i = 0; i < 512; i++) sector[i] = fgetc(floppy);

// find a free entry in the directory
for(i = 0; i < 1024; i = i + 16)
    if(dir[i] == 0 && dir[i + 1] == 0 && dir[i + 2] == 0) break;
if(i == 1024) {
    printf("Nggak cukup room di direktori \n");
    return;
}
int dirindex = i;

// fill the name field with 00s first
for(i = 0; i < 14; i++) dir[dirIndex + i + 2] = 0x00;
// copy the name over
for(i = 0; i < 14; i++) {
    if(argv[1][i] == 0) break;
    dir[dirindex + i + 2] = argv[1][i];
}

// mark as file on root and find unoccupied entry
for(i = 0; i < 32; i++) {
    if(sector[i * 16] == 0) break;
}
if(i == 32) {
    printf("Nggak ada entry tersedia\n");
    return;
}
int entryIndex = i;
dir[dirIndex] = 0xFF;
dir[dirIndex + 1] = entryIndex;

// mark the sector in the sectors entry
// dir[dirindex] = i;
// dirindex++;
// sectcount++;

sector[entryIndex * 16 + sectcount] = i;
sectcount++;