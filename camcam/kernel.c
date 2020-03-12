#define SL2 256
#define COLUMN_COUNT 80
#define rowCount 25
#define screenCharCount 2000 // 80x25
#define colorWhite 0x7
#define segmentBase 0xB000
#define memoryBase 0x8000
#define videoMemoryBase 0xB8000

/* Ini deklarasi fungsi */
void handleInterrupt21(int AX, int BX, int CX, int DX);
void printChar(char ch);
void printString(char *string);
void readString(char *string);
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);
void readFile(char *buffer, char *filename, int *success);
void clear(char *buffer, int length); // Fungsi untuk mengisi buffer dengan 0
void writeFile(char *buffer, char *filename, int *sectors);
void executeProgram(char *filename, int segment, int *success);
void clearScreen();

int mod(int a, int b);
int div(int a, int b);
int strComp(char *buffer, char *fileName);
int strlen(char *string);
void *numToString(char *ch, int number);

int main()
{
    int success, len, sectors = 1, segment = 0x2000;
    char *read, readfile[512 * 20], *spaces = "     ";
    makeInterrupt21();
    clearScreen();
    //  _    _ _____ _
    // | |  | /  ___| |
    // | |  | \ `--.| |
    // | |/\| |`--. \ |
    // \  /\  /\__/ / |____
    //  \/  \/\____/\_____/
    printString(" _    _ _____ _     \r\n");
    printString("| |  | /  ___| |    ");
    printString(spaces);
    printString("by:\r\n");
    printString("| |  | \\ `--.| |    ");
    printString(spaces);
    printString(spaces);
    printString("Moch. Nafkhan Alzamzami (13518132)\r\n");
    printString("| |/\\| |`--. \\ |    ");
    printString(spaces);
    printString(spaces);
    printString("Aditias Alif Mardiano (13518039)\r\n");
    printString("\\  /\\  /\\__/ / |____");
    printString(spaces);
    printString(spaces);
    printString("Byan Sakura Kireyna Aji (13518066)\r\n");
    printString(" \\/  \\/\\____/\\_____/\r\n");
    printString("\r\n");
    printString("Pilih salah satu menu di bawah ini!\r\n");
    printString("[1] Jalankan program milestone1\r\n");
    printString("[2] Baca file key.txt\r\n");
    printString("[3] Test write file test.txt\r\n");
    printString("[4] Test read file test.txt\r\n");
    printString("[5] Jalankan program test\r\n");
    while (1)
    {
        printString("> ");
        readString(read);
        len = strlen(read);
        printString("\r\n");
        if (read[0] == '1' && len == 1)
        {
            printString("Menjalankan perintah 1...\r\n");
            executeProgram("milestone1", segment, &success);
        }
        else if (read[0] == '2' && len == 1)
        {
            printString("Menjalankan perintah 2...\r\n");
            readFile(readfile, "key.txt", &success);
            if (success)
            {
                printString("Konten file [key.txt]: ");
                printString(readfile);
                printString("\r\n");
            }
        }
        else if (read[0] == '3' && len == 1)
        {
            printString("Menjalankan perintah 3...\r\n");
            writeFile("testIsiFile", "test.txt", &sectors);
        }
        else if (read[0] == '4' && len == 1)
        {
            printString("Menjalankan perintah 4...\r\n");
            readFile(readfile, "test.txt", &success);
            if (success)
            {
                printString("[test.txt]: ");
                printString(readfile);
                printString("\r\n");
            }
        }
        else if (read[0] == '5' && len == 1)
        {
            printString("Menjalankan perintah 5...\r\n");
            executeProgram("test", segment, &success);
        }
        else if (len > 0)
        {
            printString("Perintah tidak ditemukan!\r\n");
        }
        // read[0] = 0;
    }
}

int mod(int a, int b)
{
    //Mengembalikan sisa pembagian a oleh b
    while (a >= b)
    {
        a -= b;
    }
    return a;
}

int div(int a, int b)
{
    //Mengembalikan hasil pembagian a oleh b
    int res = 0;
    while (a >= b)
    {
        ++res;
        a -= b;
    }
    return res;
}

int strComp(char *buffer, char *fileName)
{
    int i, j, checkFound = 0;
    char ch1, ch2;

    for (i = 0; i < 16; i++)
    {
        /* If there is a file at the location */
        if (buffer[32 * i])
        {
            /* Check the first characters */
            for (j = 0; j < 12; j++)
            {
                ch1 = buffer[j + 32 * i];
                ch2 = fileName[j];

                if (ch1 == ch2)
                {
                    checkFound = 1;
                    if (!ch1)
                    {
                        break;
                    }
                }
                else
                {
                    checkFound = 0;
                    break;
                }
            }
            if (checkFound == 1)
            {
                return i;
            }
        }
    }
    if (checkFound == 0)
    {
        return 0;
    }
}

int strlen(char *string)
{
    int res = 0;
    while (string[res])
    {
        ++res;
    }
    return res;
}

void handleInterrupt21(int AX, int BX, int CX, int DX)
{
    switch (AX)
    {
    case 0x0:
        printString(BX);
        break;
    case 0x1:
        readString(BX);
        break;
    case 0x2:
        readSector(BX, CX);
        break;
    case 0x3:
        writeSector(BX, CX);
        break;
    case 0x4:
        readFile(BX, CX, DX);
        break;
    case 0x5:
        writeFile(BX, CX, DX);
        break;
    case 0x6:
        executeProgram(BX, CX, DX);
        break;
    default:
        printString("Invalid interrupt");
    }
}

void printChar(char ch)
{
    interrupt(0x10, 0xE * SL2 + ch, 0, 0, 0);
}

void printString(char *string)
{
    int i = 0;
    while (string[i])
    {
        printChar(string[i++]);
    }
}

void readString(char *string)
{
    int dashn = 0xA; // \n
    int endStr = 0x0;
    int enter = 0xD;
    int backsp = 0x8;
    int dashr = 0xD; // \r
    int count = 0;

    // string[0] = dashr;
    // string[1] = dashn;
    while (1)
    {
        /* Call interrupt 0x16 */
        /* interrupt #, AX, BX, CX, DX */
        int ascii = interrupt(0x16, 0, 0, 0, 0);
        if (ascii == enter)
        {
            string[count] = 0x0;
            string[count + 1] = dashr;
            string[count + 2] = dashn;
            return;
        }
        else if (ascii == backsp)
        {
            if (count > 0)
            {
                string[count--] = 0x0;
                interrupt(0x10, 0xE * SL2 + backsp, 0, 0, 0);
                interrupt(0x10, 0xE * SL2 + endStr, 0, 0, 0);
                interrupt(0x10, 0xE * SL2 + backsp, 0, 0, 0);
            }
        }
        else
        {
            string[count++] = ascii;
            interrupt(0x10, 0xE * SL2 + ascii, 0, 0, 0);
        }
    }
}

void readSector(char *buffer, int sector)
{
    interrupt(0x13, 0x201, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

void writeSector(char *buffer, int sector)
{
    interrupt(0x13, 0x301, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

void *numToString(char *ch, int number)
{
    int i = 0, divider = 10000;
    for (i = 0; i < 5; ++i)
    {
        ch[i] = (number / divider) + '0';
        number = mod(number, divider);
        divider = div(divider, 10);
    }
    ch[5] = 0x00;
    return ch;
}

void readFile(char *buffer, char *filename, int *success)
{
    int fileFound, nameCt = 0, index, i, j = 0, k, h, buffAddress = 0, sectors[21];
    char ch[6], dir[512];

    *success = 0;
    printString("Mencari file...\r\n");

    /* Read in the directory sector */
    readSector(dir, 2);

    /* Try to find the file name */
    fileFound = strComp(dir, filename);

    if (fileFound)
    {
        printString("File ditemukan!\r\n");

        /* 1.Using the sector numbers in the directory, load the file, sector by sector, into the buffer array.
			You should add 512 to the buffer address every time you call readSector*/
        /* Get the line that the file info is on from the buffer */
        index = fileFound * 32 + 12;
        for (j = 0; j < 20; j++)
        {
            sectors[j] = dir[index + j];
        }
        sectors[j] = 0;

        k = 0;
        while (sectors[k])
        {
            printString("Reading sector: ");
            numToString(ch, sectors[k]);
            printString(ch);
            printString("\r\n");
            readSector(buffer + buffAddress, sectors[k]);
            buffAddress += 512;
            k++;
        }
        while (k < 20)
        {
            for (j = 0; j < 512; ++j)
            {
                buffer[k * 512 + j] = 0;
            }
            ++k;
        }
        *success = fileFound;
    }
    else
    {
        printString("File tidak ditemukan!\r\n");
    }
}

void clear(char *buffer, int length)
{
    int i;

    for (i = 0; i < length; ++i)
    {
        buffer[i] = 0;
    }
}

void writeFile(char *buffer, char *filename, int *sectors)
{
    int directoryLine, j, k, index, diff, nameLen = 0, sectorNum, iterator = 0, foundFree = 0, i, h, kVal;
    char map[512], directory[512], subBuff[512];

    printString("Mulai menulis file...\r\n");

    // 1. Load the Map and Directory sectors into buffers
    readSector(map, 1);
    readSector(directory, 2);

    // 2. Find a free directory entry (one that begins with 0x00)
    for (directoryLine = 0; directoryLine < 16; directoryLine++)
    {
        /* If there isnt a file at the location */
        if (!directory[32 * directoryLine])
        {
            foundFree = 1;
            break;
        }
    }

    if (!foundFree)
    {
        printString("Memori direktori penuh!\r\n");
        return;
    }

    /*3.Copy the name to that directory entry.  If the name is less than 6 bytes, fill in the remaining bytes with 0x00*/
    /* Get the name length */
    nameLen = strlen(filename);
    /* Write in the name*/
    for (j = 0; j < nameLen; j++)
    {
        directory[32 * directoryLine + j] = filename[j];
    }
    /* Check if the file name is less than 6 chars. If so, fill the remainder with 0x0s */
    if (nameLen < 12)
    {
        diff = 12 - nameLen;
        for (j = 0; j < diff; j++)
        {
            index = j + nameLen;
            directory[32 * directoryLine + index] = 0x0;
        }
    }

    /*4.For each sector making up the file:*/
    for (k = 0; k < *sectors; k++)
    {

        /*5.Find a free sector by searching through the Map for a 0x00*/
        sectorNum = 0;
        while (map[sectorNum])
        {
            ++sectorNum;
            if (sectorNum == 512)
            {
                printString("Not enough space in directory entry for file\r\n");
                return;
            }
        }
        /*6.Set that sector to 0xFF in the Map*/
        map[sectorNum] = 0xFF;
        /*7.Add that sector number to the file's directory entry*/
        directory[32 * directoryLine + 12 + k] = sectorNum;
        /*8.Write 512 bytes from the buffer holding the file to that sector*/
        for (j = 0; j < 512; j++)
        {
            // kVal = k + 1;
            if (!(subBuff[j] = buffer[k * 512 + j]))
            {
                break;
            }
        }
        writeSector(subBuff, sectorNum);
    }
    /*9.Fill in the remaining bytes in the directory entry to 0x00*/
    while (k <= 20)
    {
        directory[32 * directoryLine + 12 + k++] = 0;
    }

    /*10.Write the Map and Directory sectors back to the disk*/
    writeSector(map, 1);
    writeSector(directory, 2);
}

void executeProgram(char *filename, int segment, int *success)
{
    // 512 * 20
    int i;
    char readingBuffer[512 * 20];

    readFile(readingBuffer, filename, success);

    if (!*success)
    {
        return;
    }

    printString("Memasukkan program ke memori...\r\n");
    for (i = 0; i < 512 * 20; i++)
    {
        putInMemory(segment, i, readingBuffer[i]);
    }

    printString("Menjalankan program...\r\n");
    launchProgram(segment);
}

void clearScreen()
{
    int i = 0, j = 100;

    /* Iterate through each character position and null it out and
	 * default the color code to white.
	 */
    while (i < screenCharCount * 2)
    {
        putInMemory(segmentBase, memoryBase + i, '\0');
        putInMemory(segmentBase, memoryBase + i + 1, colorWhite);
        i += 2;
    }

    /* Delay before returning to allow the screen to catch up */
    while (--j)
        ;

    // Reset cursor ke 0,0
    interrupt(0x10, 0x2 * SL2 + 0x0, 0, 0x0, 0);

    return;
}
