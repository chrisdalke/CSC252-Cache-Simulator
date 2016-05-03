/////////////////////////////////////////////////////
// CSC 252 Project 4
// Cache Simulator
// Wilfred Wallis and Chris Dalke
/////////////////////////////////////////////////////
// File: cache.c
/////////////////////////////////////////////////////

#include "cache.h"
#include "trace.h"

//Define constants
//Hit/miss constants
#define HIT_SUCCESS 0
#define CONFLICT_MISS 1
#define COMPULSORY_MISS 2
#define CAPACITY_MISS 3
//Replacement policies
#define FIFO 0
#define LRU 1

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
    /////////////////////////////////////////////////////
    // Command line argument parsing
    /////////////////////////////////////////////////////

    //Cache specification variables (defaults)
    //These can be overwritten in the command line.
    uint32_t size = 32; //total size of L1$ (KB)
    uint32_t ways = 1; //# of ways in L1. Default to direct-mapped
    uint32_t line = 32; //line size (B)
    uint32_t replacementPolicy = FIFO; //replacement policy
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
            //Use LRU replacement policy
            replacementPolicy = LRU;
        }

        //unrecognized input
        else {
            printf("Unrecognized argument. Exiting.\n");
            return -1;
        }
    }

    /////////////////////////////////////////////////////
    // Cache Initialization
    /////////////////////////////////////////////////////

    //Calculate the cache attributes
    uint32_t numSets = (size * 1024) / (line * ways);
    uint32_t numIndexBits = logBaseTwo(numSets);
    uint32_t numOffsetBits = logBaseTwo(line);
    uint32_t numTagBits = (32) - (numIndexBits + numOffsetBits);

    //Cache will be stored as arrays
    //Size is based on the total size, ways, etc...
    //Initialize the Cache

    //Possibly store the cache data in a 2d array of bytes? or do we even need to store the actual cache since its a simulation?
    //Maybe we just store the index/tag arrays

    //"SETS" == "SLOTS"
    //A full cache address is made up of (Tag Bits & Offset Bits)

    //Index stores a fast way to connect with the slot in the cache
    //aka an index to the slot line

    //Setup tag array
    //Setup valid bit array
    //Setup dirty bit array
    uint32_t tagArray[numSets][ways];
    uint32_t validBit[numSets][ways];
    uint32_t dirtyBit[numSets][ways];
    uint32_t evictionTable[numSets][ways];

    //initialize the arrays to default values
    for (int currentSet = 0; currentSet < numSets; currentSet++){
        for (int currentWay = 0; currentWay < ways; currentWay++){
            tagArray[currentSet][currentWay] = 0;
            validBit[currentSet][currentWay] = 0;
            dirtyBit[currentSet][currentWay] = 0;
            evictionTable[currentSet][currentWay] = 0;
        }
    }

    //Print out the parameters we grabbed for the cache
    printf("Ways: %u; Sets: %u; Line Size: %uB\n", ways,numSets,line);
    printf("Tag: %d bits; Index: %d bits; Offset: %d bits\n", numTagBits, numIndexBits, numOffsetBits);

    /////////////////////////////////////////////////////
    // Output File Initialization
    // Result files will be stored in the trace folder
    /////////////////////////////////////////////////////

    //Name the filename based on the input file
    //Allocate a string big enough for the appended filename and add extension
    char * outputFilename = malloc( sizeof(char) * ( strlen(filename) + 16 ) );
    strcpy(outputFilename,filename);
    strcat(outputFilename,".simulated");

    //Open the output file stream
    //We will write to this file later
    outputOpen(outputFilename);

    //Set up output labels
    char * cacheHitTag = " hit\n";
    char * compulsoryMissTag = " compulsory\n";
    char * capacityMissTag = " capacity\n";
    char * conflictMissTag = " conflict\n";
    char * defaultMissTag = " miss\n";

    /////////////////////////////////////////////////////
    // Input File Initialization
    /////////////////////////////////////////////////////

    //Load the input file
    traceOpen(filename);

    /////////////////////////////////////////////////////
    // Cache Simulation Loop
    /////////////////////////////////////////////////////

    //Temporary utility variables
    int numReadLines = 0;
    char * currentOutput;
    char tempAccessType;
    char * tempAddress = malloc( sizeof(char) * (12));
    uint32_t currentAddress;
    char * statusTag;

    //Read in the file, line by line
    char * nextLine = traceReadLine();
    while (nextLine != NULL){
        numReadLines++;

        /////////////////////////////////////////////////////
        // Instruction Data Parsing / Setup
        /////////////////////////////////////////////////////

        //Get the access type
        tempAccessType = nextLine[0];

        //Get the address of the access by getting character 2 to 10 in the input line
        strncpy(tempAddress,nextLine + 2,10);
        //manually add null terminator to end the string
        tempAddress[10] = '\0'; 
        //Convert hex address to integer address
        currentAddress = strtol(tempAddress, NULL, 0);

        //Get the index, tag, and offset bits
        uint32_t tagBits = extractBitSequence(currentAddress,32-numTagBits,numTagBits);
        uint32_t indexBits = extractBitSequence(currentAddress,numOffsetBits,numIndexBits);
        uint32_t offsetBits = extractBitSequence(currentAddress,0,numOffsetBits);

        //Convert this to the set number / slot number
        uint32_t slotId = indexBits;

        //Store result cache location after we find the slot we would like to use
        uint32_t selectedWay;

        //Based on the index bits, get the row of the cache that we are looking at
        //loop through all of the sets in this index
        uint32_t hitStatus = COMPULSORY_MISS;

        /////////////////////////////////////////////////////
        // Cache Search
        /////////////////////////////////////////////////////

        for (int currentWay = 0; currentWay < ways; currentWay++){
            if (validBit[indexBits][currentWay] == 1){
                if (tagArray[indexBits][currentWay] == tagBits){
                    //Mark this as a successful hit
                    hitStatus = HIT_SUCCESS;
                    selectedWay = currentWay;
                    break;
                } else {
                    //Tags don't match, must be a conflict miss
                    hitStatus = CONFLICT_MISS;
                }
            }
        }

        /////////////////////////////////////////////////////
        // Hit Handling
        /////////////////////////////////////////////////////

        //Based on the hit/miss status, perform whatever action we need to do
        if (hitStatus == HIT_SUCCESS){
            //Record the hit
            totalHits++;
            //Display the hit
            statusTag = cacheHitTag;

            //If our replacement policy is LRU
            //We need to record that we have accessed this cache
            if (replacementPolicy == LRU){
                evictionTable[indexBits][selectedWay] = numReadLines; //Use line number as age
            }
        } 

        /////////////////////////////////////////////////////
        // Miss Handling
        /////////////////////////////////////////////////////

        else if (hitStatus == COMPULSORY_MISS | hitStatus == CONFLICT_MISS | hitStatus == CAPACITY_MISS){
            //Record the miss
            totalMisses++;

            //Display the miss status
            switch (hitStatus){
                case COMPULSORY_MISS: {
                    statusTag = compulsoryMissTag;
                    break;
                }
                case CONFLICT_MISS: {
                    statusTag = conflictMissTag;
                    break;
                }
                case CAPACITY_MISS: {
                    statusTag = capacityMissTag;
                    break;
                }
            }

            /////////////////////////////////////////////////////
            // Replacement Policy Way Selection
            /////////////////////////////////////////////////////
            //Choose which item to evict from the current index
            //We will choose this based on our replacement policy
            // -FIFO (First In First Out), choose item with lowest age
            // -LRU (Least Recently Used), choose item with lowest age, update age based on usage

            uint32_t lowestAge = numReadLines + 1; //start with current age+1 as lowest age
            //Find the lowest age in the table at the current index
            for (int currentWay = 0; currentWay < ways; currentWay++){
                if (evictionTable[indexBits][currentWay] < lowestAge){
                    lowestAge = evictionTable[indexBits][currentWay];
                    selectedWay = currentWay;
                }
            }

            /////////////////////////////////////////////////////
            // Writeback
            /////////////////////////////////////////////////////

            if (dirtyBit[indexBits][selectedWay] == 1){
                //When we evict an old item, if the dirty bit has been set, write it to memory
                //increment write_xactions to represent this
                write_xactions++;
            }

            /////////////////////////////////////////////////////
            // Cache Item Update
            /////////////////////////////////////////////////////

            //update new cache items
            tagArray[indexBits][selectedWay] = tagBits;
            validBit[indexBits][selectedWay] = 1;
            dirtyBit[indexBits][selectedWay] = 0;

            //update eviction table data for our replacement policy
            evictionTable[indexBits][selectedWay] = numReadLines; //use the line number as an age

            //Increment 'read from memory' counter
            read_xactions++;
        }

        /////////////////////////////////////////////////////
        // Store Instruction Dirty Bit Handling
        /////////////////////////////////////////////////////

        //Handle store instruction
        if (tempAccessType == 's'){
            //Set the dirty bit to true for the data we have written to
            dirtyBit[indexBits][selectedWay] = 1;
        }

        /////////////////////////////////////////////////////
        // Output
        /////////////////////////////////////////////////////

        //Initialize output string
        currentOutput = malloc( sizeof(char) * ( strlen(nextLine) + 16 ) );
        //Copy original string into output
        strcpy(currentOutput,nextLine); 
        //Remove newline from original input string
        currentOutput[strlen(currentOutput) - 1] = '\0'; 
        //Add tag to end of output based on what happened this step
        strcat(currentOutput,statusTag); 
        //Write output to file
        outputWrite(currentOutput);

        //Load the next line, if one exists
        nextLine = traceReadLine();
    }

    /////////////////////////////////////////////////////
    // End of Cache Simulation Loop
    /////////////////////////////////////////////////////

    /* Print results */
    printf("Miss Rate: %8lf%%\n", ((double) totalMisses) / ((double) totalMisses + (double) totalHits) * 100.0);
    printf("Read Transactions: %d\n", read_xactions);
    printf("Write Transactions: %d\n", write_xactions);

    /////////////////////////////////////////////////////
    // Cleanup
    /////////////////////////////////////////////////////

    //Close the trace
    traceClose();

    //Close the output file
    outputClose();

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

//Extracts a sequence of bits from a number
uint32_t extractBitSequence(uint32_t in, int start, int offset) {
   return (in >> start) & ((1 << offset)-1);
}

/////////////////////////////////////////////////////
// End of file
/////////////////////////////////////////////////////