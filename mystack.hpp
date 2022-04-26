#ifndef MYSTACK
#define MYSTACK
#include <pthread.h>
#include <unistd.h>

void *my_malloc(int size);
void *my_free(int size);

struct node_stack
{
    struct node_stack *next;
    char *txt;
};
typedef struct node_stack node_stack_t;

extern pthread_mutex_t mutex;

void push(char *str);
void pop();
char *top();
int get_size();

#endif // MYSTACK