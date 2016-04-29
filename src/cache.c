/////////////////////////////////////////////////////
// CSC 252 Project 4
// Cache Simulator
// Wilfred Wallis and Chris Dalke
/////////////////////////////////////////////////////
// File: cache.c
/////////////////////////////////////////////////////

#include "cache.h"
#include "trace.h"

int write_xactions = 0;
int read_xactions = 0;

/////////////////////////////////////////////////////
// printHelp function: Prints help message to user
/////////////////////////////////////////////////////

void printHelp(const char * prog) {
    printf("%s [-h] | [-s <size>] [-w <ways>] [-l <line>] [-t <trace>]\n", prog);
    printf("options:\n");
    printf("-h: print out help text\n");
    printf("-s <cache size>: set the total size of the cache in KB\n");
    printf("-w <ways>: set the number of ways in each set\n");
    printf("-l <line size>: set the size of each cache line in bytes\n");
    printf("-t <trace>: use <trace> as the input file for memory traces\n");
    printf("-lru: use LRU replacement policy instead of FIFO\n"); //Extra credit parameter
}

/////////////////////////////////////////////////////
// Main function. Feed to options to set the cache
//	
//	Options:
//	-h : print out help message
//	-s : set L1 cache Size (B)
//	-w : set L1 cache ways
//	-l : set L1 cache line size
/////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    //Cache specification variables (defaults)
    //These can be overwritten in the command line.
    uint32_t size = 32; //total size of L1$ (KB)
    uint32_t ways = 1; //# of ways in L1. Default to direct-mapped
    uint32_t line = 32; //line size (B)
    int i;

    // hit and miss counts
    int totalHits = 0;
    int totalMisses = 0;

    //Filename for the cache simulation we want to load
    char * filename;

    //Command line arguments to compare to
    const char helpString[] = "-h";
    const char sizeString[] = "-s";
    const char waysString[] = "-w";
    const char lineString[] = "-l";
    const char traceString[] = "-t";
    const char lruString[] = "-lru";

    if (argc == 1) {
    // No arguments passed, show help
        printHelp(argv[0]);
        return 1;
    }

    //parse command line
    for(i = 1; i < argc; i++){

        //check for help
        if(!strcmp(helpString, argv[i])){
            //print out help text and terminate
            printHelp(argv[0]);
            return 1; //return 1 for help termination
        }

        //check for size
        else if(!strcmp(sizeString, argv[i])){
            //take next string and convert to int
            i++; //increment i so that it skips data string in the next loop iteration
            //check next string's first char. If not digit, fail
            if(isdigit(argv[i][0])){
                size = atoi(argv[i]);
            } else {
                printf("Incorrect formatting of size value\n");
                return -1; //input failure
            }
        }

        //check for ways
        else if(!strcmp(waysString, argv[i])){
            //take next string and convert to int
            i++; //increment i so that it skips data string in the next loop iteration
            //check next string's first char. If not digit, fail
            if(isdigit(argv[i][0])){
                ways = atoi(argv[i]);
            } else {
                printf("Incorrect formatting of ways value\n");
                return -1; //input failure
            }
        }

        //check for line size
        else if(!strcmp(lineString, argv[i])){
            //take next string and convert to int
            i++; //increment i so that it skips data string in the next loop iteration
            //check next string's first char. If not digit, fail
            if(isdigit(argv[i][0])){
                line = atoi(argv[i]);
            } else {
                printf("Incorrect formatting of line size value\n");
                return -1; //input failure
            }
        }

        else if (!strcmp(traceString, argv[i])){
            filename = argv[++i];
        }

        else if (!strcmp(lruString, argv[i])){
            // Extra Credit: Implement Me!
            printf("Unrecognized argument. Exiting.\n");
            return -1;
        }

        //unrecognized input
        else {
            printf("Unrecognized argument. Exiting.\n");
            return -1;
        }
    }


    /////////////////////////////////////////////////////
    // Cache Simulation
    /////////////////////////////////////////////////////

    /* TODO: Probably should intitalize the cache */

    //Calculate the cache attributes
    uint32_t numSets = (size * 1024) / (line * ways);
    uint32_t numIndexBits = logBaseTwo(numSets);
    uint32_t numOffsetBits = logBaseTwo(line);
    uint32_t numTagBits = (32) - (numIndexBits + numOffsetBits);

    //Cache will be stored as arrays
    //Size is based on the total size, ways, etc...



    //Print out the parameters we grabbed for the cache
    printf("Ways: %u; Sets: %u; Line Size: %uB\n", ways,numSets,line);
    printf("Tag: %d bits; Index: %d bits; Offset: %d bits\n", numTagBits, numIndexBits, numOffsetBits);


    /////////////////////////////////////////////////////
    // Read file input
    /////////////////////////////////////////////////////

    /* TODO: Now we read the trace file line by line */


    /////////////////////////////////////////////////////
    // Cache Simulation
    /////////////////////////////////////////////////////

    /* TODO: Now we simulate the cache */  

    /* Print results */
    printf("Miss Rate: %8lf%%\n", ((double) totalMisses) / ((double) totalMisses + (double) totalHits) * 100.0);
    printf("Read Transactions: %d\n", read_xactions);
    printf("Write Transactions: %d\n", write_xactions);

    /////////////////////////////////////////////////////
    // Output to result file
    /////////////////////////////////////////////////////

    /* TODO: Now we output the file */

    /////////////////////////////////////////////////////
    // Cleanup
    /////////////////////////////////////////////////////

    /* TODO: Cleanup */

}

/////////////////////////////////////////////////////
// Utility Functions
/////////////////////////////////////////////////////

//Finds log base 2 of a number
uint32_t logBaseTwo(uint32_t num){
    uint32_t r = 0;
    while (num >>= 1){
        r++;
    }
    return r;
}

/////////////////////////////////////////////////////
// End of file
/////////////////////////////////////////////////////