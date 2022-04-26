#include "mystack.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LIMIT 1024

// https://stackoverflow.com/questions/56606496/how-to-add-a-string-to-linked-list-in-c

node_stack_t *head_stack = NULL;
int size = 0;

void *my_malloc(int size)
{
    void *firstEnd = sbrk(0);
    void *currentEnd = sbrk(size);
    return firstEnd;
}

void *my_free(int size)
{
    int decrease = (-1) * size;
    void *firstEnd = sbrk(0);
    void *currentEnd = sbrk(decrease);
    return currentEnd;
}

void push(char *str)
{
    memcpy(str, str + 5, MAX_LIMIT - 5);
    node_stack_t *newnode = (node_stack_t *)my_malloc(sizeof(node_stack_t));
    // step 1. allocate memory to hold word
    newnode->txt = (char *)my_malloc(strlen(str) + 1);
    // step 2. copy the current word
    strcpy(newnode->txt, str);
    newnode->next = NULL;
    if (head_stack == NULL)
    {
        head_stack = newnode;
    }
    else
    {
        newnode->next = head_stack;
        head_stack = newnode;
    }
    size++;
}

void pop()
{
    node_stack_t *temp = head_stack;
    head_stack = head_stack->next;
    size--;
    my_free(sizeof(node_stack_t));
}

char *top()
{
    char *msg = (char *)my_malloc(strlen(head_stack->txt) + 50);
    strcpy(msg, "OUTPUT:");
    strcat(msg, head_stack->txt);
    printf("%s", msg);
    return msg;
}

int get_size()
{
    return size;
}
