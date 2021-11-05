#ifndef SETTINGS_H
#define SETTINGS_H
#pragma once

#include <stdlib.h>


// [ ########## Hardware ########################### ]
const size_t NREGS                  = 5;

const size_t RAM_SZ                 = 8192;

const size_t MAX_LINE_LEN			= 32;

const size_t MAX_LABEL_LEN          = MAX_LINE_LEN;

const size_t STACK_MAX_SZ           = 1024;

const size_t MAX_PROC_OPERATIONS    = 1000000;


// [ ####### Other constants ####################### ]
const char _OUT[] 				= ".bin";

const char _INP[] 				= ".txt";

//char STD_PROG_NAME[] 			= "../PROGS/a1.txt";

const char STDBIN[] 			= "../PROGS/cpu_program.bin";

const size_t CODE_MX_SZ			= 1024;

const size_t EXPAND_COEF		= 2;

const size_t LABLS_MX_SZ      	= 1024;

const size_t MAX_ARGS        	= 2;

const size_t ASM_PASS		 	= 2;

const size_t VERSION		 	= 3;

const char STR_VERSION[]	 	= "3.0.1";

const unsigned SIGNATURE		= 0x0000DED1;

const unsigned CMDMASK			= 255;


// [ ####### VIDEO ################################ ]
const size_t PIXEL_SZ = 5;

#endif
