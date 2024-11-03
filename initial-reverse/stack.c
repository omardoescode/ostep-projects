#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *DUMMYVALUE = "\0";

typedef struct node node;
struct node {
  char *element;
  node *next;
};

node *nd_init(char *val, node *next) {
  node *nd = (node *)malloc(sizeof(node));
  if (!nd) {
    fprintf(stderr, "malloc failed");
    exit(1);
  }
  nd->element = val;
  nd->next = next;
  return nd;
}
void nd_free(node *nd) {
  if (!nd)
    return;
  nd_free(nd->next);
  free(nd->element);
  free(nd);
}

////////////// STACK

struct stack {
  node *head;
  size_t size;
  size_t str_size;
};

stack *st_init() {
  stack *st = (stack *)malloc(sizeof(stack));
  if (!st) {
    fprintf(stderr, "malloc failed");
    exit(1);
  }
  st->head = nd_init(DUMMYVALUE, NULL);
  st->size = st->str_size = 0;
  return st;
}

bool st_empty(stack *st) { return (st->head->next == 0); }

char *st_top(stack *st) {
  if (st_empty(st))
    return DUMMYVALUE;

  return st->head->next->element;
}

size_t st_size(stack *st) { return st->size; }
size_t st_str_size(stack *st) { return st->str_size; }

void st_push(stack *st, char *val) {
  node *n = nd_init(val, st->head->next);
  st->head->next = n;
  st->size++;
  st->str_size += strlen(val);
}

int st_pop(stack *st) {
  if (st_empty(st))
    return 1;
  st->size--;
  node *temp = st->head->next;
  st->str_size -= strlen(temp->element);
  st->head->next = st->head->next->next;
  free(temp);
  return 0;
}

void st_free(stack *st) {
  if (st_empty(st))
    return;
  nd_free(st->head);
  free(st);
}
