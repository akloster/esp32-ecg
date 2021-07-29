#ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>
#include "consts.h"
extern int16_t sampleBuffer[2][BUFFER_LENGTH];
extern uint16_t  timingBuffer[2][BUFFER_LENGTH];

extern volatile boolean bufferFlag;         // indicates a buffer is ready
extern volatile unsigned int currentBuffer; // which buffer is currently being written to, 0 or 1
extern boolean running;                     // does what it's name implies
 
#endif