#ifndef Stack_h
#define Stack_h

#include "list.h"

typedef List Stack;

#define stack_create() createList()

#define stack_pop(stack) popFront(Stack)

#define stack_push(stack, data) pushFront(stack, data)

#define stack_top(stack) firstList(stack)

#define stack_clean(stack) clenaList(stack)

#endif /* Stack_h */