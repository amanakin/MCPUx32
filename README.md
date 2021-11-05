#### MCPUx32 - 4th project within the framework of the Dedinsky course<br/>

<sub>the project works in three stages<sub>  
---
asm  
<br/>first,
he assembler converts the text of a program written in a special DSL language and encodes each word of the program with a bytecode, which is specified in DEF_COMMANDS.sh. Comments, (db command) are encoded character by character and if the string is not multiple of four, for example, 3 characters, then the size is rounded up to a multiple of four and written to the output file.<sub><sub><sub>
 ------- 
 <br/>proc  
<sub>then the processor gets down to business, which reads bytes of 4 pieces and executes the code that is contained in the binary file. The processor implements jumps commands (all different conditions a _ b, where "_" is any sign from \[==, <,>, <=,> =\] the processor stores values in the stack and in RAM, to which it can contact at any time of program execution. <sub>
 
