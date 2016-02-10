#include <stdlib.h>
#include <stdio.h>
// Data sizes
#define MEMORY_SIZE 1000
#define REGISTERS_SIZE 16

// Boolean
#define FALSE 0
#define TRUE 1

// Access SPW by name
#define ZERO 0
#define NEGATIVE 1

// Access memory address by name
#define BEGIN 0
#define FINAL 1

// Program status
#define HALT -1
#define RUNNING 1;

typedef struct VirtualMachine {
  int PC, SP; // Registradores específicos
  short unsigned int PSW[2]; // Registradores específicos [0] = zero / [1] = negativo
  int memoryAddress[2]; // Endereco de memoria
  int registers[REGISTERS_SIZE]; // Registradores gerais
  int memory[MEMORY_SIZE]; // Memória
  int verbose; // Modo da VM
} VirtualMachine;

// Functions
void createVM(int argc, char *argv[], VirtualMachine *virtualMachine);

void readInstructions(VirtualMachine *virtualMachine, char input[]);

void runProgram(VirtualMachine *virtualMachine);

int getInstruction(VirtualMachine *virtualMachine, int instruction);

int getNextInstruction(VirtualMachine virtualMachine);

char * getInstructionName(int instruction);

void updatePSW(VirtualMachine *virtualMachine, int result);