/*
 *
 * University of California, San Diego
 *    
 *    CSE141L Summer 2017
 *
 */


#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <stdint.h>
#include <stdlib.h>

#define RF_DEPTH    16  
#define STACK_DEPTH 16

#define INSTR_WIDTH 9

#define DATA_WIDTH  8
#define DMEM_DEPTH  (1 << DATA_WIDTH)
#define IMEM_DEPTH  (1 << DATA_WIDTH)


/*
 * Instruction Memory; 9-bit width
 *
 * DO NOT MODIFY
 *
 */
class InstructionMemory 
{

   private:
      uint8_t memory[IMEM_DEPTH][INSTR_WIDTH];

      bool assertIndex(uint16_t index) 
      {
         return (index >= 0 && index < IMEM_DEPTH) ? true : false;
      }

      bool assertData(uint8_t *data) 
      {
         for(int i = 0; i < INSTR_WIDTH; i++) 
         {
            if(data[i] != 0 && data[i] != 1) 
            {
               return false;
            }
         }
         return true;
      }

   public:
      InstructionMemory()  {};
      ~InstructionMemory() {};

      void clear() 
      {
         for(int i = 0; i < IMEM_DEPTH; i++) 
         {
            for(int j = 0; j < INSTR_WIDTH; j++) 
            {
               memory[i][j] = 0;
            }
         }
      }

      bool set(uint16_t index, uint8_t *data) 
      {
         //printf("in set :%s \n", data);
         if(!this->assertIndex(index) || !this->assertData(data)) 
         {
            return false;
         }

         for(int i = 0; i < INSTR_WIDTH; i++) 
         {
            this->memory[index][i] = data[i];
         }
         
         return true;
      }

      uint8_t* get(uint16_t index) 
      {

         if(!this->assertIndex(index)) 
         {
            return NULL;
         }

         uint8_t *data = (uint8_t*) malloc(sizeof(uint8_t) * INSTR_WIDTH);

         for(int i = 0; i < INSTR_WIDTH; i++) 
         {  
            data[i] = this->memory[index][i];
         }

         return data;
      }

};


/*
 * Register File Memory; 8-bit width
 *
 * DO NOT MODIFY
 *
 */
class RegisterFileMemory {

   private:
      uint8_t memory[RF_DEPTH][DATA_WIDTH];

      bool assertIndex(uint16_t index) {
         return (index >= 0 && index < RF_DEPTH) ? true : false;
      }

      bool assertData(uint8_t *data) {
         for(int i = 0; i < DATA_WIDTH; i++) {
            if(data[i] != 0 && data[i] != 1) {
               return false;
            }
         }

         return true;
      }

   public:
      RegisterFileMemory()  {};
      ~RegisterFileMemory() {};
      
      void clear() {
         for(int i = 0; i < RF_DEPTH; i++) {
            for(int j = 0; j < DATA_WIDTH; j++) {
               memory[i][j] = 0;
            }
         }
      }

      bool set(uint16_t index, uint8_t *data) {
         
         if(!this->assertIndex(index) || !this->assertData(data)) {
            return false;
         }

         for(int i = 0; i < DATA_WIDTH; i++) {
            this->memory[index][i] = data[i];
         }

         return true;
      }

      uint8_t* get(uint16_t index) {

         if(!this->assertIndex(index)) {
            return NULL;
         }

         uint8_t *data = (uint8_t*) malloc(sizeof(uint8_t) * DATA_WIDTH);

         for(int i = 0; i < DATA_WIDTH; i++) {
            data[i] = this->memory[index][i];
         }

         return data;
      }

};

/*
 * Data Memory; 8-bit width
 *
 * DO NOT MODIFY
 *
 */
class DataMemory {

   private:
      uint8_t memory[DMEM_DEPTH][DATA_WIDTH];

      bool assertIndex(uint16_t index) {
         return (index >= 0 && index < DMEM_DEPTH) ? true : false;
      }

      bool assertData(uint8_t *data) {
         for(int i = 0; i < DATA_WIDTH; i++) {
            if(data[i] != 0 && data[i] != 1) {
               return false;
            }
         }
         return true;
      }

   public:
      DataMemory()  {};
      ~DataMemory() {};
      
      void clear() {
         for(int i = 0; i < DMEM_DEPTH; i++) {
            for(int j = 0; j < DATA_WIDTH; j++) {
               memory[i][j] = 0;
            }
         }
      }

      bool set(uint16_t index, uint8_t *data) {
         
         if(!this->assertIndex(index) || !this->assertData(data)) {
            return false;
         }

         for(int i = 0; i < DATA_WIDTH; i++) {
            this->memory[index][i] = data[i];
         }

         return true;
      }

      uint8_t* get(uint16_t index) {

         if(!this->assertIndex(index)) {
            return NULL;
         }

         uint8_t *data = (uint8_t*) malloc(sizeof(uint8_t) * DATA_WIDTH);

         for(int i = 0; i < DATA_WIDTH; i++) {
            data[i] = this->memory[index][i];
         }

         return data;
      }

};

/*
 * Stack Memory; 8-bit width
 *
 * DO NOT MODIFY
 *
 */
class StackMemory {

   private:
      uint8_t memory[STACK_DEPTH][DATA_WIDTH];
      int8_t top;

      bool assertData(uint8_t *data) {
         for(int i = 0; i < DATA_WIDTH; i++) {
            if(data[i] != 0 && data[i] != 1) {
               return false;
            }
         }
         return true;
      }

   public:
      StackMemory() {
         top = -1;
      };

      ~StackMemory() {};
      
      void clear() {
         for(int i = 0; i < STACK_DEPTH; i++) {
            for(int j = 0; j < DATA_WIDTH; j++) {
               memory[i][j] = 0;
            }
         }
         top = -1;
      }

      bool push(uint8_t *data) {

         if(!this->assertData(data) && this->top < STACK_DEPTH-1) {
            return false;
         }

         this->top++;
         for(int i = 0; i < DATA_WIDTH; i++) {
            this->memory[top][i] = data[i];
         }
         
         return true;
      }


      uint8_t* pop() {

         if(top < 0) {
            return NULL;
         }

         uint8_t *data = (uint8_t*) malloc(sizeof(uint8_t) * DATA_WIDTH);
         for(int i = 0; i < DATA_WIDTH; i++) {
            data[i] = this->memory[top][i];
         }

         this->top--;
         return data;
      }

};

/*
 * Return 2's compliment of binary value (8-bit width)
 *
 * Assumptions:
 *    LSB is in index = DATA_WIDTH-1
 *    MSB is in index = 0
 * 
 * DO NOT MODIFY
 *
 */
inline void twosComplement(int data_i[DATA_WIDTH], int data_o[DATA_WIDTH]) {

    for(int i = 0; i < DATA_WIDTH; i++) {
        data_o[i] = data_i[i] == 1 ? 0 : 1;
    }

    int carry = 1;
    for(int i = DATA_WIDTH-1; i >= 0; i--) {
        if(carry == 0) {
            break;
        }

        if(data_o[i] == 1 && carry == 1) {
            data_o[i] = 0;
        }
        else {
            data_o[i] = 1;
            carry = 0;
        }
    }
}


#endif
