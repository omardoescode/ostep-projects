#include <stdio.h>
#include <stdlib.h>

void print_file(char *file_name) {
  FILE *fp = fopen(file_name, "r");

  if (fp == NULL) {
    puts("wcat: cannot open file");
    exit(1);
  }

  char c;
  while (EOF != (c = getc(fp)))
    putc(c, stdout);
  fclose(fp);
}
int main(int argc, char **argv) {

  for (int i = 1; i < argc; i++) {
    print_file(argv[i]);
  }
}
