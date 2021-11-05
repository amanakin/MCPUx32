#include <string.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>  

#include "M_CPUx32.h"
#include "../asm/asm.h"
#include "../stack/stack.h"

// [ #### introduce ########################################## ]

void Tsleep(int time) {
	long long int nano = 10000 * time;
	for (int ms = 0; ms < nano; ms++) {
		int b = ms * ms;
	}
}

void SlowLine(char *line, size_t strlen) {
	for (int ch = 0; ch < strlen; ch++) {
		putchar(line[ch]);
		Tsleep(1000);
	}
}

void SlowErase(size_t strlen) {
	for (int ch = 0; ch < strlen; ch++) {
		putchar('\b');
		putchar(' ');
		putchar('\b');
		Tsleep(1000);
	}
}

void CP_Intro() {
	
	char labelLoad[] = "Processor M_CPUx32:";
	SlowLine(labelLoad, strlen(labelLoad));

	char version[] = "  .cur_ver:  ";
	SlowLine(version, strlen(version));

	putchar('<');
	char curVer[5] = "";
	strcpy(curVer, STR_VERSION);
	int stlen = strlen(curVer);
	SlowLine(curVer, 5);
	putchar('>');
	stlen == 20;

	sleep(2);
	SlowErase(20);
	SlowErase(strlen(version));
	SlowErase(strlen(labelLoad));

	char labelLoad2[] = "NAME OF THE PROGRAM: ";
	SlowLine(labelLoad2, strlen(labelLoad2));
	
}

// [ #### PREPARE PROCESS #################################### ]
EXECUTE_ERROR CP_DataCtor(CPU_32 *proc) {
	assert(proc != nullptr);

	if (RAM_Init(&proc->ram) != EXECUTE_ERROR::noErr) {
		return EXECUTE_ERROR::ramNullptr;
		proc->state = cpuState::BROKEN;
	}
	stackInit(&(proc->stack));

	proc->state = cpuState::OFF;

	return EXECUTE_ERROR::noErr;
}

EXECUTE_ERROR RAM_Init(Mem *ram) {
	assert(ram != nullptr);

	char *tryCalloc = (char *)calloc(8196, sizeof(char));
	if (tryCalloc == nullptr) {
		return EXECUTE_ERROR::ramNullptr;
	}
	
    ram->data = tryCalloc;
    ram->index = 0;

    return EXECUTE_ERROR::noErr;
}

EXECUTE_ERROR stackInit(Stack *stack) {
	assert(stack != nullptr);

    if (stack_ctor(stack, 1) != 0) {
    	return EXECUTE_ERROR::stackNullptr;
    }
    return EXECUTE_ERROR::noErr;
}

static EXECUTE_ERROR CreateInName(char *filePtr, char *exeName) {
	assert(filePtr != nullptr);
	assert(exeName != nullptr);

	if (filePtr == nullptr || exeName == nullptr) {
		return EXECUTE_ERROR::argErr;
	}

	char Prf[32] = "../PROGS/";
	strcpy(filePtr, Prf);

	strcat(filePtr, exeName);
	strcat(filePtr, _INP);

	return EXECUTE_ERROR::noErr;
}

EXECUTE_ERROR CP_Compile_Txt(CPU_32 *proc, char *exeName, ASM_DATA *asmData) {
	assert(proc != nullptr);
	assert(exeName != nullptr);
	
	char exeTxtPtr[32] = "";
	if (CreateInName(exeTxtPtr, exeName) != EXECUTE_ERROR::noErr) {
		proc->state = cpuState::BROKEN;
		return EXECUTE_ERROR::argErr;
	}
	
	proc->state = cpuState::ASSEMBLING;
	if (DSL_FILE_compile(exeTxtPtr, asmData) != COMPILE_ERROR::noErr) {
		proc->state = cpuState::BROKEN;
		return EXECUTE_ERROR::badSntx;
	}

	return EXECUTE_ERROR::noErr;
}

// [ #### PREPARE PROCESS #################################### ]
EXECUTE_ERROR Execute(FILE *bexe, CPU_32 *proc, size_t fsize) {
	assert(bexe != nullptr);
	assert(proc != nullptr);

	char bytesFromBinary[1000];
    fread(&bytesFromBinary, sizeof(int), fsize, bexe);
    fseek(bexe, 0l, SEEK_SET);

	stack_ctor(&proc->stack, 1);

	proc->state = cpuState::EXECUTE;
	printf("\nexecutin::\n");
	printf("______________________________________________________________________________________________\n");
	printf("Version %d\t\t |\t\t      x-Val,    cmd    optional info\t    stack size\n", VERSION );
	printf("______________________________________________________________________________________________\n\n");
	int flag = 0;
	int reg = 0;
	int adres = 0;
	int constant = 0;
	int prt = 0;

	for(;prt < fsize - 2;) {
    	int ip = prt + 1;
    	char op_code = 0;
    	op_code = (*(( int *)bytesFromBinary + ip) & 255);

		printf("EXCTIN: SCR/CPU/PROC.CPP .................");
    	printf(" %10x\t", op_code);
		int curCodeVal = *((int *)bytesFromBinary + ip);
		float nextCodeVal = *((float *)bytesFromBinary + ip + 1);
		float registernum = *((int *)bytesFromBinary + ip + 1);

		#define CMD(name, num, code, ...)								\
            if ( (curCodeVal & CMDMASK) == num ) {						\
            	code 													\
            }

        #include "../CMD_DEFINITION.h"
		printf("\t\tsz: %d", proc->stack.size);
		printf("\n");
		prt += 1;
	}
	return EXECUTE_ERROR::noErr;
}
