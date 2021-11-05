#### MCPUx32 - 4th project within the framework of the Dedinsky course
 
<sub>the project works in three stages:
first, the assembler converts the text of a program written in a special DSL language and encodes each word of the program with a bytecode, which is specified in DEF_COMMANDS.sh. Comments, (db command) are encoded character by character and if the string is not multiple of four, for example, 3 characters, then the size is rounded up to a multiple of four and written to the output file. <sub> 

then the processor gets down to business, which reads bytes of 4 pieces and executes the code that is contained in the binary file. The processor implements jumps commands (all different conditions a _ b, where "_" is any sign from \[==, <,>, <=,> =\] the processor stores values in the stack and in RAM, to which it can contact at any time of program execution.

my first project which contains over 1000 lines of code, this project used multiple folders scr/: {[cpu](https://github.com/MoonXCode/MCPUx32/tree/main/scr/cpu), [stack](https://github.com/MoonXCode/MCPUx32/tree/main/scr/stack), [asm](https://github.com/MoonXCode/MCPUx32/tree/main/scr/asm), [dsm](https://github.com/MoonXCode/MCPUx32/tree/main/scr/dsm), [progs](https://github.com/MoonXCode/MCPUx32/tree/main/scr/PROGS)}; [log](https://github.com/MoonXCode/MCPUx32/tree/main/log).
 
  <sub>The processor works in the following order `algorithm.txt` > `asm` > `exe.bin` > `proc` > `result` <sub>

  <sub>Also we can convert `exe.bin` > `dsm` > `table.txt` which will contain the type and purpose of each byte of the code <sub>

* [cpu](https://github.com/MoonXCode/MCPUx32/tree/main/scr/cpu) the file contains the processor core and functions for executing byte code that is generated using the library asm.h.
* [stack](https://github.com/MoonXCode/MCPUx32/tree/main/scr/stack)
* [asm](https://github.com/MoonXCode/MCPUx32/tree/main/scr/asm)
* [dsm](https://github.com/MoonXCode/MCPUx32/tree/main/scr/dsm) 
* [progs](https://github.com/MoonXCode/MCPUx32/tree/main/scr/PROGS)
 
