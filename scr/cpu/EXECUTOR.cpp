#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>

#include "M_CPUx32.h"
#include "../../log/log.h"
#include "../dsm/dsm.h"


int main() {
	LOG_MESS(GOOD, "WE STARTED\n")

	CP_Intro();

	char exeName[] = "";
	scanf("%s", exeName);
	
	CPU_32 proc = {};
	CP_DataCtor(&proc);

	ASM_DATA asmData = {};

	CP_Compile_Txt(&proc, exeName, &asmData);

	FILE *bexe = fopen(STDBIN, "rb");
	size_t fsize = getNbytes(bexe);
	CvrtTxt(bexe, fsize);
	Execute(bexe, &proc, fsize);
	
	
	return 0;
}
