#include <stdio.h>
#include <stdlib.h>

#include "spimcore.h"

/*
gcc -o spimcore spimcore.c project.c
./spimcore input_file.asc
*/

//Tyler Beach, Courtney McCracken, Dennis Shelton

/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
  switch(ALUControl)
  {
    case 0x0:
      *ALUresult = A + B;
      break;
      
    case 0x1:
      *ALUresult = A - B;
      break;

    case 0x2:
      if((int)A < (int)B)
        *ALUresult = 1;
      else
        *ALUresult = 0;
      break;

    case 0x3:
      if(A < B)
        *ALUresult = 1;
      else
        *ALUresult = 0;
      break;

    case 0x4:
      *ALUresult = A & B;
      break;
    
    case 0x5:
      *ALUresult = A | B;
      break;
    
    case 0x6:
      *ALUresult = B << 16;
      break;
    
    case 0x7:
      *ALUresult = ~A;
      break;
  }
  if(*ALUresult == 0)
    *Zero = 1;
  else
    *Zero = 0;
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
  //HALT CONDITIONS
  if(PC % 4 != 0 || PC > 0xffff){
  //printf("PC modulo 4 is %x\n", PC % 4);
  //printf("PC is %x\n", PC); 
  //printf("PC > 0xffff is %d\n", PC > 0xffff);
    return 1;
  }
  else
    *instruction = Mem[PC>>2];

  return 0;
}

/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
  //getting...
  //op instruction [31-26]
  // this gets the first 6 bits by default
  *op = (instruction>>26) & 0x0000003f;
  // printf("%u\n", *op);
  
  //r1 instruction [25-21]
  *r1 = (instruction>>21) & 0x0000001f;

  // This bitmasks appropriately for getting the 5 bits

  //r2 instruction [20-16]
  *r2 = (instruction>>16) & 0x0000001f;
  //So does this

  //r3 instruction [15-11]
  *r3 = (instruction>>11) & 0x0000001f;
  //And this
  
  //funct instruction [5-0]
  *funct = instruction & 0x0000003f;
  //you get the gist
  
  //offset instruction [15-0]
  *offset = instruction & 0x0000ffff;
  
  //jsec instruction [25-0]
  *jsec = instruction & 0x3ffffff;
}

//McCracken

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
  //printf("OpCode: %u\n", op);
  switch (op)
  {
    case 0x0: //r-type
      controls->RegDst = 1;
      controls->ALUSrc = 0;
      controls->MemtoReg = 0;
      controls->RegWrite = 1;
      controls->MemRead = 0;
      controls->MemWrite = 0;
      controls->Branch = 0;
      controls->ALUOp = 7;
      controls->Jump = 0;
      //printf("case 0x0 done (InstOp)\n");
      break;
    case 0x8: //add immediate
      controls->RegDst = 0;
      controls->ALUSrc = 1;
      controls->MemtoReg = 0;
      controls->RegWrite = 1;
      controls->MemRead = 0;
      controls->MemWrite = 0;
      controls->Branch = 0;
      controls->ALUOp = 0;
      controls->Jump = 0;
      //printf("case 0x8 done (InstOp)\n");
      break;
    case 0xf: //load unsigned immediate
      controls->RegDst = 0;
      controls->ALUSrc = 1;
      controls->MemtoReg = 0;
      controls->RegWrite = 1;
      controls->MemRead = 0;
      controls->MemWrite = 0;
      controls->Branch = 0;
      controls->ALUOp = 6; 
      controls->Jump = 0;
      //printf("case 0xf done (InstOp)\n");
      break;
    case 0xd: //or immediate
      controls->RegDst = 0;
      controls->ALUSrc = 1;
      controls->MemtoReg = 0;
      controls->RegWrite = 1;
      controls->MemRead = 0;
      controls->MemWrite = 0;
      controls->Branch = 0;
      controls->ALUOp = 5;
      controls->Jump = 0;
      //printf("case 0xd done (InstOp)\n");
      break;
    case 0x23: //load word
      controls->RegDst = 0;
      controls->ALUSrc = 1;
      controls->MemtoReg = 1;
      controls->RegWrite = 1;
      controls->MemRead = 1;
      controls->MemWrite = 0;
      controls->Branch = 0;
      controls->ALUOp = 0;
      controls->Jump = 0;
      //printf("case 0x23 done (InstOp)\n");
      break;
    case 0x2b: //store word
      controls->RegDst = 2;
      controls->ALUSrc = 1;
      controls->MemtoReg = 2;
      controls->RegWrite = 0;
      controls->MemRead = 0;
      controls->MemWrite = 1;
      controls->Branch = 0;
      controls->ALUOp = 0;
      controls->Jump = 0;
      //printf("case 0x2b done (InstOp)\n");
      break;
    case 0xa: //set less than immediate
      controls->RegDst = 0;
      controls->ALUSrc = 1;
      controls->MemtoReg = 0;
      controls->RegWrite = 1;
      controls->MemRead = 0;
      controls->MemWrite = 0;
      controls->Branch = 0;
      controls->ALUOp = 2;
      controls->Jump = 0;
      //printf("case 0xa done (InstOp)\n");
      break;
    case 0xb: //set less than immediate unsigned
      controls->RegDst = 0;
      controls->ALUSrc = 1;
      controls->MemtoReg = 0;
      controls->RegWrite = 1;
      controls->MemRead = 0;
      controls->MemWrite = 0;
      controls->Branch = 0;
      controls->ALUOp = 3;
      controls->Jump = 0;
      //printf("case 0xa done (InstOp)\n");
      break;
    case 0x4: //branch on equal
      controls->RegDst = 2;
      controls->ALUSrc = 0;
      controls->MemtoReg = 2;
      controls->RegWrite = 0;
      controls->MemRead = 0;
      controls->MemWrite = 0;
      controls->Branch = 1;
      controls->ALUOp = 1;
      controls->Jump = 0;
      //printf("case 0x4 done (InstOp)\n");
      break;
    case 0x2: //jump
      controls->RegDst = 0;
      controls->ALUSrc = 0;
      controls->MemtoReg = 0;
      controls->RegWrite = 0;
      controls->MemRead = 0;
      controls->MemWrite = 0;
      controls->Branch = 0;
      controls->ALUOp = 0;
      controls->Jump = 1;
      //printf("case 0x2 done (InstOp)\n");
      break;
    default:
      //printf("halting at instruction_decode; unknown operation\n");
      return 1;
  }
  return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
  *data1 = Reg[r1];
  *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
  if((offset >> 15) == 1) //negative
    *extended_value = offset | 0xffff0000;
  else //positive
    *extended_value = offset & 0x0000ffff;
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
  if(ALUSrc)
    data2 = extended_value;
  
  if(ALUOp == 0x7)
  {
    switch(funct)
    {
      case 0x6:
        ALUOp = 0x6;
        break;
      case 0x20:
        ALUOp = 0x0;
        break;
      case 0x22:
        ALUOp = 0x1;
        break;
      case 0x24:
        ALUOp = 0x4;
        break;
      case 0x25:
        ALUOp = 0x5;
        break;
      case 0x27:
        ALUOp = 0x7;
        break;
      case 0x2a:
        ALUOp = 0x2;
        break;
      case 0x2b:
        ALUOp = 0x3;
        break;
      default:
        return 1;
    }
  }

  ALU(data1, data2, ALUOp, ALUresult, Zero);
  
  return 0;
}


/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
  if(MemRead)
  {
    if(ALUresult % 4 != 0)
      return 1;
    else
      *memdata = Mem[ALUresult >> 2];
  }

  if(MemWrite)
  {
    if(ALUresult % 4 != 0)
      return 1;
    else
      Mem[ALUresult >> 2] = data2;
  }

  return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
  if(RegWrite == 1 && MemtoReg == 1)
  {
    if(RegDst == 1)
      Reg[r3] = memdata;
    else
      Reg[r2] = memdata;
  }
  if(RegWrite == 1 && MemtoReg == 0)
  {
    if(RegDst == 1)
      Reg[r3] = ALUresult;
    else
      Reg[r2] = ALUresult;
  }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC)
{
  *PC += 4; // shift by 4 for the next set of instructions
  
  if(Jump)
    *PC = (jsec << 2);// | (*PC | 0xf);
  else if(Branch && Zero)
    *PC += (extended_value<<2);
}

/* We love the boi
░░░░░░░░░▄░░░░░░░░░░░░░░▄░░░░
░░░░░░░░▌▒█░░░░░░░░░░░▄▀▒▌░░░
░░░░░░░░▌▒▒█░░░░░░░░▄▀▒▒▒▐░░░
░░░░░░░▐▄▀▒▒▀▀▀▀▄▄▄▀▒▒▒▒▒▐░░░
░░░░░▄▄▀▒░▒▒▒▒▒▒▒▒▒█▒▒▄█▒▐░░░
░░░▄▀▒▒▒░░░▒▒▒░░░▒▒▒▀██▀▒▌░░░ 
░░▐▒▒▒▄▄▒▒▒▒░░░▒▒▒▒▒▒▒▀▄▒▒▌░░
░░▌░░▌█▀▒▒▒▒▒▄▀█▄▒▒▒▒▒▒▒█▒▐░░
░▐░░░▒▒▒▒▒▒▒▒▌██▀▒▒░░░▒▒▒▀▄▌░
░▌░▒▄██▄▒▒▒▒▒▒▒▒▒░░░░░░▒▒▒▒▌░
▐▒▀▐▄█▄█▌▄░▀▒▒░░░░░░░░░░▒▒▒▐░
▐▒▒▐▀▐▀▒░▄▄▒▄▒▒▒▒▒▒░▒░▒░▒▒▒▒▌
▐▒▒▒▀▀▄▄▒▒▒▄▒▒▒▒▒▒▒▒░▒░▒░▒▒▐░
░▌▒▒▒▒▒▒▀▀▀▒▒▒▒▒▒░▒░▒░▒░▒▒▒▌░
░▐▒▒▒▒▒▒▒▒▒▒▒▒▒▒░▒░▒░▒▒▄▒▒▐░░
░░▀▄▒▒▒▒▒▒▒▒▒▒▒░▒░▒░▒▄▒▒▒▒▌░░
*/