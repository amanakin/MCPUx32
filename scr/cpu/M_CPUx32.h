#ifndef PROC_H
#define PROC_H

#include "../settings.h"

/**
 * @brief show Proc status
 */
enum cpuState : char {
    OFF = 0,
    ASSEMBLING,
    EXECUTE,
    BROKEN
};

/**
 * @brief errors in stack funcions
 * 
 * @param line ptr to string begin
 * @param strlen number of letters in string
 */
enum StackError : char {
    // [ ########### GENERAL ############ ]
	noErr               = 0,
    // [ ############ STACK ############# ]
    badCap              = 1 << 2,
    badSz               = 1 << 3,
    dataErr             = 1 << 4,
    structPtrErr        = 1 << 5
};

/**
 * @brief errors in proc funcions
 * 
 * @param line ptr to string begin
 * @param strlen number of letters in string
 */
enum class EXECUTE_ERROR : char {
    noErr = 0,
    // [ FUNC ]
    argErr,
    // [ PROC STRUCT ]
    ramNullptr,
    ramOverflow,
    // [ STACK ERRORS ]
    stackNullptr,
    StackOverflow,
    stackEmpty,
    // [ STACK ERRORS ]
    codeNullptr,
    regNullptr,
    badSntx,
    // [ RUNTIME PROCESS ]
    commandsOverflow
};

#include "../stack/stack.h"
#include "../asm/asm.h"

// [ ############## STRUCTS ################ ]

/**
 * @brief memory struct. In CPU_32 we use like a RAM
 * 
 * @param data ptr to proc data
 * @param index index of element in data
 */
struct Mem {
    char *data;
    size_t index;
};

/**
 * @brief we set what follow to funcion in DSL
 * 
 * @param w_... = [ w.ith_ {Adres, Value, Registerб Label} ]
 */
struct{
    unsigned char wArgVal:5;
    unsigned char wVal:1;
    unsigned char wReg:1;
    unsigned char wLab:1;
} cmdFlags;


/**
 * @brief CPU_32 struct
 * 
 * @param state :: status of work CPU_32 DECLARED in M_CPUx32.h as enum
 * @param ram   :: RAM structure DECLARED in M_CPUx32.h as struct
 * @param stack :: stack structure DECLARED in ../stack/stack.h as struct
 * 
 * @param code  :: pointer to executecode
 * @param reg   :: reg struct DECLARED in M_CPUx32.h
 * @param ip    :: executeble code index
 */
struct CPU_32 {
    size_t state            = -1; 
    Mem ram                 = {};
    Stack stack             = {};
    char* code              = nullptr;
    float reg[NREGS]        = {};

    int ip                  = {};
};

// [ ######## DEFINES ################################### ]

#define NEXTIP          prt + 1

#define NOERR           0

#define ARGPTR          argument

#define ARG             (*ARGPTR)

#define PRINTF(arg)     printf("OUT: %d", arg)

#define HLT             return EXECUTE_ERROR::noErr

#define JMP(arg)        prt += arg

#define JMP_ABS(arg)    prt = arg

#define SCANF(argptr)   scanf("%d ", argptr)

#define OUTC(arg)       printf("%c", (char)arg);

#define PAUSE           getchar();

#define SLEEP(arg)      tsleep(arg);


// #### [ SUBORDINARY ################################ ]

/**
 * @brief CPU_Introduce CPU_32 program
 */
void CP_Intro();

/**
 * @brief stop program work in small period of time
 * 
 * @param time value of stop period 
 */
void Tsleep(int time);

/**
 * @brief slow putting chars of string [use tsleep to do it]
 * 
 * @param line ptr to string begin
 * @param strlen number of letters in string
 */
void SlowLine(char *line, size_t strlen);

/**
 * @brief slow erase after string pring
 * 
 * @param strlen number of symbols that we want to dell
 */
void SlowErase(char *line, size_t strlen);

/**
 * @brief calloc RAM and watch if smth went wrong
 * 
 * @param ram RAM of proc
 */
EXECUTE_ERROR RAM_Init(Mem* ram);

/**
 * @brief calloc stackData and watch if smth went wrong
 * 
 * @param ram stack of proc
 */
EXECUTE_ERROR stackInit(Stack* stack);


// [ #### PREPARING ################################## ]

/**
 * @brief load the program for CPU_32 & watch if smt went wrong
 * 
 * @param proc struct of proc
 * @param exe_txt name of program
 * @param ASM_DATA assembler struct
 */
EXECUTE_ERROR CP_Compile_Txt(CPU_32 *proc, char *exe_txt, ASM_DATA *asmData);

/**
 * @brief load the program for CPU_32 & watch if smt went wrong
 * 
 * @param proc struct of proc
 */
EXECUTE_ERROR CP_DataCtor(CPU_32 *proc);


// [ #### EXECUTING ################################## ]

/**
 * @brief load the program for CPU_32 & watch if smt went wrong
 * 
 * @param proc struct of proc
 */
EXECUTE_ERROR Execute(FILE *bexe, CPU_32 *proc, size_t fsize);

#endif//PROC_H
