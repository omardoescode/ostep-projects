#include <stdio.h>

int main(int argc, char *argv[argc]) {
  if (argc == 1) {
    puts("wzip: file1 [file2 ...]");
    return 1;
  }

  FILE *inp;
  char current, next;
  int count = 1;
  size_t file_num = 1;
  inp = fopen(argv[file_num], "r");
  current = getc(inp);
  while (inp) {
    while (EOF != (next = getc(inp))) {
      if (current == next) {
        count++;
        continue;
      }
      fwrite(&count, 4, 1, stdout);
      putc(current, stdout);
      current = next;
      count = 1;
    }
    fclose(inp);
    if (++file_num < argc) {
      inp = fopen(argv[file_num], "r");
    } else
      break;
  }

  fwrite(&count, 4, 1, stdout);
  putc(current, stdout);
  return 0;
}
