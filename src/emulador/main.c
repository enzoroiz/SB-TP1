#include <stdio.h>
#include <stdlib.h>
#include "virtualmachine.h"

int main(int argc, char *argv[]){
    VirtualMachine virtualMachine;
    createVM(argc, argv, &virtualMachine);
    readInstructions(&virtualMachine, argv[4]);
    runProgram(&virtualMachine);
    return 0;
}