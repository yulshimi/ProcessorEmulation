#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "emulator.h"


#define ARG_ERROR "[ERROR] Usage: <program> <imem file> <dmem file>"
#define IMEM_LOAD_ERROR "[ERROR] Unable to load imem from input file"
#define DMEM_LOAD_ERROR "[ERROR] Unable to load dmem from input file"

int main(int argc, char *argv[]) {

   if(argc < 3) {
      fprintf(stderr, "%s\n", ARG_ERROR);
      return 1;
   }

   Emulator emulator;
   emulator.reset();

   if(!emulator.loadInstructions(argv[1])) {
      fprintf(stderr, "%s\n", IMEM_LOAD_ERROR);  
      return 2;
   }

   if(!emulator.loadDataMemory(argv[2])) {
      fprintf(stderr, "%s\n", DMEM_LOAD_ERROR);
      return 3;
   }



   uint8_t exitCode = emulator.run();

   if(exitCode == 0) {

      /*
       * YOUR TEST CASES HERE
       *
       * This test harness will be similar to how our grading script will
       * run your program.
       *
       */

      fprintf(stdout, "[SIMULATION] PASSED\n");
   }
   else {

      fprintf(stdout, "[SIMULATION] FAILED\n");
   }

   return 0;
}
