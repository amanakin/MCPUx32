#include <math.h>
#include "cpu/M_CPUx32.h"

#ifndef CMD
#error  CMD not defined
#endif

// [ ### JUMPS ################################## ]

#define CMD_JMP(name, value, mark)  \
    CMD(name, value, {              \
        prt++;                                  \
        printf("%-5d   ", (int)registernum);          \
        float stackVal1 = 0;                    \
        stack_top(&proc->stack, &stackVal1);    \
        stack_pop(&proc->stack);                \
        float stackVal2 = 0;                    \
        stack_top(&proc->stack, &stackVal2);    \
        stack_push(&proc->stack, stackVal1);    \
        printf("jmpto? %f %s %f", stackVal2, #mark, stackVal1);  \
        if(stackVal2 mark stackVal1){           \
            printf(" yes\n");                   \
            JMP_ABS((int)registernum - 1);           \
        }                                       \
        prt++;  \
        printf(" no\n");                           \
        continue;  \
    })                                          \

CMD_JMP(ja , 0b01110001, > )

CMD_JMP(jae, 0b01110010, >=)

CMD_JMP(jb , 0b01110011, < )

CMD_JMP(jbe, 0b01110100, <=)

CMD_JMP(je,  0b01110101, ==)

CMD_JMP(jne, 0b01110110, !=)

CMD_JMP(jb , 0b01110011, < )

CMD_JMP(ja , 0b01110001, > )


CMD(ret, 0b00011001, {
    float stackVal1 = 0;
    stack_top(&proc->stack, &stackVal1);
    printf("ret\t %d\t\t", (int)stackVal1);
    JMP_ABS(stackVal1 + 1);
})
#undef CMD_JMP

CMD(call, 0b00111000, {
    printf("call    $%d", nextCodeVal - 1);
    stack_push(&proc->stack, (float)prt);
    JMP_ABS(registernum);
    printf("\n");
    continue;
})

// [ ### ARITHM ################################## ]

#define CMD_AR(name, num, arth, zerocheck)      \
    CMD(name, num, {                            \
        printf("%-4s", #name);                  \
        float stackVal1 = 0;                    \
        stack_top(&proc->stack, &stackVal1);    \
        stack_pop(&proc->stack);                \
        float stackVal2 = 0;                    \
        stack_top(&proc->stack, &stackVal2);    \
        {zerocheck};                            \
        stack_pop(&proc->stack);                                    \
        stack_push(&proc->stack, stackVal2 arth stackVal1);         \
        printf("%12f", stackVal2 arth stackVal1);                   \
    })

CMD_AR(mul,    0b00000101, *, ;)

CMD_AR(add,    0b00000011, +, ;)

CMD_AR(sub,    0b00000100, -, ;)

CMD_AR(div,    0b00000110, /, {
    if(stackVal1 == 0){
        printf("DIV_ZERO");
    }})

#undef CMD_AR

CMD(sqrt,   0b00001111, {
    float stackVal1 = 0;
    stack_top(&proc->stack, &stackVal1);
    stack_pop(&proc->stack);
    if(stackVal1 < 0) 
        EXECUTE_ERROR::argErr;
    printf("sqrt\t");
    printf("%f", sqrt(stackVal1));
    stack_push(&proc->stack, sqrt(stackVal1) );
})

CMD(abs,   0b00011100, {
    float stackVal1 = 0;
    stack_top(&proc->stack, &stackVal1);
    stack_pop(&proc->stack);
    printf("abs\t(%d)", stackVal1);
    if (stackVal1 < 0) {
        stackVal1 *= -1;
    }
    stack_push(&proc->stack, stackVal1);
})



CMD(hlt,    0b00000000, {
    printf("hlt ");
    HLT;
})


// [ ### STD CMDS ################################## ]

CMD(push,   0b11100001, {
    printf("push");
    if ( (curCodeVal & (1 << 10)) != 0 ) {
        float regval = proc->reg[(int)registernum - 1];
        printf("\twreg [%1d %lg]", (int)registernum - 1, regval);
        stack_push(&proc->stack, regval);
        prt++;
    }
    if ( (curCodeVal & (1 << 9)) != 0 ) {
        printf("\twcon %lf", nextCodeVal);
        stack_push(&proc->stack, nextCodeVal);
        prt++;
    }
})

CMD(in,    0b00001000, {
    if ( (curCodeVal & (1 << 10)) != 0 ) {
        float regval2 = 0;
        printf("INPUT: << ");
        scanf("%f", &regval2);
        proc->reg[(int)registernum - 1] = (float)regval2;
        prt += 2;
        continue;
    }
})

CMD(db,     0b00010111, {
    printf("Mes: ");
    //printf("%d", (int)curCodeVal >> 8 );

    int *dbval = ((int *)bytesFromBinary + ip + 1);

    for (int xtime = 0; xtime < curCodeVal >> 8; xtime++) {
        char ch1 = *(dbval + xtime) & 0b11111111;
        char ch2 = (*(dbval + xtime) >> 8) & 0b11111111;
        char ch3 = (*(dbval + xtime) >> 16) & 0b11111111;
        char ch4 = (*(dbval + xtime) >> 24) & 0b11111111;
        putchar(ch1);
        putchar(ch2);
        putchar(ch3);
        putchar(ch4);
    }
    prt += (curCodeVal >> 8) + 1;
    printf("\n");
    continue;
})

CMD(pop,    0b11100010, {
    printf("pop ");
    if ( (curCodeVal & (1 << 10)) != 0 ) {
        float popreg = 0;
        stack_top(&proc->stack, &popreg);
        //printf("we got: %f", popreg);
        proc->reg[(int)(registernum - 1)] = (float)popreg;
        printf("\twreg [%d:%3f]", (int)(registernum - 1), (float)popreg);
        stack_pop(&proc->stack);
        prt++;
    }
    else if ( (curCodeVal & 14336 ) != 0 ) {
        int popadr = proc->reg[(curCodeVal >> 11) - 1];
        popadr += nextCodeVal;
        printf("\twadr %6d", popadr);
        float popAdrVal = 0;
        stack_top(&proc->stack, &popAdrVal);
        proc->ram.data[popadr] = popAdrVal;
        prt++;
    }
    else {
        stack_pop(&proc->stack);
        printf("\twn______");
    }
})

CMD(out,   0b00000111, {
    printf("OUTPUT ");
    float outval = 0;
    stack_top(&proc->stack, &outval);
    printf("\t >>  %6f", outval);
    stack_pop(&proc->stack);
})



#undef COM_DEF_ARITHM
