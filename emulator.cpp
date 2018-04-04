//Name: Phillip Jo
/*
 * University of California, San Diego
 *    
 *    CSE141L Summer 2017
 *
 */

#include "emulator.h"
#define OP_CODE_SIZE 5
//==============================================================================
// Instruction Memory Module
//==============================================================================
InstructionMemoryModule::InstructionMemoryModule() 
{
  this->clearModule();
}

void InstructionMemoryModule::clearModule() 
{
  this->imem.clear();
}
/* 
 * Returns the i-th instruction 
 *
 */
uint8_t* InstructionMemoryModule::getInstructionMemoryAt(uint8_t index) 
{
  return this->imem.get(index);
}

/* 
 * Load instruction memory from input file
 *
 */
bool InstructionMemoryModule::setInstructionMemoryWith(char *file) 
{
  uint8_t* charPtr = (uint8_t*)malloc(INSTR_WIDTH);
  FILE* file_in = fopen(file, "r");
/************
  if(file_in != NULL)
  { 
    printf("NULL pointer \n");
    printf("File Open Error \n");
    return false;
  }
**************/
  int i = 0;
  while(fscanf(file_in, "%s", charPtr) == 1)
  {
    for(int j=0; j < INSTR_WIDTH; ++j)
    {
      charPtr[j] = charPtr[j] - '0';
    }
    this->imem.set(i, charPtr);
    ++i;
    if(i == IMEM_DEPTH)
    {
      break;
    }
  }
  free(charPtr);
  return true;
}

void InstructionMemoryModule::fetch(uint8_t pc_i, uint8_t*& instruction_o) 
{
  instruction_o = getInstructionMemoryAt(pc_i);
}


//==============================================================================
// Register File Module
//==============================================================================

/*
 * Constructor
 *
 * Initialize your register module
 *
 */
RegisterModule::RegisterModule() 
{
  carryBit[0] = '0';
}

/*
 * Clear register module
 *
 */
void RegisterModule::clearModule() 
{
  carryBit[0] = '0';
  this->rf.clear();
}

/* 
 * Returns the i-th register value
 */
uint8_t* RegisterModule::getRegisterAt(uint8_t index) 
{ 
  return this->rf.get(index);
}

bool RegisterModule::setRegisterAt(uint8_t index, uint8_t* data) 
{
  return this->rf.set(index, data);
}

// You may modify write's function signature
void RegisterModule::write(uint8_t* write_reg_i, uint8_t* write_data_i) 
{
  uint8_t index = 0;
  for(int i=0; i < 4; ++i)
  {
    index = index << 1;
    if(write_reg_i[i] == 1)
    {
      index = index + 1;
    }
  }
  this->rf.set(index, write_data_i);
}

// You may modify read's function signature
void RegisterModule::read(uint8_t* reg2_index_i, uint8_t*& reg1_data_o, uint8_t*& reg2_data_o) 
{
  uint8_t index_2 = 0;
  for(int i=0; i < 4; ++i)
  {
    index_2 = index_2 << 1;
    if(reg2_index_i[i] == 1)
    {
      index_2 = index_2 + 1;
    }
  }
  reg1_data_o = this->rf.get(0);
  reg2_data_o = this->rf.get(index_2);
}

//==============================================================================
// Decoder Module
//==============================================================================

/*
 * Control signal decorder
 *
 */
void DecoderModule::decode(uint8_t* instruction_i
                          ,uint8_t* reg_dst_o
                          ,uint8_t* branch_bez_o
                          ,uint8_t* branch_bnez_o
                          ,uint8_t* mem_read_o
                          ,uint8_t* mem_to_reg_o
                          ,uint8_t*& alu_op_o
                          ,uint8_t* mem_write_o
                          ,uint8_t* alu_src_o
                          ,uint8_t* reg_write_o)
{
  if(instruction_i[0] == 1)
  {
    *reg_dst_o = '0'; 
    *branch_bez_o = '0';
    *branch_bnez_o = '0';
    *mem_read_o = '0';
    *mem_to_reg_o = '0';
    m_memcpy(alu_op_o, instruction_i, OP_CODE_SIZE);
    *mem_write_o = '0';
    *alu_src_o = '1';
    *reg_write_o = '1';
  }
  else if(m_memcmp(instruction_i, "00000", OP_CODE_SIZE)) // set reg
  {
    *reg_dst_o = '0'; 
    *branch_bez_o = '0';
    *branch_bnez_o = '0';
    *mem_read_o = '0';
    *mem_to_reg_o = '0';
    m_memcpy(alu_op_o, instruction_i, OP_CODE_SIZE);
    *mem_write_o = '0';
    *alu_src_o = '0';
    *reg_write_o = '1';
  }
  else if(m_memcmp(instruction_i, "00001", OP_CODE_SIZE)) // lw reg
  {
    *reg_dst_o = '1'; 
    *branch_bez_o = '0';
    *branch_bnez_o = '0';
    *mem_read_o = '1';
    *mem_to_reg_o = '1';
    m_memcpy(alu_op_o, instruction_i, OP_CODE_SIZE);
    *mem_write_o = '0';
    *alu_src_o = '0';
    *reg_write_o = '1';
  }
  else if(m_memcmp(instruction_i, "00010", OP_CODE_SIZE)) // add reg
  {
    *reg_dst_o = '0'; 
    *branch_bez_o = '0';
    *branch_bnez_o = '0';
    *mem_read_o = '0';
    *mem_to_reg_o = '0';
    m_memcpy(alu_op_o, instruction_i, OP_CODE_SIZE);
    *mem_write_o = '0';
    *alu_src_o = '0';
    *reg_write_o = '1';
  }
  else if(m_memcmp(instruction_i, "00011", OP_CODE_SIZE)) // bez imm
  {
    *reg_dst_o = '0'; 
    *branch_bez_o = '1';
    *branch_bnez_o = '0';
    *mem_read_o = '0';
    *mem_to_reg_o = '0';
    m_memcpy(alu_op_o, instruction_i, OP_CODE_SIZE);
    *mem_write_o = '0';
    *alu_src_o = '0';
    *reg_write_o = '0';
  }
  else if(m_memcmp(instruction_i, "00100", OP_CODE_SIZE)) // bnez imm
  {
    *reg_dst_o = '0'; 
    *branch_bez_o = '0';
    *branch_bnez_o = '1';
    *mem_read_o = '0';
    *mem_to_reg_o = '0';
    m_memcpy(alu_op_o, instruction_i, OP_CODE_SIZE);
    *mem_write_o = '0';
    *alu_src_o = '0';
    *reg_write_o = '0';
  }
  else if(m_memcmp(instruction_i, "00101", OP_CODE_SIZE)) // slo reg
  {
    *reg_dst_o = '1'; 
    *branch_bez_o = '0';
    *branch_bnez_o = '0';
    *mem_read_o = '0';
    *mem_to_reg_o = '0';
    m_memcpy(alu_op_o, instruction_i, OP_CODE_SIZE);
    *mem_write_o = '0';
    *alu_src_o = '0';
    *reg_write_o = '1';
  }
  else if(m_memcmp(instruction_i, "00110", OP_CODE_SIZE)) // slg reg
  {
    *reg_dst_o = '1'; 
    *branch_bez_o = '0';
    *branch_bnez_o = '0';
    *mem_read_o = '0';
    *mem_to_reg_o = '0';
    m_memcpy(alu_op_o, instruction_i, OP_CODE_SIZE);
    *mem_write_o = '0';
    *alu_src_o = '0';
    *reg_write_o = '1';
  }
  else if(m_memcmp(instruction_i, "00111", OP_CODE_SIZE)) // addone reg
  {
    *reg_dst_o = '1'; 
    *branch_bez_o = '0';
    *branch_bnez_o = '0';
    *mem_read_o = '0';
    *mem_to_reg_o = '0';
    m_memcpy(alu_op_o, instruction_i, OP_CODE_SIZE);
    *mem_write_o = '0';
    *alu_src_o = '0';
    *reg_write_o = '1';
  }
  else if(m_memcmp(instruction_i, "01000", OP_CODE_SIZE)) // srg reg
  {
    *reg_dst_o = '1'; 
    *branch_bez_o = '0';
    *branch_bnez_o = '0';
    *mem_read_o = '0';
    *mem_to_reg_o = '0';
    m_memcpy(alu_op_o, instruction_i, OP_CODE_SIZE);
    *mem_write_o = '0';
    *alu_src_o = '0';
    *reg_write_o = '1';
  }
  else if(m_memcmp(instruction_i, "01001", OP_CODE_SIZE)) // mov reg
  {
    *reg_dst_o = '1'; 
    *branch_bez_o = '0';
    *branch_bnez_o = '0';
    *mem_read_o = '0';
    *mem_to_reg_o = '0';
    m_memcpy(alu_op_o, instruction_i, OP_CODE_SIZE);
    *mem_write_o = '0';
    *alu_src_o = '0';
    *reg_write_o = '1';
  }
  else if(m_memcmp(instruction_i, "01010", OP_CODE_SIZE)) // sub reg
  {
    *reg_dst_o = '0'; 
    *branch_bez_o = '0';
    *branch_bnez_o = '0';
    *mem_read_o = '0';
    *mem_to_reg_o = '0';
    m_memcpy(alu_op_o, instruction_i, OP_CODE_SIZE);
    *mem_write_o = '0';
    *alu_src_o = '0';
    *reg_write_o = '1';
  }
  else if(m_memcmp(instruction_i, "01011", OP_CODE_SIZE)) // flip reg
  {
    *reg_dst_o = '1'; 
    *branch_bez_o = '0';
    *branch_bnez_o = '0';
    *mem_read_o = '0';
    *mem_to_reg_o = '0';
    m_memcpy(alu_op_o, instruction_i, OP_CODE_SIZE);
    *mem_write_o = '0';
    *alu_src_o = '0';
    *reg_write_o = '1';
  }
  else if(m_memcmp(instruction_i, "01100", OP_CODE_SIZE)) // and reg
  {
    *reg_dst_o = '0'; 
    *branch_bez_o = '0';
    *branch_bnez_o = '0';
    *mem_read_o = '0';
    *mem_to_reg_o = '0';
    m_memcpy(alu_op_o, instruction_i, OP_CODE_SIZE);
    *mem_write_o = '0';
    *alu_src_o = '0';
    *reg_write_o = '1';
  }
  else if(m_memcmp(instruction_i, "01101", OP_CODE_SIZE)) // xor reg
  {
    *reg_dst_o = '0'; 
    *branch_bez_o = '0';
    *branch_bnez_o = '0';
    *mem_read_o = '0';
    *mem_to_reg_o = '0';
    m_memcpy(alu_op_o, instruction_i, OP_CODE_SIZE);
    *mem_write_o = '0';
    *alu_src_o = '0';
    *reg_write_o = '1';
  }
  else if(m_memcmp(instruction_i, "01110", OP_CODE_SIZE)) // sw reg
  {
    *reg_dst_o = '0'; 
    *branch_bez_o = '0';
    *branch_bnez_o = '0';
    *mem_read_o = '0';
    *mem_to_reg_o = '0';
    m_memcpy(alu_op_o, instruction_i, OP_CODE_SIZE);
    *mem_write_o = '1';
    *alu_src_o = '0';
    *reg_write_o = '0';
  }
}

void DecoderModule::print() 
{}
//==============================================================================
// ALU Module
//==============================================================================

/*
 * Arithmetic logic unit
 *
 */
void ALUModule::compute(uint8_t* ctrl_alu_op_i
                       ,uint8_t* reg1_data_i
                       ,uint8_t* reg2_data_i
                       ,uint8_t* branch_signal
                       ,uint8_t*& alu_result_o
                       ,uint8_t* carry_bit_i) 
{
  if(ctrl_alu_op_i[0] == 1) // seti imm
  {
    m_memcpy(alu_result_o, reg2_data_i, DATA_WIDTH);
    branch_signal[0] = '0';
  }
  else if(m_memcmp(ctrl_alu_op_i, "00000", OP_CODE_SIZE)) // set reg
  {
    m_memcpy(alu_result_o, reg2_data_i, DATA_WIDTH);
    branch_signal[0] = '0';
  }
  else if(m_memcmp(ctrl_alu_op_i, "00001", OP_CODE_SIZE))
  {
    m_memcpy(alu_result_o, reg1_data_i, DATA_WIDTH);
    branch_signal[0] = '0';
  }    
  else if(m_memcmp(ctrl_alu_op_i, "00010", OP_CODE_SIZE)) // add reg
  {
    uint8_t reg_1 = 0;
    uint8_t reg_2 = 0;
    uint8_t result = 0;
    converter_1(reg1_data_i, &reg_1);
    converter_1(reg2_data_i, &reg_2);
    result = reg_1 + reg_2;
    converter_2(&result, alu_result_o);
    branch_signal[0] = '0';
  }
  else if(m_memcmp(ctrl_alu_op_i, "00011", OP_CODE_SIZE))
  {
    if(m_memcmp(reg1_data_i, "00000000", DATA_WIDTH))
    {
      branch_signal[0] = '1';
    }
    else
    {
      branch_signal[0] = '0';
    }
  }
  else if(m_memcmp(ctrl_alu_op_i, "00100", OP_CODE_SIZE))
  {
    if(m_memcmp(reg1_data_i, "00000000", DATA_WIDTH))
    {
      branch_signal[0] = '0';
    }
    else
    {
      branch_signal[0] = '1';
    }
  }
  else if(m_memcmp(ctrl_alu_op_i, "00101", OP_CODE_SIZE)) // slo reg
  {
    bool carry_set;
    if(reg2_data_i[0] == 1)
    {
      carry_set = true;
    }
    else
    {
      carry_set = false;
    }
    uint8_t result;
    this->converter_1(reg2_data_i, &result);
    result = result << 1;
    if(carry_bit_i[0] == '1')
    {
      result = result + 1;
    }  
    this->converter_2(&result, alu_result_o);
    if(carry_set == true)
    {
      carry_bit_i[0] = '1';
    }
    else
    {
      carry_bit_i[0] = '0';
    }
    branch_signal[0] = '0';
  }
  else if(m_memcmp(ctrl_alu_op_i, "00110", OP_CODE_SIZE)) // slg reg
  {
    bool carry_set;
    if(reg2_data_i[0] == 1)
    {
      carry_set = true;
    }
    else
    {
      carry_set = false;
    }
    uint8_t result;
    this->converter_1(reg2_data_i, &result);
    result = result << 1;  
    this->converter_2(&result, alu_result_o);
    if(carry_set == true)
    {
      carry_bit_i[0] = '1';
    }
    else
    {
      carry_bit_i[0] = '0';
    }
    branch_signal[0] = '0';
  }
  else if(m_memcmp(ctrl_alu_op_i, "00111", OP_CODE_SIZE)) // addone reg;
  {
    uint8_t result = 0;
    this->converter_1(reg2_data_i, &result);
    ++result;
    this->converter_2(&result, alu_result_o);
    branch_signal[0] = '0';
  }
  else if(m_memcmp(ctrl_alu_op_i, "01000", OP_CODE_SIZE)) // srg reg;
  {
    uint8_t result;
    this->converter_1(reg2_data_i, &result);
    result = result >> 1;
    this->converter_2(&result, alu_result_o);
    branch_signal[0] = '0';
  }
  else if(m_memcmp(ctrl_alu_op_i, "01001", OP_CODE_SIZE)) // mov reg;
  {
    m_memcpy(alu_result_o, reg1_data_i, DATA_WIDTH);
    branch_signal[0] = '0';
  }
  else if(m_memcmp(ctrl_alu_op_i, "01010", OP_CODE_SIZE)) // sub reg;
  {
    uint8_t reg_1 = 0;
    uint8_t reg_2 = 0;
    uint8_t result = 0;
    converter_1(reg1_data_i, &reg_1);
    converter_1(reg2_data_i, &reg_2);
    result = reg_1 - reg_2;
    converter_2(&result, alu_result_o);
    branch_signal[0] = '0';
  }
  else if(m_memcmp(ctrl_alu_op_i, "01011", OP_CODE_SIZE)) // flip reg;
  {
    for(int i=0; i < DATA_WIDTH; ++i)
    {
      if(reg2_data_i[i] == 1)
      {
        alu_result_o[i] = 0;
      }
      else
      {
        alu_result_o[i] = 1;
      }
    }
    branch_signal[0] = '0';
  }
  else if(m_memcmp(ctrl_alu_op_i, "01100", OP_CODE_SIZE)) 
  {
    uint8_t reg_1 = 0;
    uint8_t reg_2 = 0;
    uint8_t result = 0;
    converter_1(reg1_data_i, &reg_1);
    converter_1(reg2_data_i, &reg_2);
    result = reg_1 & reg_2;
    converter_2(&result, alu_result_o);
    branch_signal[0] = '0';
  }
  else if(m_memcmp(ctrl_alu_op_i, "01101", OP_CODE_SIZE)) 
  {
    uint8_t reg_1 = 0;
    uint8_t reg_2 = 0;
    uint8_t result = 0;
    converter_1(reg1_data_i, &reg_1);
    converter_1(reg2_data_i, &reg_2);
    result = reg_1 ^ reg_2;
    converter_2(&result, alu_result_o);
    branch_signal[0] = '0';
  }
  else if(m_memcmp(ctrl_alu_op_i, "01110", OP_CODE_SIZE)) 
  {
    m_memcpy(alu_result_o, reg1_data_i, DATA_WIDTH);
    branch_signal[0] = '0';
  }
}
//==============================================================================
// Data Memory Module
//==============================================================================

/*
 * DO NOT MODIFY
 *
 */
DataMemoryModule::DataMemoryModule() 
{
   this->clearModule();
}
/*
 * DO NOT MODIFY
 *
 */
void DataMemoryModule::clearModule() 
{
   this->dmem.clear();
}

uint8_t* DataMemoryModule::getDataMemoryAt(uint8_t index) 
{
  return this->dmem.get(index);
}

bool DataMemoryModule::setDataMemoryWith(char *file) 
{
  uint8_t* charPtr = (uint8_t*)malloc(DATA_WIDTH);
  FILE* file_in = fopen(file, "r");
/*************************
  if(file_in != NULL)
  {
    printf("File Open Error \n");
    return false;
  }
****************************/
  int i = 0;
  while(fscanf(file_in, "%s", charPtr) == 1)
  {
    for(int j=0; j < DATA_WIDTH; ++j)
    {
      charPtr[j] = charPtr[j] - '0';
    }
    this->dmem.set(i, charPtr);
    ++i;
    if(i == DMEM_DEPTH)
    {
      break;
    }
  }
  free(charPtr);
  return true;
}

// You may modify write's function signature
void DataMemoryModule::write(uint8_t* write_addr_i ,uint8_t* write_data_i) 
{
  uint8_t index = 0;
  this->converter_1(write_addr_i, &index);
  if(!(this->dmem.set(index, write_data_i)))
  {
    printf("Write to MEM error");
  }
}

// You may modify read's function signature
void DataMemoryModule::read(uint8_t* read_addr_i, uint8_t*& data_o) 
{
  uint8_t index = 0;
  this->converter_1(read_addr_i, &index);
  data_o = this->dmem.get(index);  // It is a deep copy but it points to a memory location in a heap so you need to free it later......
}

//==============================================================================
// Emulator for your specific ISA
//==============================================================================

/*
 * Initialize emulator
 *
 * DO NOT MODIFY
 *
 */
Emulator::Emulator() 
{
  this->reset();
}

/*
 * Reset RF, IMem, DMem, and PC
 *
 * DO NOT MODIFY
 *
 */
void Emulator::reset() 
{
  this->dynamicInstructionCount = 0;
  this->pcCurr = 0;
  this->pcNext = 1;
  this->imemModule.clearModule();
  this->regModule.clearModule();
  this->dmemModule.clearModule();
}

/*
 * DO NOT MODIFY
 *
 */
uint8_t* Emulator::getRegisterAt(uint8_t index) 
{
  return this->regModule.getRegisterAt(index);
}

/*
 * DO NOT MODIFY
 *
 */
uint8_t* Emulator::getInstructionMemoryAt(uint8_t index) 
{
  return this->imemModule.getInstructionMemoryAt(index);
}

/*
 * DO NOT MODIFY
 *
 */
uint8_t* Emulator::getDataMemoryAt(uint8_t index) 
{
  return this->dmemModule.getDataMemoryAt(index);
}


bool Emulator::loadInstructions(char *file) 
{
  return this->imemModule.setInstructionMemoryWith(file);
}

bool Emulator::loadDataMemory(char *file) 
{
  return this->dmemModule.setDataMemoryWith(file);
}

/*
 * Simulate single cycle processor
 *
 */
uint8_t Emulator::run() 
{
  uint8_t* inst_ptr = NULL;
  uint8_t* opcode_part = (uint8_t*)malloc(5);
  uint8_t* reg_part = (uint8_t*)malloc(4); 
  uint8_t* imm_part = (uint8_t*)malloc(8);
  uint8_t reg_dst;
  uint8_t branch_bez;
  uint8_t branch_bnez;
  uint8_t mem_read;
  uint8_t mem_to_reg;
  uint8_t* alu_op = (uint8_t*)malloc(5);
  uint8_t mem_write;
  uint8_t alu_src;
  uint8_t reg_write;
  uint8_t* reg1_data = NULL;
  uint8_t* reg2_data = NULL;
  uint8_t* alu_result = (uint8_t*)malloc(DATA_WIDTH);
  uint8_t* mem_read_data = NULL;
  uint8_t alu_branch;
  pcNext = 0;
  while(1)
  {
    pcCurr = pcNext;
    inst_ptr = this->getInstructionMemoryAt(pcCurr);
    for(int j=0; j < 5; ++j)
    {
      opcode_part[j] = inst_ptr[j];
    }
    for(int j=0; j < 4; ++j)
    {
      reg_part[j] = inst_ptr[j+5];
    }
    for(int j=0; j < 8; ++j)
    {
      imm_part[j] = inst_ptr[j+1];
    }
    if(m_memcmp(opcode_part, "01111", OP_CODE_SIZE))
    {
      break; // halt instruction
    }
    decoderModule.decode(opcode_part, &reg_dst, &branch_bez, &branch_bnez, &mem_read, &mem_to_reg, alu_op, &mem_write, &alu_src, &reg_write);
    regModule.read(reg_part, reg1_data, reg2_data);
    if(alu_src == '0')
    {
      aluModule.compute(alu_op, reg1_data, reg2_data, &alu_branch, alu_result, regModule.getCarryBit());
    }
    else
    {
      aluModule.compute(alu_op, reg1_data, imm_part, &alu_branch, alu_result, regModule.getCarryBit());
    }
    if(mem_read == '1')
    {
      dmemModule.read(alu_result, mem_read_data);
    }
    if(mem_write == '1')
    {
      dmemModule.write(alu_result, reg2_data);
    }
    if(reg_write == '1')
    {
      if(mem_to_reg == '1')
      {
        if(reg_dst == '0')
        {
          uint8_t zero_reg[4] = {0, 0, 0, 0};
          regModule.write(zero_reg, mem_read_data);
        }
        else
        {
          regModule.write(reg_part, mem_read_data);
        }
      } 
      else
      {
        if(reg_dst == '0')
        {
          uint8_t zero_reg[4] = {0, 0, 0, 0};
          regModule.write(zero_reg, alu_result);
        }
        else
        {
          regModule.write(reg_part, alu_result);
        }
      } 
    }
    if((alu_branch == '1' && branch_bez == '1') || (alu_branch == '1' && branch_bnez == '1'))
    {
      int8_t pc_offset = 0;
      for(int i=0; i < 4; ++i)
      {
        pc_offset = pc_offset << 1;
        if(reg_part[i] == 1)
        {
          pc_offset = pc_offset + 1;
        }
      }
      if(reg_part[0] == 1)
      {
        pc_offset = pc_offset - 16;
      }
      pc_offset = pc_offset << 1;
      if(branch_bez == '1')
      {
        pcNext = pcCurr + pc_offset - 2;
      }
      else
      {
        pcNext = pcCurr + pc_offset - 4;
      }
    }
    else
    {
      pcNext = pcCurr + 1;
    }
    this->dynamicInstructionCount++;
    //printf("Inst Count: %d \n", dynamicInstructionCount);
  }
  free(opcode_part);
  free(reg_part);
  free(imm_part);
  free(alu_op);
  free(alu_result);
  free(reg1_data);
  free(reg2_data);
  free(mem_read_data);
  free(inst_ptr);
  return 0;
}

uint32_t Emulator::getDynamicInstructionCount() 
{
  return this->dynamicInstructionCount;
}

