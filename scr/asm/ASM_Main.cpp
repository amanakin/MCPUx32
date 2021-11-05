#include <stdio.h>
#include "asm.h"

int main() {

	ASM_DATA asmData = {};
	char name[] = "../PROGS/a1.txt";

	DSL_FILE_compile(name, &asmData);

	return 0;
}
