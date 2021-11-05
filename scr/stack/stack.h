#ifndef STACK_2_STACK_H
#define STACK_2_STACK_H

#include <cmath>
#include <cstdint>

#include "../cpu/M_CPUx32.h"

typedef float sType;

///\brief contains main constants in stack
///\param CANARY special value which control content in stack
///\param REALLOC_COEFF corresponds to multiplier of stack increase/decrease
enum CONSTANS{
	CANARY = 0xB12D00,
	REALLOC_COEFF = 2
};

///\brief it is a structure of the stack
///\param canary_beg spacial value that places at the begining structure of stack and save it's content
///\param capacity max number of elements in stack
///\param size current number of elements
///\param data content of the stack
///\param hash special value that control conten of stack's stack
///\param canary_end spacial value that places at the end structure of stack and save it's content
typedef  struct{
    int		capacity; //max size
    int		size; //cur number of element
    sType		*data; //stack path
}Stack;

///\brief create stack
///\param new_stack stack example 
///\param start_capacity start max number of elements in stack
StackError stack_ctor (Stack *new_stack, size_t start_capacity);

///\brief changes capacity of stack 
///\param new_stack stack example 
///\param increase_by in what capacity we change current capacity of stack
StackError stack_resize (Stack *stack, size_t increase_by);

///\brief changes capacity of stack 
///\param new_stack stack example 
///\param increase_by in what capacity we change current capacity of stack
StackError stack_push (Stack *stack, sType value);

///\brief returns last element in stack
///\param new_stack stack example 
StackError stack_top (Stack *stack, sType *top_element);

///\brief delete last element
///\param new_stack stack example 
StackError stack_pop (Stack *stack);

///\brief delete stack example
///\param new_stack stack example 
StackError stack_dtor (Stack *stack);

#endif //STACK_2_STACK_H
