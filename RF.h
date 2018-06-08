#ifndef _RF_h
#define _RF_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

// receiver states
#define STATE_IDLE     2
#define STATE_MARK     3
#define STATE_SPACE    4
#define STATE_STOP     5

#define ERR 0
#define DECODED 1

// Some useful constants
#define USECPERTICK 50  // microseconds per clock interrupt tick
#define RAWBUF 500 // Length of raw duration buffer

#define RF_TIMER_OFF   15 // ms timer
#define MIN_PULSE_LENGTH  25  // min pulse in micros

// information for the interrupt handler
typedef struct {
  uint8_t recvpin;           // pin for IR data from detector
  uint8_t rcvstate;          // state machine
  unsigned int timer;     // state timer, counts 50uS ticks.
  unsigned int rawbuf[RAWBUF]; // raw data
  uint8_t rawlen;         // counter of entries in rawbuf
}
rfparams_t;

// Defined in IRremote.cpp
extern volatile rfparams_t rfparams;

// Results returned from the decoder
class rfdecode_results {
  public:
    int decode_type;
    unsigned long value; // Decoded value
    int bits; // Number of bits in decoded value
    volatile unsigned int *rawbuf; // Raw intervals in .5 us ticks
    int rawlen; // Number of records in rawbuf.
};

// main class for receiving IR
class RFrecv
{
  public:
    RFrecv(int recvpin);
    void enableRX();
    void disableRX();
    int decode(rfdecode_results *results);
    void resume();
};

#endif
