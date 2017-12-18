#include "infrared.h"

void setup(void)
{
  pinMode(IRLEDpin, OUTPUT);
  digitalWrite(IRLEDpin, LOW);    //turn off IR LED to start
}


// Ouput the 38KHz carrier frequency for the required time in microseconds
// This is timing critial and just do-able on an Arduino using the standard I/O functions.
// If you are using interrupts, ensure they disabled for the duration.
void NECcarrier(unsigned int IRtimemicroseconds)
{
  int i;
  for(i = 0; i < (IRtimemicroseconds / 26); i++)
  {
  	digitalWrite(IRLEDpin, HIGH);   //turn on the IR LED
    delayMicroseconds(13);          //delay for 13us (9us + digitalWrite), half the carrier frequnecy
    digitalWrite(IRLEDpin, LOW);    //turn off the IR LED
    delayMicroseconds(13);          //delay for 13us (9us + digitalWrite), half the carrier frequnecy
  }
}


// Ouput the 40KHz carrier frequency for the required time in microseconds
// This is timing critial and just do-able on an Arduino using the standard I/O functions.
// If you are using interrupts, ensure they disabled for the duration.
void SIRCcarrier(unsigned int IRtimemicroseconds)
{
  int i;
  for(i = 0; i < (IRtimemicroseconds / 26); i++)
  {
  	digitalWrite(IRLEDpin, HIGH);   //turn on the IR LED
    delayMicroseconds(13);          //delay for 13us, half the carrier frequnecy
    digitalWrite(IRLEDpin, LOW);    //turn off the IR LED
    delayMicroseconds(13);          //delay for 13us, half the carrier frequnecy
  }
}


// Ouput the 36KHz carrier frequency for the required time in microseconds
// This is timing critial and just do-able on an Arduino using the standard I/O functions.
// If you are using interrupts, ensure they disabled for the duration.
void RC5carrier(unsigned int IRtimemicroseconds)
{
  int i;
  for(i = 0; i < (IRtimemicroseconds / 28); i++)
  {
  	digitalWrite(IRLEDpin, HIGH);   //turn on the IR LED
    delayMicroseconds(14);          //delay for 14us, half the carrier frequnecy
    digitalWrite(IRLEDpin, LOW);    //turn off the IR LED
    delayMicroseconds(14);          //delay for 14us, half the carrier frequnecy
  }
}


// Ouput the 36KHz carrier frequency for the required time in microseconds
// This is timing critial and just do-able on an Arduino using the standard I/O functions.
// If you are using interrupts, ensure they disabled for the duration.
void RC6carrier(unsigned int IRtimemicroseconds)
{
  int i;
  for(i = 0; i < (IRtimemicroseconds / 28); i++)
  {
  	digitalWrite(IRLEDpin, HIGH);   //turn on the IR LED
    delayMicroseconds(14);          //delay for 14us, half the carrier frequnecy
    digitalWrite(IRLEDpin, LOW);    //turn off the IR LED
    delayMicroseconds(14);          //delay for 14us, half the carrier frequnecy
  }
}


//Sends the IR code in 4 byte NEC format
void NECsendCode(unsigned long code)
{
  	//send the leading pulse
  	NECcarrier(8500);
  	delayMicroseconds(4250);

  	//send the user defined 4 byte/32bit code
  	int i;
  	for (i = 0; i < 28; i++)            //send all 4 bytes or 32 bits
  	{
    	NECcarrier(NECBITtime);               //turn on the carrier for one bit time
    	if (code & 0x08000000)            //get the current bit by masking all but the MSB
      		delayMicroseconds(3 * NECBITtime); //a HIGH is 3 bit time periods
    	else
      		delayMicroseconds(NECBITtime);     //a LOW is only 1 bit time period
      
		code<<=1;                        //shift to the next bit for this byte
  	}
  
	NECcarrier(NECBITtime);                 //send a single STOP bit.
}


//Sends the IR code in 12,15 or 20 bit SONY SIRC format
void SIRCsendCode(unsigned long code, int nBits)
{
    //Set mask according to the number of bits
	unsigned long mask;
	switch(nBits) 
	{
      case 12 :
         mask = 0x00000800;
		 break;
      case 15 :
         mask = 0x00004000;
         break;
      case 20 :
         mask = 0x00080000;
         break;
	}
	
	//send the leading pulse
  	SIRCcarrier(4*SIRCBITtime);            //2.4ms of carrier
  	delayMicroseconds(SIRCBITtime);    //0.6ms of silence
  
  	//send the user defined 12/15/20bit code
  	int i;
  	for (i = 0; i < nBits; i++)            //send all 12/15/20bits
  	{
  		if (code & mask) //get the current bit by masking all but the MSB
			SIRCcarrier(2*SIRCBITtime); //turn on the carrier for two bit time
		else
			SIRCcarrier(SIRCBITtime); //turn on the carrier for one bit time
	  
		delayMicroseconds(SIRCBITtime); //a LOW is only 1 bit time period
	  
		code <<= 1;  //shift to the next bit for this byte
  	}
}


void RC5bitLow(unsigned int IRTimemicroseconds)
{
	RC5carrier(IRTimemicroseconds);
	delayMicroseconds(IRTimemicroseconds);
}


void RC5bitHigh(unsigned int IRTimemicroseconds)
{
	delayMicroseconds(IRTimemicroseconds);
	RC5carrier(IRTimemicroseconds);
}


//Sends the IR code in 11 bits PHILIPS RC5 format
void RC5sendCode(unsigned long code)
{
  //send 2 start pulses + toggle
  RC5bitHigh(RC5BITtime);
  RC5bitHigh(RC5BITtime);
  RC5bitLow(RC5BITtime);	
	
  //send the user defined 11bit code
  int i;
  for (i = 0; i < 11; i++)            //send all 11 bits
  {
  	if (code & 0x00000400) //get the current bit by masking all but the MSB
		RC5bitHigh(RC5BITtime);
	else
		RC5bitLow(RC5BITtime);
		
	code <<= 1;  //shift to the next bit for this byte
  }
}


void RC6bitLow(unsigned int IRTimemicroseconds)
{
	delayMicroseconds(IRTimemicroseconds);
	RC6carrier(IRTimemicroseconds);
	
}


void RC6bitHigh(unsigned int IRTimemicroseconds)
{
	RC6carrier(IRTimemicroseconds);
	delayMicroseconds(IRTimemicroseconds);
}


//Sends the IR code in 2 byte PHILIPS RC6 format
void RC6sendCode(unsigned long code)
{
  //send 2 start pulses (first one is longer)
  RC6carrier(6*RC6BITtime); //2.666ms of carrier
  delayMicroseconds(2*RC6BITtime); //0.889ms of silence	
  RC6bitHigh(RC6BITtime);
	
  //send 3 field pulses
  int i;
  for (i = 0; i < 3; ++i)
	  RC6bitLow(RC6BITtime);
	
  //send toggle pulse
  RC6bitHigh(2*RC6BITtime);	
	
  //send the user defined 2 byte/16bit code
  for (i = 0; i < 16; ++i)            //send all 16 bits
  {
  	if (code & 0x00008000) //get the current bit by masking all but the MSB
		RC6bitHigh(RC6BITtime);
	else
		RC6bitLow(RC6BITtime);
		
	code <<= 1;  //shift to the next bit for this byte
  }
}
