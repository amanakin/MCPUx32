-----------------------------------------------
 MCPUx32 Descriprion [4th project]
-----------------------------------------------

this is my first project with over a thousand lines. I changed the code-style a little again, twice per project, now I am still planning to slightly change the case of the first letters [possibly].

Autor: (Michael Krapchatov) 11.2021                                           I will be glad if you write to me Issue


The project works in three stages

ASM  [https://github.com/MoonXCode/MCPUx32/tree/main/scr/asm]
First, he assembler converts the text of a program written in a special DSL language and encodes each word of the program with a bytecode, which is specified in DEF_COMMANDS.sh. Comments, [db command] are encoded character by character and if the string is not multiple of four, for example, 3 characters, then the size is rounded up to a multiple of four and written to the output file.

CPU  [https://github.com/MoonXCode/MCPUx32/tree/main/scr/cpu]
Then the processor gets down to business, which reads bytes of 4 pieces and executes the code that is contained in the binary file. The processor implements jumps commands (all different conditions a _ b, where "_" is any sign from \[==, <,>, <=,> =\] the processor stores values in the stack and in RAM, to which it can contact at any time of program execution.

DSM  [https://github.com/MoonXCode/MCPUx32/tree/main/scr/dsm]
The disassembler turns a binary file into a table that carries information about the entire program as a whole. From this table, you can understand what the program was written about and what its essence is. In addition, the disassembler can make a program history table, that is, a table of the history of actions.

все значения хранятся либо в стеке [https://github.com/MoonXCode/MCPUx32/tree/main/scr/stack]
либо в оперативной памяти процессора


I want to finish:
 - The logging library, which is located by the link https://github.com/MoonXCode/MCPUx32/tree/main/log This will be an excellent option for displaying information about the current state of the processor or other programs
 - nformation output to an arbitrary text file
