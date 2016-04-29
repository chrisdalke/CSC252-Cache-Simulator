/////////////////////////////////////////////////////
// CSC 252 Project 4
// Cache Simulator
// Wilfred Wallis and Chris Dalke
/////////////////////////////////////////////////////
// File: trace.c
/////////////////////////////////////////////////////

#include "trace.h"

//Variable declarations
FILE *traceFile;
FILE *outputFile;
char *inputLine;
const size_t MAX_LINE_SIZE = 300;

/////////////////////////////////////////////////////
// Trace functions
// traceOpen: opens a new trace file
// traceReadLine: reads the next line. Returns null if done.
// traceClose: closes the trace file
/////////////////////////////////////////////////////

void traceOpen(char *filename){
	traceFile = fopen(filename, "r");
	inputLine = malloc(MAX_LINE_SIZE);
}

char * traceReadLine(){
	return fgets(inputLine, MAX_LINE_SIZE, traceFile);
}

void traceClose(){
	fclose(traceFile);
	free(inputLine);
}

/////////////////////////////////////////////////////
// Output functions
// outputOpen: opens a new output file
// outputWrite: writes a line to the output file
// outputClose: closes the output file
/////////////////////////////////////////////////////

void outputOpen(char *filename){
	outputFile = fopen(filename, "w");
}

void outputWrite(char *text){
	fputs(text,outputFile);
}

void outputClose(){
	fclose(outputFile);
}

/////////////////////////////////////////////////////
// End of file
/////////////////////////////////////////////////////