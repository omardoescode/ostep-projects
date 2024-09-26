#include <stdio.h>

int main(int argc, char *argv[argc]) {
  if (argc == 1) {
    puts("wunzip: file1 [file2 ...]");
    return 1;
  }

  size_t file_num = 1;
  FILE *fp = fopen(argv[file_num], "r");

  int count;
  char c;
  while (file_num < argc) {
    fp = fopen(argv[file_num], "r");
    while (fread(&count, 4, 1, fp) && fread(&c, 1, 1, fp)) {
      while (count--) {
        putc(c, stdout);
      }
    }

    file_num++;
  }
}
