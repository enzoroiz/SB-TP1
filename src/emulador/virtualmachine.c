#include <stdio.h>
#include <stdlib.h>
#include "virtualmachine.h"

void createVM(int argc, char *argv[], VirtualMachine *virtualMachine){
	// Inicializa registradores 
	virtualMachine->PC = atoi(argv[1]);
	virtualMachine->SP = atoi(argv[2]);
	virtualMachine->PSW[ZERO] = 0;
	virtualMachine->PSW[NEGATIVE] = 0; 

	// Inicializa memória
	virtualMachine->memoryAddress[BEGIN] = atoi(argv[3]);
	virtualMachine->memoryAddress[FINAL] = atoi(argv[3]);

	// Inicializa modo da VM
	if((argc != 6) || (argv[5][0] == 's')){
		virtualMachine->verbose = FALSE;
	} else {
		virtualMachine->verbose = TRUE;
	}

	// Inicializando os registradores
	int i;
	for(i=0; i<REGISTERS_SIZE; i++){
		virtualMachine->registers[i] = 0;
	}

	// Inicializando a memória
	for(i=0; i<MEMORY_SIZE; i++){
		virtualMachine->memory[i] = 0;
	}
}

void readInstructions(VirtualMachine *virtualMachine, char input[]){
	FILE *inputProgram;
	int memoryAddress;

	// Abre arquivo e pega posição de início da memória
	inputProgram = fopen(input, "r");
	memoryAddress = virtualMachine->memoryAddress[BEGIN];

	// Lê todo arquivo caso não estoure a memória disponível
	while((!feof(inputProgram)) && (memoryAddress < MEMORY_SIZE)){
		fscanf(inputProgram, "%d\n", &virtualMachine->memory[memoryAddress]);
		memoryAddress ++;
	}

	// Atualiza memória final e fecha o arquivo que foi lido
	virtualMachine->memoryAddress[FINAL] = memoryAddress;
	fclose(inputProgram);
}

int getNextInstruction(VirtualMachine virtualMachine){
	int position;
	position = virtualMachine.memoryAddress[BEGIN] + virtualMachine.PC;

	if(position == virtualMachine.memoryAddress[FINAL]){
		return HALT;
	} else {
		return virtualMachine.memory[position];
	}
}

void printInstructionName(int instruction){
	switch(instruction){
		case 1:{
			printf("LOAD\n");
			break;
		}

		case 2:{
			printf("STORE\n");
			break;
		}

		case 3:{
			printf("READ\n");
			break;
		}

		case 4:{
			printf("WRITE\n");
			break;
		}

		case 5:{
			printf("COPY\n");
			break;
		}

		case 6:{
			printf("NEG\n");
			break;
		}

		case 7:{
			printf("SUB\n");
			break;
		}

		case 8:{
			printf("ADD\n");
			break;
		}

		case 9:{
			printf("AND\n");
			break;
		}

		case 10:{
			printf("OR\n");
			break;
		}

		case 11:{
			printf("XOR\n");
			break;
		}

		case 12:{
			printf("NOT\n");
			break;
		}

		case 13:{
			printf("JMP\n");
			break;
		}

		case 14:{
			printf("JZ\n");
			break;
		}

		case 15:{
			printf("JNZ\n");
			break;
		}

		case 16:{
			printf("JN\n");
			break;
		}

		case 17:{
			printf("JNN\n");
			break;
		}

		case 18:{
			printf("PUSH\n");
			break;
		}

		case 19:{
			printf("POP\n");
			break;
		}

		case 20:{
			printf("CALL\n");
			break;
		}

		case 21:{
			printf("RET\n");
			break;
		}

		case 22:{
			printf("HALT\n");
			break;
		}
	}
}

void runProgram(VirtualMachine *virtualMachine){
	int i, instruction, status = RUNNING;
	instruction = getNextInstruction(*virtualMachine);
	
	// Enquanto houver instruções
	while((instruction != HALT) && (status != HALT)){
		// Incrementa PC e pega instrução correspondente
		virtualMachine->PC++;
		status = getInstruction(virtualMachine, instruction);

		// Se verbose então imprime
		if(virtualMachine->verbose){
			// Nome instrução
			printInstructionName(instruction);

			// Registradores específicos
			printf("PC: %d SP: %d PSW[ZERO]: %d PSW[NEGATIVE]: %d\n", virtualMachine->PC, virtualMachine->SP, virtualMachine->PSW[ZERO], virtualMachine->PSW[NEGATIVE]);

			// Registradores gerais
			for(i=0; i<REGISTERS_SIZE; i++){
				printf("[%d]: %d ", i, virtualMachine->registers[i]);
			}

			printf("\n");
		}

		instruction = getNextInstruction(*virtualMachine);
	}
}


int getInstruction(VirtualMachine *virtualMachine, int instruction){
	int register1;
	int register2;
	int memoryAddress;
	int status = RUNNING;
	int offset = virtualMachine->memoryAddress[BEGIN];

	switch (instruction){
		// LOAD
		case 1:{
			register1 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			memoryAddress = getNextInstruction(*virtualMachine) + offset;
			virtualMachine->PC++;
			virtualMachine->registers[register1] = virtualMachine->memory[memoryAddress + virtualMachine->PC];
			break;
		}

		//STORE
		case 2:{
			register1 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			memoryAddress = getNextInstruction(*virtualMachine) + offset;
			virtualMachine->PC++;
			virtualMachine->memory[memoryAddress + virtualMachine->PC] = virtualMachine->registers[register1];
			break;
		}

		// READ
		case 3:{
			int aux;
			register1 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			printf("Type the number to be read: ");
			scanf("%d", &aux);
			virtualMachine->registers[register1] = aux;
			break;
		}

		// WRITE
		case 4:{
			register1 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			printf("Writing register: %d\n", virtualMachine->registers[register1]);
			break;
		}

		// COPY
		case 5:{
			register1 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			register2 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			virtualMachine->registers[register1] = virtualMachine->registers[register2];
			updatePSW(virtualMachine, virtualMachine->registers[register1]);
			break;
		}

		// NEG
		case 6:{
			register1 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			virtualMachine->registers[register1] = -(virtualMachine->registers[register1]);
			updatePSW(virtualMachine, virtualMachine->registers[register1]);
			break;
		}

		//SUB
		case 7:{
			register1 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			register2 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			virtualMachine->registers[register1] = virtualMachine->registers[register1] - virtualMachine->registers[register2];
			updatePSW(virtualMachine, virtualMachine->registers[register1]);
			break;
		}

		// ADD
		case 8:{
			register1 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			register2 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			virtualMachine->registers[register1] = virtualMachine->registers[register1] + virtualMachine->registers[register2];
			updatePSW(virtualMachine, virtualMachine->registers[register1]);
			break;
		}

		// AND
		case 9:{
			register1 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			register2 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			virtualMachine->registers[register1] = virtualMachine->registers[register1] & virtualMachine->registers[register2];
			updatePSW(virtualMachine, virtualMachine->registers[register1]);
			break;
		}

		// OR
		case 10:{
			register1 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			register2 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			virtualMachine->registers[register1] = virtualMachine->registers[register1] | virtualMachine->registers[register2];
			updatePSW(virtualMachine, virtualMachine->registers[register1]);
			break;
		}

		// XOR
		case 11:{
			register1 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			register2 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			virtualMachine->registers[register1] = virtualMachine->registers[register1] ^ virtualMachine->registers[register2];
			updatePSW(virtualMachine, virtualMachine->registers[register1]);
			break;
		}

		// NOT
		case 12:{
			register1 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			virtualMachine->registers[register1] = ~(virtualMachine->registers[register1]);
			updatePSW(virtualMachine, virtualMachine->registers[register1]);
			break;
		}

		//JMP
		case 13:{
			memoryAddress = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			virtualMachine->PC = virtualMachine->PC + memoryAddress;
			break;
		}

		//JZ
		case 14:{
			memoryAddress = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			
			if(virtualMachine->PSW[ZERO]){
				virtualMachine->PC = virtualMachine->PC + memoryAddress;
			}

			break;
		}

		//JNZ
		case 15:{
			memoryAddress = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;

			if(!virtualMachine->PSW[ZERO]){
				virtualMachine->PC = virtualMachine->PC + memoryAddress;
			}
			
			break;
		}

		//JN
		case 16:{
			memoryAddress = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;

			if(virtualMachine->PSW[NEGATIVE]){
				virtualMachine->PC = virtualMachine->PC + memoryAddress;
			}
			
			break;
		}

		//JNN
		case 17:{
			memoryAddress = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;

			if(!virtualMachine->PSW[NEGATIVE]){
				virtualMachine->PC = virtualMachine->PC + memoryAddress;
			}
			
			break;
		}

		//PUSH
		case 18:{
			register1 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			virtualMachine->SP = virtualMachine->SP - 1;
			virtualMachine->memory[virtualMachine->SP] = virtualMachine->registers[register1];
			break;
		}

		//POP
		case 19:{
			register1 = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			virtualMachine->registers[register1] = virtualMachine->memory[virtualMachine->SP];
			virtualMachine->SP = virtualMachine->SP + 1;
			break;
		}

		//CALL
		case 20:{
			memoryAddress = getNextInstruction(*virtualMachine);
			virtualMachine->PC++;
			virtualMachine->SP = virtualMachine->SP - 1;
			virtualMachine->memory[virtualMachine->SP] = virtualMachine->PC;
			virtualMachine->PC = virtualMachine->PC + memoryAddress;
			break;
		}

		//RET
		case 21:{
			virtualMachine->PC = virtualMachine->memory[virtualMachine->SP];
			virtualMachine->SP = virtualMachine->SP + 1;
			break;
		}

		case 22:{
			status = HALT;
			break;
		}
	}

	return status;
}

void updatePSW(VirtualMachine *virtualMachine, int result){
	// Check if negative
	if(result < 0){
		virtualMachine->PSW[NEGATIVE] = TRUE;
		virtualMachine->PSW[ZERO] = FALSE;
	} 
	// Check if zero
	else if(result == 0) {
		virtualMachine->PSW[NEGATIVE] = FALSE;
		virtualMachine->PSW[ZERO] = TRUE;
	} 
	// It is a positive number
	else {
		virtualMachine->PSW[NEGATIVE] = FALSE;
		virtualMachine->PSW[ZERO] = FALSE;
	}
}

// READ atualiza PSW ?
// Valores default para os bits de PSW 0 e 0