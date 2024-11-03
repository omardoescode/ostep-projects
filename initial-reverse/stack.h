#include <stdbool.h>
#include <stdlib.h>
typedef struct stack stack;
struct stack;

stack *st_init();
char *st_top(stack *);
bool st_empty(stack *);
void st_push(stack *, char *);
int st_pop(stack *);
void st_free(stack *);
size_t st_size(stack *);
size_t st_str_size(stack *);
