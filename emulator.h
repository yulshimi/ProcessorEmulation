//Name: Phillip Jo
/*
 * University of California, San Diego
 *    
 *    CSE141L Summer 2017
 *
 */

#ifndef _EMULATOR_H_
#define _EMULATOR_H_
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "utility.h"

//==============================================================================
class InstructionMemoryModule 
{
private:
  InstructionMemory imem;

public:
  InstructionMemoryModule();

      // Clears instruction memory
  void clearModule();         

      // Initialize instruction memory with machine code file    
  bool setInstructionMemoryWith(char *file);

      /*
       * Interface for the Emulator, this is used for testing the processor. To
       * load to and fetch from instruction memory module, use the fetch() and
       * setInstructionMemoryWith() method.
       *
       */
  uint8_t* getInstructionMemoryAt(uint8_t index);
      
      // You may modify fetch's function signature
  void fetch(uint8_t pc_i, uint8_t*& instruction_o);
  void print_out()
  {
    for(int i=0; i < 100; ++i)
    {
      uint8_t* myPtr = NULL;
      myPtr = getInstructionMemoryAt(i);
      printf("%s \n", myPtr);
    }
  }
};

//==============================================================================
class RegisterModule 
{
private:
  RegisterFileMemory rf;
      //StackMemory stack;
  uint8_t carryBit[1];
      //uint8_t accumulator[DATA_WIDTH];
public:
  RegisterModule();

      // Clears register data
  void clearModule();

      /*
       * Interfaces for the Emulator, this is used for testing the processor. To
       * modify the internal register module's values, use the write() and
       * read() method.
       *
       */
  uint8_t* getRegisterAt(uint8_t index);
  bool setRegisterAt(uint8_t index, uint8_t* data);
  void setCarryBit(bool flag)
  {
    if(flag == true)
    {
      carryBit[0] = '1';
    }
    else
    {
      carryBit[0] = '0';
    }
  }
  uint8_t* getCarryBit()
  {
    return carryBit;
  }
  // You may modify write's function signature
  void write(uint8_t* write_reg_i, uint8_t* write_data_i);

  // You may modify read's function signature
  void read(uint8_t* reg2_index_i, uint8_t*& reg1_data_o, uint8_t*& reg2_data_o);
};

//==============================================================================
class DecoderModule 
{
private:
  // Optional private variables for print() method
  uint8_t ctrl_reg_dst;
  uint8_t ctrl_branch;
  uint8_t ctrl_mem_read;
  uint8_t ctrl_mem_to_reg;
  uint8_t ctrl_alu_op;
  uint8_t ctrl_mem_write;
  uint8_t ctrl_alu_src;
  uint8_t ctrl_reg_write;

public:
  // You may modify decode's function signature
  void decode(uint8_t* instruction_i
             ,uint8_t* reg_dst_o
             ,uint8_t* branch_bez_o
             ,uint8_t* branch_bnez_o
             ,uint8_t* mem_read_o
             ,uint8_t* mem_to_reg_o
             ,uint8_t*& alu_op_o
             ,uint8_t* mem_write_o
             ,uint8_t* alu_src_o
             ,uint8_t* reg_write_o);

  // Optional: prints the current control signals
  void print();
  int m_memcmp(uint8_t* input_1, const char input_2[], int size)
  {
    for(int i=0; i < size; ++i)
    {
      if(input_1[i] != (input_2[i] - '0'))
      {
        return 0;
      }
    }
    return 1;
  }
  void m_memcpy(uint8_t* dest, uint8_t* depart, int size)
  {
    for(int i=0; i < size; ++i)
    {
      dest[i] = depart[i];
    }
  }
};

//==============================================================================
class ALUModule 
{
public:
void converter_1(uint8_t* input, uint8_t* output)
{
  *output = 0;
  for(int i=0; i <= DATA_WIDTH-1; ++i)
  {
    *output = *output << 1;
    if(input[i] == 1)
    {
      *output = *output + 1;
    }
  }
}

void converter_2(uint8_t* input, uint8_t* output)
{
  uint8_t bit_mask = 128;
  uint8_t temp_result = 0;
  for(int i=0; i < DATA_WIDTH; ++i)
  {
    temp_result = bit_mask & (*input);
    if(temp_result == 0)
    {
      output[i] = 0;
    }
    else
    {
      output[i] = 1;
    }
    *input = *input << 1;
  }
}

int m_memcmp(uint8_t* input_1, const char input_2[], int size)
{
  for(int i=0; i < size; ++i)
  {
    if(input_1[i] != (input_2[i] - '0'))
    {
      return 0;
    }
  }
  return 1;
}

void m_memcpy(uint8_t* dest, uint8_t* depart, int size)
{
  for(int i=0; i < size; ++i)
  {
    dest[i] = depart[i];
  }
}

void compute(uint8_t* ctrl_alu_op_i
            ,uint8_t* reg1_data_i
            ,uint8_t* reg2_data_i
            ,uint8_t* branch_signal
            ,uint8_t*& alu_result_o
            ,uint8_t* carry_bit_i);
};

//==============================================================================
class DataMemoryModule 
{
private:
  DataMemory dmem;

public:
  DataMemoryModule();

  void clearModule();
  void converter_1(uint8_t* input, uint8_t* output)
  {
    *output = 0;
    for(int i=0; i < DATA_WIDTH; ++i)
    {
      *output = *output << 1;
      if(input[i] == 1)
      {
        *output = *output + 1;
      }
    }
  }

      // Initialize data memory 
  bool setDataMemoryWith(char *file);

      /*
       * Interface for the Emulator, this is used for testing the processor. To
       * modify data memory module's values, use the write() and read() method.
       *
       */
  uint8_t* getDataMemoryAt(uint8_t index);
      
      // You may modify write's function signature
  void write(uint8_t* write_addr_i, uint8_t* write_data_i);

      // You may modify read's function signature
  void read(uint8_t* read_addr_i, uint8_t*& data_o);
};

//==============================================================================
class Emulator 
{
private:
  InstructionMemoryModule imemModule;
  RegisterModule          regModule;
  DecoderModule           decoderModule;
  ALUModule               aluModule;
  DataMemoryModule        dmemModule;

  uint8_t pcCurr;
  uint8_t pcNext;

  uint32_t dynamicInstructionCount;
public:
  static const uint8_t ERR_INSTR_LOAD = 1;
  static const uint8_t ERR_DMEM_LOAD  = 2;

  Emulator();

  void reset();
      
  bool loadInstructions(char *file);
  bool loadDataMemory(char *file);

  uint8_t run();

  uint8_t* getInstructionMemoryAt(uint8_t index);
  uint8_t* getDataMemoryAt(uint8_t index);
  uint8_t* getRegisterAt(uint8_t index);
  uint32_t getDynamicInstructionCount();
  int m_memcmp(uint8_t* input_1, const char input_2[], int size)
  {
    for(int i=0; i < size; ++i)
    {
      if(input_1[i] != (input_2[i] - '0'))
      {
        return 0;
      }
    }
    return 1;
  }
};

#endif
