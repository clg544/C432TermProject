/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <uart.h>
#include <debug.h>

#define ENDIANNESS_UNCHECKED 0
#define LITTLE_ENDIAN 1
#define BIG_ENDIAN 2

int endianness = ENDIANNESS_UNCHECKED; 

int checkEndianness(){
    int i = 1;
    char c = i;
    if (c){
	return LITTLE_ENDIAN;
    } else {
	return BIG_ENDIAN;
    }
}



void printUIntHex(unsigned int character){
    switch(endianness){
	case ENDIANNESS_UNCHECKED:
	    endianness = checkEndianness();
	    printUIntHex(character);
	    break;
	case LITTLE_ENDIAN:
	    printUIntHexLittleEndian(character);
	    break;
	case BIG_ENDIAN:
	    printUIntHexBigEndian(character);
	    break;
	default:
	    endianness = checkEndianness();
	    printUIntHexLittleEndian(character);
	    break;
    }
}

void terminateLine(){
    UARTCharPut(UART_PORT0, '\n');
    UARTCharPut(UART_PORT0, '\r');
}


void printUIntHexBigEndian(unsigned int character){
    char hexrep[10] = "0x";
    char* string = &(hexrep[2]);
    for (int i = 0; i < 4; i++){
	char c = (character >> (i * 8));
	char upper = (c >> 4);
	char lower = (c & 0x0f);
	string[i * 2]= charNum2ascii(upper); 
	string[(i * 2) + 1]= charNum2ascii(lower);
    }
    UART_puts(UART_PORT0,hexrep, 10);
}

void printUIntHexLittleEndian(unsigned int character){
    char hexrep[10] = "0x";
    char* string = &(hexrep[2]);
    for (int i = 3,j = 0; i >= 0; i--, j++){
	char c = (character >> (i * 8));
	char upper = (c >> 4);
	char lower = (c & 0x0f);
	string[j * 2]= charNum2ascii(upper); 
	string[(j * 2) + 1]= charNum2ascii(lower);
    }
    UART_puts(UART_PORT0,hexrep, 10);
}

void printUIntBinaryLittleEndian(unsigned int character){
    char binaryRep[42] = "0b";
    char* string = &(binaryRep[2]);
    for (int i = 7,j = 0; i >= 0; i--, j++){
	for(int k = 3; k > 0; k--){
	    int mask = (1 << (k + (i * 4)));
	    if (mask & character){
		string[(j * 4) + (3 - k) + j]= 1;
	    } else {
		string[(j * 4) + (3 - k) + j]= 0;
	    }
	}
	string[(j + 1)* 4]= ' ';
    }
    UART_puts(UART_PORT0,binaryRep, 42);
}

void printUIntBinaryBigEndian(unsigned int character){
    char hexrep[38] = "0b";
    char* string = &(hexrep[2]);
    for (int i = 0; i < 4; i++){
	
	char c = (character >> (i * 8));
	char upper = (c >> 4);
	char lower = (c & 0x0f);
	string[i * 2]= charNum2ascii(upper); 
	string[(i * 2) + 1]= charNum2ascii(lower);
    }
    UART_puts(UART_PORT0,hexrep, 10);
}

char charNum2ascii(char num){
    switch (num){
	case 0 ... 9:
	    return 48 + num;
	    break;
	case 10 ... 15:
	    return 97 + (num - 10);
	    break;
	default:
	    return 0;
	    break;
    }
}