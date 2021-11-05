#include <stdio.h>

#include "dsm.h"

int main() {
	FILE *in_bin = fopen("../PROGS/cpu_program.bin", "rb");

	size_t fsize = getNbytes(in_bin);
	CvrtTxt(in_bin, fsize);

	return 0;
}
