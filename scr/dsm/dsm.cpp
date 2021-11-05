#include "dsm.h"

size_t getNbytes(FILE *in_bin) {

	fseek(in_bin, 0l, SEEK_END);
	size_t sizet = ftell(in_bin);
	fseek(in_bin, 0l, SEEK_SET);
	sizet /= 4;

	sizet += 2;
	return sizet;
}

REtranslateError CvrtTxt(FILE *bin, int pkSize) {

	printf("\nCvrtTxt:\n\n");

	char bytesFromBinary[1000];
    fread(&bytesFromBinary, sizeof(char), 1000, bin);
    fseek(bin, 0l, SEEK_SET);

    printf("first: %x\n", (unsigned char)bytesFromBinary[0]);

    if (*((unsigned int *)bytesFromBinary) != SIGNATURE) {
    	printf("  SIGNATURE UN_DEFINED: NEED:[0x%X], WRONG: %x\n", SIGNATURE, *((unsigned int *)bytesFromBinary));
    	printf("                                              ^---\n");
    	return REtranslateError::undefSign;
    }

    printf("  SIGNATURE DEFINED: (0x%X), VERSION %d\n", SIGNATURE, VERSION);
    printf("----------------------------------------------------------------------------------\n");
    printf("| pos\t |\t bin_val\t |\t txt_meaning\t |\t __type__\t |\n");
    printf("|--------------------------------------------------------------------------------|\n");

    char NxtConst = 0;
    char NxtJMP = 0;
    char NxtArgReg = 0;
    char NxtR = 0;
    char stringCharsCounter = 0;

    int nxt_ip = 0;

    for (int k = 0; k < pkSize - 2; k++) {
    	int i = k + 1;
    	char op_code = 0;
    	op_code = (*((unsigned int *)bytesFromBinary + i) & 255);
    	//printf("STEP: %d\t %x\n\n", i, op_code);
    	//printf("\tstep\t%d\n", NxtR);
    	//NxtR = 0;

    	if ( *((unsigned int *)bytesFromBinary + i) == 129) {
    		printf("| [%d]\t |\t 0x%-5X\t |\t %-7s\t |\t %-8s\t |\n", i, *((unsigned int *)bytesFromBinary + i), "label:", "LABEL<");
    		continue;
    	}

    	if (stringCharsCounter > 0) {
    		printf("| [%d]\t |\t 0x%-5X\t |\t %d %-5s\t |\t %-8s\t |\n", i, *((unsigned int *)bytesFromBinary + i), stringCharsCounter, "TxSz_int", "letter");
    		stringCharsCounter--;
    		continue;
    	}


    	if (NxtR == 1) {
    		printf("| [%d]\t |\t 0x%-5X\t |\t %-7s\t |\t %-8s\t |\n", i, *((unsigned int *)bytesFromBinary + i), "x", "REG");
    		NxtR = 0;
    		continue;
    	}

    	if (NxtConst) {
    		printf("| [%d]\t |\t 0x%-5X\t |\t %-7d\t |\t %-8s\t |\n", i, *((unsigned int *)bytesFromBinary + i), *((unsigned int *)bytesFromBinary + i), "CON");
    		NxtConst = 0;
    		continue;
    	}

    	if (NxtJMP == 1) {
    		printf("| [%d]\t |\t 0x%-5d\t |\t %-7d\t |\t %-8s\t |\n", i, *((unsigned int *)bytesFromBinary + i), *((unsigned int *)bytesFromBinary + i), "JMP>");
    		NxtJMP = 0;
    		continue;
    	}

    	if (NxtArgReg == 1) {
    		printf("| [%d]\t |\t 0x%-5d\t |\t %-7d\t |\t %-8s\t |\n", i, *((unsigned int *)bytesFromBinary + i), *((unsigned int *)bytesFromBinary + i), "[reg + val]");
    		NxtArgReg = 0;
    		continue;
    	}

    	// [ ############## DSM PROCESSS ###################################### ]
    	#define CMD(name, val, ...)															                \
	    if(val == (*(( int *)bytesFromBinary + i) & 255) ) {								    \
	    	if (val == 0b00010111) {														                  \
	    		stringCharsCounter = (*(( int *)bytesFromBinary + i) >> 8);					\
	    		printf("| [%d]\t |\t 0x%-5X\t |\t %-7s\t |\t %-8s\t |\n",					  \
	    			i, *((unsigned int *)bytesFromBinary + i), "message", "db");			\
	    		continue;																	                          \
	    	}																				                              \
	    	if ( (*((unsigned int *)bytesFromBinary + i) & 14336 ) != 0 ) {				\
	        	NxtArgReg = 1;																                    \
	        	continue;																	                        \
	        }																				                            \
	        if ( (*((unsigned int *)bytesFromBinary + i) & (1 << 10) ) != 0) {  \
	        	NxtR = 1;																	                        \
	        	printf("| [%d]\t |\t 0x%-5x\t |\t %-7s\t |\t %-8s\t |\n",					\
	        	i, *((unsigned int *)bytesFromBinary + i), #name, "w_REG");				\
	        	continue;																	                        \
	        }																				                            \
	        if ( (*((unsigned int *)bytesFromBinary + i) & (1 << 9) ) != 0) {		\
	        	NxtConst = 1;																                      \
	        	printf("| [%d]\t |\t 0x%-5x\t |\t %-7s\t |\t %-8s\t |\n",					\
	        	i, *((unsigned int *)bytesFromBinary + i), #name, "w_CON");				\
	        	continue;																	                        \
	        }																				                            \
	        if ( (*((unsigned int *)bytesFromBinary + i) & (1 << 8) ) != 0) {		\
	        	NxtJMP = 1;																	                      \
	        	printf("| [%d]\t |\t 0x%-5x\t |\t %-7s\t |\t %-8s\t |\n",					\
	        	i, *((unsigned int *)bytesFromBinary + i), #name, "w_JMP");				\
	        	continue;																	                        \
	        }																				                            \
	        printf("| [%d]\t |\t 0x%-5x\t |\t %-7s\t |\t %-8s\t |\n",						\
	        	i, *((unsigned int *)bytesFromBinary + i), #name, "CMD");					\
	    }else

    	#include "../CMD_DEFINITION.h"
    	{
	    	printf("| [%d]\tAnkn %x\n", i, *((unsigned int *)bytesFromBinary + i));
		}
    }
    printf("----------------------------------------------------------------------------------\n");
	return REtranslateError::noErr;
}
