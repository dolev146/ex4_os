#include "mystack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// https://stackoverflow.com/questions/56606496/how-to-add-a-string-to-linked-list-in-c

node_stack_t *head_stack = NULL;
int size = 0;
// node_stack_t *tail_stack = NULL;

void push(char *str)
{
    node_stack_t *newnode = (node_stack_t *)malloc(sizeof(node_stack_t));
    // step 1. allocate memory to hold word
    newnode->txt = malloc(strlen(str) + 1);
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
    free(temp);
}

char *top()
{
    char *msg = (char *)malloc(strlen(head_stack->txt) + 50);
    strcpy(msg, "OUTPUT:");
    strcat(msg, head_stack->txt);
    printf("%s", msg);
    free(msg);
}

int get_size()
{
    return size;
}
