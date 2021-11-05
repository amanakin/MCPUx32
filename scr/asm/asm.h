#ifndef ASM_H
#define ASM_H

#include "stdio.h"
#pragma pack(push, 1) 

// [ ##### REGS ################################# ]
#define REG(name, num)   \
    name = num,

enum class regs {
    #include "../REG_DEFINITION.h"
};
#undef REG

/**
 * @brief We check which arg follow to first word
 * 
 * @param flags :: show which type of info follow to f. word
 */
typedef struct{
    unsigned char wArgVal:5;
    unsigned char wcnst  :1;
    unsigned char wReg   :1;
    unsigned char wLab   :1;
} CMD_FLAGS;

/**
 * @brief DSL COMPILE LANGUAGE ANALIS SYNTAX
 * we compile text program to byte code and check
 * syntax of this file, error of language are
 * contains in this enum 
 */
enum class COMPILE_ERROR : char {
    noErr = 0,
    // [ ##### ___FUNC___ ##### ]
    argERR,
    codeNullptr,
    labelNullptr,
    regsNullptr,
    IofError,
    OufError,
    // [ ### SYNTAXIS ERROR ### ]
    MuchArgs,
    FewArgs,
    undefCmd,
    undefLabel,
    longStr,
    sntxErr
};

/**
 * @brief Contains info how we should assembly word
 */
enum bytte {
    ASCMD = 0,
    ASREG,
    ASCONST,
    ASLABEL
};


// [ ##### STRUCTS ################################# ]
/**
 * @brief Label strucrure
 */
struct label {
    int ip = 0;
    char name[100]  = {};
};

/**
 * @brief Register structure
 */
struct registers {
    char name[3]             = "";
    int ptr                 = {};
};

struct ASM_DATA {
    int *code                  = nullptr;
    size_t code_index           = 0;

    int byte_ip                 = 0;
    int pass_num                = 0;
    size_t nLines               = 1;
    label *labels               = nullptr;
    size_t labels_count         = 0;
    int *regs                   = nullptr;
    size_t cur_reg              = (int)regs::noReg;
    FILE *EXE_                  = nullptr;
    FILE *exe_txt               = nullptr;
};

/**
 * @brief LINE structure
 */
struct LINE {
    char p_beg[32];
};

/**
 * @brief Text structure
 */
struct text {
    LINE *lines = nullptr;
};


// [ ################ __ASM WORK___  ######################### ]

/**
 * @brief COMPILE TEXT FILE TO BYTE CODE
 * 
 * @param exe_txt :: string ptr to [...].txt
 * @param asmData     :: asmData structure
 * 
 * @return COMPILE_ERROR result
 */
COMPILE_ERROR DSL_FILE_compile (char *exe_txt, ASM_DATA *asmData);

/**
 * @brief COMPILE TEXT FILE TO BYTE CODE
 * 
 * @param exe_txt :: string ptr to [...].txt
 * @param asmData     :: asmData structure
 * 
 * @return COMPILE_ERROR result
 */
COMPILE_ERROR asmDataCtor (ASM_DATA *asmData, const char* exe_txt);

// [ ################ WORK WITH TEXT ######################### ]

/**
 * @brief solve is word is const
 * 
 * @param word :: currrent word in line
 * @return 1 if word is const
 * @return 0 if word isn't const
 */
int IS_Const (char *word);

/**
 * @brief solve is word is Register
 * 
 * @param word :: currrent word in line
 * 
 * @return 1 if word is Register
 * @return 0 if word isn't Register
 */
int IS_Reg (char *word);

/**
 * @brief solve is word is Label
 * 
 * @param word :: currrent word in line
 * @return 1 if word is Label
 * @return 0 if word isn't Label
 */
int IS_Label (char *word);

/**
 * @brief solve is word is Label
 * 
 * @param word :: currrent word in line
 * @return 1 if word is Label
 * @return 0 if word isn't Label
 */
int IS_Print (char *Line);

/**
 * @brief Read text in special structure. it contains str begins
 * 
 * @param program_txt :: currrent word in line
 * @param asmData     :: asmData structure
 * 
 * @return COMPILE_ERROR result
 */
COMPILE_ERROR text_read (text *program_txt, ASM_DATA *asmData);

/**
 * @brief placed strings begin ptrs to structure
 * 
 * @param program_txt :: currrent word in line
 * @param txt         :: readen text
 * 
 * @return COMPILE_ERROR result
 */
COMPILE_ERROR GET_Lines_Begin (text *program_txt, FILE *txt);

// [ ################ ___ASM LINE___ ######################### ]

/**
 * @brief assembling lines in readen text
 * 
 * @param asmData :: structure of assembler
 * @param line    :: current line in text
 * @param ln      :: number of strings
 * 
 * @return COMPILE_ERROR result
 */
COMPILE_ERROR assembly_line (ASM_DATA *asmData, char *line, int ln);

#endif//ASM_H
