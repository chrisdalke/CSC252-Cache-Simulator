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

extern int write_xactions;
extern int read_xactions;

void printHelp(const char * prog);
uint32_t logBaseTwo(uint32_t num);
uint32_t extractBitSequence(uint32_t in, int start, int offset);

/////////////////////////////////////////////////////
// End of file
/////////////////////////////////////////////////////