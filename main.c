/*
 * main.c: Software for EE316 Project 6. This program converts the keyboard scancode
 * 		   inputs and converts them to ascii. The program displays the ascii character
 * 		   and the line and column where the character is displayed.
 */
#include <kybd_slv.h>
#include <slv.h>
#include <stdio.h>
#include "xbasic_types.h"
#include "xil_types.h"
#include "xil_io.h"
#include "xparameters.h"
#include "platform.h"
#include "xil_printf.h"
#include "xstatus.h"
#include "ctype.h"

int row = 0;
int column = 0;
char result = '-';
char SCAN_TO_CHAR(u32 scan)
{
	//char result = '-';
	//printf("scan is %d \n\r", scan);
	switch(scan)
	{

		case 28 : result = 'a';break;
		case 50 : result = 'b';break;
		case 33 : result = 'c';break;
		case 35 : result = 'd';break;
		case 36 : result = 'e';break;
		case 43 : result = 'f';break;
		case 52 : result = 'g';break;
		case 51 : result = 'h';break;
		case 67 : result = 'i';break;
		case 59 : result = 'j';break;
		case 66 : result = 'k';break;
		case 75 : result = 'l';break;
		case 58 : result = 'm';break;
		case 49 : result = 'n';break;
		case 68 : result = 'o';break;
		case 77 : result = 'p';break;
		case 21 : result = 'q';break;
		case 45 : result = 'r';break;
		case 27 : result = 's';break;
		case 44 : result = 't';break;
		case 60 : result = 'u';break;
		case 42 : result = 'v';break;
		case 29 : result = 'w';break;
		case 34 : result = 'x';break;
		case 53 : result = 'y';break;
		case 26 : result = 'z';break;

		case 22 : result = '1';break;
		case 30 : result = '2';break;
		case 38 : result = '3';break;
		case 37 : result = '4';break;
		case 46 : result = '5';break;
		case 54 : result = '6';break;
		case 61 : result = '7';break;
		case 62 : result = '8';break;
		case 70 : result = '9';break;
		case 69 : result = '0';break;

		case 8  : result = '<';break;		// backspace
		case 90 : result = '>';break;		// enter
		case 41 : result = ' '; break;		// space

		case 63 : result = '}';
		//case 211 : result = '}';
		case 191 : result = '}';
		case 211 : result = '}';
		case 136 : result = '}';
		case 126 : result = '}';
		case 240 : result = '}'; break;		// pressed key release
		//default : printf("unrecognized key %d \n\r", scan); break;
	}
	return result;
}

void test()
{
	u8 in = 37;
	char pressed;
	pressed = SCAN_TO_CHAR(in);
	printf("\n\r");
	printf("Scancode --> Ascii [char] \n\r");
	printf("   %d    -->  %d   [%c]   \n\r", in, pressed, pressed);
	printf("\n\r");
}

//delayMs function from EE260 (Embedded Systems) for timing purposes
void delayMs(int n)
{
	int i, j;
	for(i = 0 ; i < n; i++)
		for (j = 0; j < 3500; j++) {}
}
int main()
{
    init_platform();

    //static u32 zero = 0;


    printf("Begin Program\n\r");


    // screen information
    // 640 x 480 pixels total resolution
    // 8 x 12 character grid
    // meaning
    // 80 columns
    // 40 rows
    // axi interface is 32 bits long
    // 8 unused, 8 for column 8 for row, 8 for char

    u32 scancode[3] = {0, 0, 0};


    //int cycle = 0;
    //int verbose = 0;


    //mainloop here,
    while(1==1)
    {
    	u32 data_sent;
    	scancode[2] = scancode[1];
    	scancode[1] = scancode[0];

		scancode[0] = Xil_In32(XPAR_KYBD_SLV_0_S00_AXI_BASEADDR + 0);
		//printf("[%d] [%d] [%d] \n\r", SCAN_TO_CHAR(scancode[0]), SCAN_TO_CHAR(scancode[1]), SCAN_TO_CHAR(scancode[2]));
		//printf("[%d] [%d] [%d] \n\r", scancode[0], scancode[1], scancode[2]);
		if ((scancode[0] != scancode[2]) & (scancode[0] != scancode[1]) & (SCAN_TO_CHAR(scancode[1]) == '}'))
		{

			u8 key = 0;
			//u32 AXI_send = 0;

			// check for a scancode
//			printf("%x \n\r",scancode[0]);
			key = (u8)SCAN_TO_CHAR(scancode[0]);
			if ((isdigit(key)) | (isalpha(key)))
			{
				printf("%c \n\r", key);
			}
			else
			{
				printf("Special : %c \n\r", key);

			}

			if(row<=39) {
				//increment row here?
			}
			else {
				row = 0;
			}
			if(column<=79) {
				//increment column here?
			}
			else {
				row++;
				column=0;
			}

			data_sent = result | (row << 8) | (column << 16);
			//write data
			printf("Row: %d, Column: %d\n", row, column);
			printf("Result: %x\n", result);
			printf("Data: %x\n", data_sent);
			SLV_mWriteReg(XPAR_SLV_0_S00_AXI_BASEADDR, 0, data_sent);
			delayMs(500); //delay 500 ms
		}
//		if (scancode[0] == 21)
//		{
//			break;
//		}
//		if (scancode[0] == 42){
//			verbose = 1;
//		}
//		if (verbose > 0)
//		{
//			printf("[%d] [%d] [%d] \n\r", scancode[0], scancode[1], scancode[2]);
//		}

    }

    printf("End Program, exit code 0\n\r");
    cleanup_platform();
    return 0;
}
