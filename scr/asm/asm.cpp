#include "string.h"
#include "assert.h"

#include "../cpu/M_CPUx32.h"
#include "../settings.h"
#include "asm.h"

// [ ###### SUBSIDIARY ############################################ ]

static size_t lineEnd(char *line) {
    assert(line != nullptr);

    return (line[0] == '\n' || line[0] == '\0') ? 1 : 0;
}

int IS_Const(char *word) {
    assert(word != nullptr);

    if ( (word[0] == '\0' || word[0] == '\n' || word[0] < '0' || word[0] > '9') && word[0] != '-' ) {
        return 0;
    }

    int num = 0;
    if (sscanf(word, "%d", &num)) {
        return 1;
    }
    return 0;
}

int IS_Reg(char *word) {
    assert(word != nullptr);

    return (word[0] >= 'a' && word[0] < 'a' + 4) ? 1 : 0;
}

int IS_Label(char *curWord) {
    assert(curWord != nullptr);

    char word[32] = "";
    size_t len = 0;
    char cur_ch = curWord[len];

    while (cur_ch != ' ' && cur_ch != '\n' && cur_ch != '\0') {
        word[len] = cur_ch;
        len++;
        cur_ch = curWord[len]; 
    }

    if (word[len - 1] == ':') {
        return 1;
    }

    return 0;
}

int IS_Print(char *cur_line) {
    if (cur_line[0] == '\"') {
        return 1;
    }
    return 0;
}

regs isAdres(char *word2, int *ptrAdj) {
    if (word2[0] != '[') {
        return regs::noReg;
    }
    word2 += 1;
    char regu[2] = "";
    sscanf(word2, "%2s", regu); 
    word2 += 5;
    #define REG(name, val)                      \
        if (strcmp(#name, regu) == 0) {         \
            sscanf(word2, "%d", ptrAdj);        \
            return regs::name;                  \
        }

    #include "../REG_DEFINITION.h"
    #undef REG

    return regs::noReg;
}

static size_t N_Lines(FILE *txt) {
    assert(txt != nullptr);

    size_t n_lines = -1;
    char cur_ch = ' ';

    for (;cur_ch != EOF; cur_ch = getc(txt)) {
        if (cur_ch == '\n') {
            n_lines++;
        }
    }

    fseek(txt, 0l, SEEK_SET);
    return ++n_lines;
}

static void DLS_readline(char *cur_line, char *save_string) {
    assert(cur_line != nullptr);

    cur_line++;
    char *endStrSym = strchr(cur_line, '\"');
    if (endStrSym != NULL) {
        char cur_char = ' ';
        size_t index = 0;
        while (cur_char != '\"') {
            save_string[index] = cur_line[index];
            index++;
            cur_char = cur_line[index];
        }
    }
    return;
}

static void ASM_db_Line(char *printLine, size_t strlen, FILE *EXE_) {
    assert(printLine != nullptr);

    int ost = (4 - (strlen % 4)) % 4;
    printf("  WE ADD %d", ost);

    for (size_t index = 0; index < strlen; index++) {
        char cur_ch = printLine[index];
        fwrite(&cur_ch, sizeof(char), 1, EXE_);
    }
    for (int index = 0; index < ost; index++) {
        char cur_ch = 0;
        fwrite(&cur_ch, sizeof(char), 1, EXE_);
    }
}

COMPILE_ERROR DSL_FILE_compile(char *exeTxtPtr, ASM_DATA *asmData) {
    assert(exeTxtPtr != nullptr);

	FILE* inFile = fopen(exeTxtPtr, "r");

	asmDataCtor(asmData, exeTxtPtr);

    text program_txt = {};

    text_read(&program_txt, asmData);

    fwrite(&SIGNATURE, sizeof(int), 1, asmData->EXE_);

    for (; asmData->pass_num < 2; asmData->pass_num++) {
        asmData->code_index = 0;
        for (int line = 0; line < asmData->nLines; line++) {
            assembly_line(asmData, program_txt.lines[line].p_beg, line + 1);
        }
        printf("\n########################################\n");
    }

    fclose(asmData->EXE_);

	return COMPILE_ERROR::noErr;
}

COMPILE_ERROR asmDataCtor(ASM_DATA *asmData, const char* exe_txt) {
    assert(asmData != nullptr);
    assert(exe_txt != nullptr);

    printf("from ptr: %s\n", exe_txt);

// [ ###### CALLOCS ############################################ ]
    int *tryCode = (int *)calloc(CODE_MX_SZ, sizeof(int));
    if (tryCode == nullptr) {
        return COMPILE_ERROR::codeNullptr;
    }
    asmData->code = tryCode;

    label *tryCallocLabel = (label *)calloc(LABLS_MX_SZ, sizeof(label));
    if (tryCallocLabel == nullptr) {
        return COMPILE_ERROR::labelNullptr;
    }
    asmData->labels = tryCallocLabel;
    asmData->labels_count = 0;

    int *tryRegs = (int *)calloc(NREGS, sizeof(int));
    if (tryRegs == nullptr) {
        return COMPILE_ERROR::regsNullptr;
    }
    asmData->regs = tryRegs;

// [ ##### FILES ############################################ ]
    asmData->exe_txt = fopen(exe_txt, "r");
    asmData->EXE_ = fopen(STDBIN, "wb");

    return COMPILE_ERROR::noErr;
}

COMPILE_ERROR assembly_line(ASM_DATA *asmData, char *line, int ln) {
    assert(asmData != nullptr);
    assert(line != nullptr);

    if (lineEnd(line) == 1) {
        return COMPILE_ERROR::noErr;
    }

    if (asmData->pass_num == 1) {       // WE START TO CHECK FIRST WORD :: [ CMD : LABEL ] 
        printf("[%3d] line:\t %-20s", ln, line);
    }

    char* comSym = strchr(line, '/');   // WE CHECK COMMENTS :: SYM [ / ]
    if(comSym != 0) {
        *comSym = '\0';
    }

    char word[MAX_LINE_LEN] = "";
    char *cur_line = line;
    size_t nChReaden = 0;

    sscanf(cur_line, " %32[A-Za-z0-9:_-]%n", word, &nChReaden);
    if (IS_Label(word)) {       // WE CHECK COMMENTS :: FIRST LABEL IS LABEL [ ? ]
        if (asmData->pass_num == 0) {
            asmData->code_index++;
            if ( (line + nChReaden)[0] == '\0' ) {
                strcpy(asmData->labels[asmData->code_index].name, word);
                asmData->labels[asmData->code_index].name[strlen(word)] = '\0';
                printf("we writen in list: %s on poaition: %d\n", asmData->labels[asmData->code_index].name, asmData->code_index);
                asmData->labels[asmData->code_index].ip = asmData->code_index;
                return COMPILE_ERROR::noErr;
            }
            else {
                printf("after label (\" %s \") should be subscribe nothing\n", word);
                return COMPILE_ERROR::sntxErr; 
            }
        }
        if (asmData->pass_num == 1) {
            asmData->code_index++;

            int aggregate = 129;
            printf("\t\thmph %d", asmData->code_index);
            fwrite(&aggregate, sizeof(int), 1, asmData->EXE_);
            printf("\n");
            return COMPILE_ERROR::noErr;
        }
    }

    CMD_FLAGS CMD_FLAGS = {};               // WE will use flag { val, lab, reg, [reg + val] } if cmd subsribed by args

    char printLine[MAX_LINE_LEN] = "";
    char ifcom = 0;
    int Len_of_printStr = 0;
    cur_line += (nChReaden + 1);            // WE CHECK NEXT WORD :: [ label : reg : val : [reg + val] ] 
    if (IS_Print(cur_line) != 0) {
        ifcom = 1;
        DLS_readline(cur_line, printLine);
        if (strlen(printLine) % 4 != 0) {
            Len_of_printStr += (((strlen(printLine) / 4) + 1) );
        }
        else {
            Len_of_printStr += (strlen(printLine) / 4 );
        }
        asmData->code_index += Len_of_printStr;
    }
    char word2[MAX_LINE_LEN] = "";
    sscanf(cur_line, " %32[^\"]%n", word2, &nChReaden);
    cur_line += (nChReaden + 1);

    int ptrAdj = 0;
    if (IS_Reg(word2)) {
        asmData->code_index++;
        CMD_FLAGS.wReg = 1;
    }
    else if (IS_Const(word2)) {
        asmData->code_index++;
        CMD_FLAGS.wcnst = 1;
    }
    else if (IS_Label(word2)) {
        CMD_FLAGS.wLab = 1;
    }
    else if (isAdres(word2, &ptrAdj) != regs::noReg) {
        asmData->code_index++;
        CMD_FLAGS.wArgVal =  (int)isAdres(word2, &ptrAdj);
    }

// [ ##### COMMANDS ######################################################### ]

    int cmd_value = 0;
    #define CMD(name, val, ...)                                             \
    if(strcmp(#name, word) == 0) {                                          \
        cmd_value = (int)val;                                               \
        cmd_value += Len_of_printStr          << 8;                         \
        cmd_value += (int)CMD_FLAGS.wArgVal   << 11;                        \
        cmd_value += (int)CMD_FLAGS.wReg      << 10;                        \
        cmd_value += (int)CMD_FLAGS.wcnst     << 9;                         \
        cmd_value += (int)CMD_FLAGS.wLab      << 8;                         \
        asmData->code_index++;                                              \
        printf("\tcmd: %10x", cmd_value);                                   \
        if (asmData->pass_num == 1) {                                       \
            fwrite(&cmd_value, sizeof(int), 1, asmData->EXE_);              \
        }                                                                   \
    }else

    #include "../CMD_DEFINITION.h"

    {
        printf("unknown: %s\n", word);
        return COMPILE_ERROR::undefCmd;
    }

    if (ifcom != 0 && asmData->pass_num == 1) {
        ASM_db_Line(printLine, strlen(printLine), asmData->EXE_);
    }

    if (IS_Label(word2)) {
        CMD_FLAGS.wLab = 1;
        if (asmData->pass_num == 1) {
            for (int i = 0; i < LABLS_MX_SZ; i++) {
                if (strcmp(asmData->labels[i].name, word2) == 0) {
                    asmData->code_index++;
                    int jump_ip = asmData->labels[i].ip;
                    fwrite(&jump_ip, sizeof(int), 1, asmData->EXE_);
                    if (lineEnd(cur_line)) {
                        printf("\n");
                        return COMPILE_ERROR::noErr;
                    }
                    else {
                        return COMPILE_ERROR::longStr;
                    }
                }
            }
            return COMPILE_ERROR::undefLabel;
        }
        asmData->code_index++;
    }
    if (IS_Const(word2)) {
        float value = 0;
        sscanf(word2, "%f", &value);
        printf("  value: %f", value);
        if (asmData->pass_num == 1) {
            fwrite(&value, sizeof(float), 1, asmData->EXE_);
        }
    }

    if (IS_Reg(word2)) {
        if (asmData->pass_num == 1) {    
            int regval = 0;

            #define REG(name, val)                              \
            if (strcmp(word2, #name) == 0) {                    \
                regval = val;                                   \
                fwrite(&regval, sizeof(int), 1, asmData->EXE_); \
            }

            #include "../REG_DEFINITION.h"
            #undef REG
        }
    }

    int adresval = 0;
    if (isAdres(word2, &adresval) != regs::noReg) {
        if (asmData->pass_num == 1) {
            //printf("WE WROTE ADJ: %d\n", adresval);
            fwrite(&adresval, sizeof(int), 1, asmData->EXE_);
        }
    }

    printf("\n");
    if (asmData->pass_num == 1) {
    }

    return COMPILE_ERROR::noErr;
}

// [ ############################### WORK WITH TEXT ####################### ]

COMPILE_ERROR GET_Lines_Begin(text *program_txt, FILE *txt) {

    size_t cur_line = 0;
    fgets(program_txt->lines[cur_line].p_beg, 32, txt);
    program_txt->lines[cur_line].p_beg[ strlen(program_txt->lines[cur_line].p_beg) - 1] = '\0';
    cur_line++;
    
    while (!feof(txt)) {
        fgets(program_txt->lines[cur_line].p_beg, 32, txt);
        program_txt->lines[cur_line].p_beg[ strlen(program_txt->lines[cur_line].p_beg) - 1] = '\0';
        cur_line++;
    }
    
    fseek(txt, 0l, SEEK_SET);

    return COMPILE_ERROR::noErr;
}

COMPILE_ERROR text_read(text *program_txt, ASM_DATA *asmData) {
    assert(program_txt != nullptr);
    assert(asmData != nullptr);
    
    asmData->nLines = N_Lines(asmData->exe_txt);

    program_txt->lines = (LINE *)calloc(asmData->nLines, sizeof(LINE));

    GET_Lines_Begin(program_txt, asmData->exe_txt);

    return COMPILE_ERROR::noErr;
}
