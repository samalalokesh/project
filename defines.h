// defines.h

#ifndef _DEFINES_H_
#define _DEFINES_H_

/*-------------------------------------------------------
  Write 8-bit data into any position of a register

  WORD      : Target register
  STARTBIT  : Starting bit position
  BYTE      : 8-bit data
-------------------------------------------------------*/
#define WRITEBYTE(WORD,STARTBIT,BYTE) \
        WORD=((WORD & ~(0xFF << STARTBIT)) | \
        (BYTE << STARTBIT))

/*-------------------------------------------------------
  Read a single bit from a register
-------------------------------------------------------*/
#define READBIT(WORD,BIT) ((WORD >> BIT) & 1)

/*-------------------------------------------------------
  Set a particular bit
-------------------------------------------------------*/
#define SETBIT(WORD,BIT) WORD |= (1 << BIT)

/*-------------------------------------------------------
  Write 4-bit nibble into a register
-------------------------------------------------------*/
#define WRITENIBBLE(WORD,BITPOS,BYTE) \
        WORD=((WORD & ~(15 << BITPOS)) | \
        ((BYTE & 15) << BITPOS))

/*-------------------------------------------------------
  Read 4-bit nibble from a register
-------------------------------------------------------*/
#define READNIBBLE(WORD,BITPOS) ((WORD >> BITPOS) & 15)

/*-------------------------------------------------------
  Hardware Connections (LPC2148)
-------------------------------------------------------*/

// Buzzer connected to P0.5
#define BUZZER  5

// LED1 connected to P0.6
#define LED1    6

// LED2 connected to P0.7
#define LED2    7

// TSOP1738 IR Receiver connected to P0.20
#define TSOP_PIN 20

#endif
