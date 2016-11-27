//-----------------------------------------------------------------------------
// File      : Stego_V1_0.c
// Author    : Marcus
// Time-stamp: <06.03.2012>
// Version   : v1.0
//-----------------------------------------------------------------------------
// Description : Verschtekt einen Text in eine BMP 24-Bit datei
//
//-----------------------------------------------------------------------------
// Document history:
//    - Erste version 06.03.2012
//
//-----------------------------------------------------------------------------
// Todo :
//   1 - Eingabe des Bild namens
//   2 -
//   3 -
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "divA.h"



typedef unsigned char byte;
FILE *source, *stego_rb, *stego_wb, *textOutput, *textInput;
char shortText[300];
int textLength, charNr, readLength;
byte puffer;

void openStegoPic_rb()
{
    stego_rb = fopen("Output\\Decrypt_Pic.bmp", "rb");

    if (stego_rb== NULL)
    {
        printf("------------------------------------------------\n");
        printf("Failed to open file: \"Decrypt_Pic.bmp\"\n");
        printf("Please check name of picture in file: \"Output\"\n");
        printf("Try again\n");
        printf("------------------------------------------------\n\n");
        displayMainMenu();
    }
}

void openStegoPic_wb()
{
    stego_wb = fopen("Output\\Decrypt_Pic.bmp", "wb");
}

void openOrginalPic()
{
    source = fopen("Input\\Orginal_Pic.bmp", "rb");

    if (source== NULL)
    {
        printf("------------------------------------------------\n");
        printf("Failed to open file: \"Orginal_Picture.bmp\"\n");
        printf("Please check name of picture in file: \"Input\"\n");
        printf("Try again\n");
        printf("------------------------------------------------\n\n");
        displayMainMenu();
    }
}

void openInputText()
{
    textInput=fopen("Input\\Text_Input.txt","r");

    if (textInput== NULL)
    {
        printf("------------------------------------------------\n");
        printf("Failed to open file: \"Text_Input.txt\"\n");
        printf("Please check name of .txt  in file: \"Input\"\n");
        printf("Try again\n");
        printf("------------------------------------------------\n\n");
        displayMainMenu();
    }
}

void openOutputText()
{
    textOutput = fopen("Output\\Text_Output.txt","w");
}

void closeFile(int n)
{
    if(n==1)fclose (stego_rb);
    if(n==2)fclose (stego_wb);
    if(n==3)fclose (source);
    if(n==4)fclose (textOutput);
    if(n==5)fclose (textInput);
}

/***************************************
* Kopiert Bildinformationen ohne       *
* Veränderung                          *
* in eine neu erstellte *.bmp Datei    *
***************************************/
void writePicInf()
{
    fread(&puffer,sizeof(byte),1,source);
    fwrite(&puffer,sizeof(byte),1,stego_wb);
}

/***************************************
* Schreibt die Anzahl Zeichen des      *
* Textes nach dem Header.Dafür wird ein*
* int reserviert                       *
* k: 32/2 = 16                         *
***************************************/
void writeTextLength(int charNr)
{
    int k, mask=0;

    for (k=0;k<16;k++)
    {
        fread(&puffer,sizeof(byte),1,source);
        puffer = puffer & 0xFC;
        mask= charNr & 0x03;
        puffer= puffer | mask;
        charNr = charNr>>2;
        fwrite(&puffer,sizeof(byte),1,stego_wb);
    }
}

/***************************************
* Verschlüsselt einen Buchstaben in 8  *
* Byte des neuen Bildes                *
* k: 8/2=4 => vier Durchläufe für einen*
* Buchstaben                           *
***************************************/
void writeEncryptData(byte oneLetter)
{
    byte mask=0;
    int k;

    for (k=0;k<4;k++)
    {
        fread(&puffer,sizeof(byte),1,source);
        puffer = puffer & 0xFC;
        mask= oneLetter & 0x03;
        puffer= puffer | mask;
        oneLetter = oneLetter>>2;
        fwrite(&puffer,sizeof(byte),1,stego_wb);
    }
}

void readPicInf()
{
    fread(&puffer,sizeof(byte),1,stego_rb);
}

/***************************************
* Entschlüsselt Textlänge und          *
* speichert diese in die Variable      *
* readLength last2bit dient als Maske  *
***************************************/
void readTextLength()
{
    int k,last2bit=0;

    for (k=0;k<31;k=k+2)
    {
        fread(&puffer,sizeof(byte),1,stego_rb);
        last2bit=puffer & 0x03;
        last2bit=last2bit<<k;
        readLength=readLength|last2bit;
        last2bit=0;
    }
}

/****************************************
* Entschlüselt einen Buchstaben und     *
* schreibt diese in die Output-Textdatei*
* k: Wird zum Plazieren der 2 Bit(k+2)  *
* und Anzahl Durchläufe benötigt        *
****************************************/
void readEncryptData()
{
    int k;
    byte last2bit=0, oneLetter=0;

    for (k=0;k<7;k=k+2)
    {
        fread(&puffer,sizeof(byte),1,stego_rb);
        last2bit=puffer & 0x03;
        last2bit=last2bit<<k;
        oneLetter=oneLetter|last2bit;
        last2bit=0;
    }
    fprintf(textOutput, "%c", oneLetter);
    oneLetter=0;
}

void consoleText()
{
    empty_stdin();
    printf("You can enter 300 character\n");
    printf("---------------------------\n\n\n");
    printf("Please enter your text: ");
    gets(shortText);
}

void testLength()
{
    textLength = strlen(shortText);

/* shortText hat Platz für 301 Zeichen
   Die 301.Stelle wird benötigt, um auf eine zu grosse Eingabe
   des Benuzers zu prüfen*/

    if(textLength>300)
    {
        empty_stdin();
        printf("-----------------------------------------------\n");
        printf("ERROR!!!!! Text is too long\n\a");
        printf("-----------------------------------------------\n\n");
        displayMainMenu();
    }
}
/****************************************
* Hauptfunktion zum Verschlüsseln des   *
* Konsolen-Textes                       *
* ACHTUNG: i enspricht nach dem Auslesen*
* des Headers nicht mehr der Anzahl     *
* Byte, die ausgelesen wurden           *
* j: wird benötigt um den Buchstaben zu *
* wechseln                              *
****************************************/
void simpleEncrypt()
{
    int j=0,i=0;
    byte oneLetter;

    system("cls");
    openOrginalPic();
    openStegoPic_wb();

    consoleText();
    system("cls");
    testLength();

    while (!feof(source))
    {
        if(i<=53) writePicInf();
        if(i==54) writeTextLength(textLength);
        if (i>54 && i<=54+textLength)
        {
            oneLetter=shortText[j];
            writeEncryptData(oneLetter);
            j++;
        }
        if (i>=54+textLength) writePicInf();
        i++;
    }
    closeFile(2);
    closeFile(3);

    printf("-------------------------------------------------------------\n");
    printf("Your text is successfully encrypted\n");
    printf("You find the picture in \\Stego_V1.0\\Output\\Decrypt_Pic.bmp\n");
    printf("--------------------------------------------------------------\n\n");
}

/***************************************
* Zählt Zeichen in der Inputtext-Datei *
***************************************/
int testTextLength()
{
    int counter;
    openInputText();
    fseek(textInput, 0L, SEEK_END);
    counter = ftell(textInput);
    closeFile(5);
    return counter;
}

/****************************************
* Haupfunktion zum Verschlüsseln der    *
* Inputtext-Datei                       *
* ACHTUNG: i enspricht nach dem Auslesen*
* des Headers nicht mehr der Anzahl     *
* Byte, die ausgelesen wurden           *
****************************************/
void textFileEncrypt()
{
    int i=0, fileLength;
    byte textLetter;

    system("cls");
    fileLength = testTextLength();

    openOrginalPic();
    openStegoPic_wb();
    openInputText();

    while(!feof(source))
    {
        if(i<=53) writePicInf();
        if(i==54) writeTextLength(fileLength);
        if(i>54 && i<=54+fileLength)
        {
            fread(&textLetter,sizeof(byte),1,textInput);
            writeEncryptData(textLetter);
        }
        if (i>=54+fileLength) writePicInf();
        i++;
    }
    closeFile(2);
    closeFile(3);
    closeFile(5);

    printf("----------------------------------------------------------\n");
    printf("Your text is successfully encrypted\n");
    printf("You find the picture in \\Stego_V1.0\\Output\\Decrypt_Pic.bmp\n");
    printf("----------------------------------------------------------\n");
}
/****************************************
* Hauptfunktion zum Entschlüsseln des   *
* stego-Bildes                          *
* ACHTUNG: i enspricht nach dem Auslesen*
* des Headers nicht mehr der Anzahl     *
* Byts, die ausgelesen wurden           *
***************************************/
void decrypt()
{
    int i=0;

    system("cls");
    openStegoPic_rb();
    openOutputText();

    while (!feof(stego_rb))
    {
        if(i<=53) readPicInf();
        if (i==54)readTextLength();
        if(i>54 && i<=54+readLength) readEncryptData();
        if(i>54+readLength) readPicInf();
        i++;
    }
    closeFile(1);
    closeFile(4);

    printf("-------------------------------------------------------------------\n");
    printf("Your decrypted text is saved in \\Stego_V1.0\\Output\\Output_Text.txt\n");
    printf("-------------------------------------------------------------------\n\n");
}

int displayMainMenu ()
{
    int value, check=0;

    printf ("Steganography Programm V 1.0\n");
    printf ("----------------------------\n\n");
    printf ("Simple encrypt                        : 1\n");
    printf ("-----------------------------------------\n");
    printf ("Text file encrypt                     : 2\n");
    printf ("-----------------------------------------\n");
    printf ("Decrypt                               : 3\n");
    printf ("-----------------------------------------\n");
    printf ("Quit                                  : 0\n");
    printf ("-----------------------------------------\n\n");
    printf ("Enter your choice : ");
    check=scanf("%i", &value);
    system("cls");

    if (check==1)
    {
        if(value==0) return value;
        if(value==1) return value;
        if(value==2) return value;
        if(value==3) return value;
        printf("-----------------\n");
        printf("Wrong choise!!!!!\n");
        printf("Pleas try again\n");
        printf("-----------------\n\n");
        displayMainMenu();
    }
    else
    {
        empty_stdin();
        printf("-----------------\n");
        printf("Wrong choise!!!!!\n");
        printf("Pleas try again\n");
        printf("-----------------\n\n");
        displayMainMenu();
    }
}

int main()
{
    int menuItem = 0;

	while (1)
	{
		menuItem = displayMainMenu();
		if (menuItem == 0) exit(0);
		if (menuItem == 1) simpleEncrypt();
		if (menuItem == 2) textFileEncrypt();
		if (menuItem == 3) decrypt();
    }
    return 0;
}
