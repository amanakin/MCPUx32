#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <assert.h>

#include "../cpu/M_CPUx32.h"
#include "stack.h"
#include "string.h"

StackError stack_ctor(Stack *stack, size_t start_capacity) {
    if (stack == nullptr) {
        printf("fatal error: BAD STACK PTR, please check entered parametres of function: %s\n", __func__);
        stack = nullptr;

        return StackError::structPtrErr;
    }

    stack->size = 0;
    stack->capacity = start_capacity;

    stack->data = (sType *)calloc(stack->capacity, sizeof(sType));

    if (stack->data == nullptr) {
        
        return StackError::structPtrErr;
    }

     return StackError::noErr;
}

StackError stack_push (Stack *stack, sType new_elem) {
    if (stack == nullptr){
        printf("fatal error: BAD STACK PTR, please check entered parametres of function: %s\n", __func__);

        return StackError::structPtrErr;
    }

    if(stack->capacity < 0) {
        return StackError::badCap;
    }

    if(stack->size < 0) {
        return StackError::badSz;
    }

    if (stack->size >= stack->capacity) {
        stack_resize(stack, stack->capacity * REALLOC_COEFF);
    }

    stack->data[stack->size] = new_elem;
    stack->size++;

    return StackError::noErr;
}

StackError stack_resize(Stack *stack, size_t increase_by) {
    if (stack == nullptr) {
        printf("fatal error: BAD STACK PTR, please check entered parametres of function: %s\n", __func__);

        return structPtrErr;
    }

    if(stack->capacity < 0) {
        return StackError::badCap;
    }

    if(stack->size < 0) {
        return StackError::badSz;
    }

    stack->capacity = increase_by;
    sType *temporary_data = (sType *) realloc(stack->data, increase_by * sizeof(sType));

    if (stack->data == nullptr) {
        return StackError::dataErr;
    }

    stack->data = temporary_data;

    return StackError::noErr;
}

StackError stack_top (Stack *stack, sType *top_element) {
    if (stack == nullptr) {
        printf("fatal error: BAD STACK PTR, please check entered parametres of function: %s\n", __func__);

        return StackError::structPtrErr;
    }

    if(stack->capacity < 0) {
        return StackError::badCap;
    }

    if(stack->size < 0) {
        return StackError::badSz;
    }

    if (stack->data == nullptr) {
        return StackError::dataErr;
    }

    *top_element = stack->data[stack->size - 1];

    return StackError::noErr;
}

StackError stack_pop (Stack *stack) {
    if (stack == nullptr) {
        return StackError::structPtrErr;

        printf("fatal error: BAD STACK PTR, please check entered parametres of function: %s\n", __func__);
    }

    if(stack->capacity < 0) {
        return StackError::badCap;
    }

    if(stack->size < 0) {
        return StackError::badSz;
    }

    stack->size--;

    if (stack->size <= (stack->capacity / REALLOC_COEFF / REALLOC_COEFF) ) {
        if (stack->size != 0) {
            stack_resize(stack, stack->capacity / REALLOC_COEFF);
        }
        if (stack->size < 0) {
            printf("WRONG STACK SIZE");
        }
    }

    return StackError::noErr;
}

StackError stack_dtor(Stack *stack) {
    if (stack == nullptr) {
        printf("fatal error: BAD STACK PTR, please check entered parametres of function: %s\n", __func__);

        return structPtrErr;
    }

    if(stack->size < 0) {
        return StackError::badSz;
    }

    if(stack->capacity < 0) {
        return StackError::badCap;
    }

    for (int index = 0; index < stack->capacity - 1; index++) {
        stack->data[index] = 0;
    }

    stack->size     = 0;
    stack->capacity = 0;

    free(stack->data);

    return StackError::noErr;
}
