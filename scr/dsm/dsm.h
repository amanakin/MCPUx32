#ifndef DSM_H
#define DSM_H

#include "../cpu/M_CPUx32.h"

enum class REtranslateError {
	noErr = 0,

	undefSign,
	undefVal
};

REtranslateError CvrtTxt(FILE *bin, int pkSize);
size_t getNbytes(FILE *in_bin);

#endif//DSM_H
