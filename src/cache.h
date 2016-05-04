/////////////////////////////////////////////////////
// CSC 252 Project 4
// Cache Simulator
// Wilfred Wallis and Chris Dalke
/////////////////////////////////////////////////////
// File: cache.h
/////////////////////////////////////////////////////

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

extern int write_xactions;
extern int read_xactions;

void printHelp(const char * prog);
uint32_t logBaseTwo(uint32_t num);
uint32_t extractBitSequence(uint32_t in, int start, int offset);
void setBit(uint32_t A[],  uint32_t index);
void resetBit(uint32_t A[],  uint32_t index);
uint32_t getBit(uint32_t A[],  uint32_t index);

/////////////////////////////////////////////////////
// End of file
/////////////////////////////////////////////////////