#ifndef INFRARED_H
#define INFRARED_H

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define IRLEDpin  0              //the arduino pin connected to IR LED to ground. HIGH=LED ON
#define NECBITtime   562            //length of the carrier bit in microseconds
#define SIRCBITtime   600            //length of the carrier bit in microseconds
#define RC5BITtime   889            //length of the carrier bit in microseconds
#define RC6BITtime   444            //length of the carrier bit in microseconds


//put your own code here - 4 bytes (ADDR1 | ADDR2 | COMMAND1 | COMMAND2)

/* Modo de refrigeracao ventilacao automatica */
//liga:
#define IRcode_auto_on 0x08800358
//desliga:
#define IRcode_off 0x088C0051


void setup(void);
void NECsendCode(unsigned long code);
void SIRCsendCode(unsigned long code, int nBits);
void RC5sendCode(unsigned long code);
void RC6sendCode(unsigned long code);


#endif /* INFRARED.H */
