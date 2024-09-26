#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 500
void grep(char *searchterm, size_t n, char buffer[n]) {
  if (strstr(buffer, searchterm) != NULL) {
    printf("%s", buffer);

    // Add newline if the buffer doesn't end with one
    if (buffer[strlen(buffer) - 1] != '\n') {
      printf("\n");
    }
  }
}
int main(int argc, char **argv) {
  if (argc < 2) {
    puts("wgrep: searchterm [file ...]");
    return 1;
  }

  char *searchterm = argv[1];
  // If it's empty, do nothing, but still successful
  if (strcmp(searchterm, "") == 0)
    return 0;

  // handle reading from `stdin`
  if (argc == 2) {
    size_t n;
    char *buffer;
    while (EOF != getline(&buffer, &n, stdin)) {
      grep(searchterm, n, buffer);
    }

    free(buffer);

    return 0;
  }

  // Read from files
  for (int i = 2; i < argc; i++) {
    FILE *fp = fopen(argv[i], "r");
    if (fp == NULL) {
      puts("wgrep: cannot open file");
      return 1;
    }

    size_t n;
    char *buffer = NULL;
    while (EOF != getline(&buffer, &n, fp)) {
      grep(searchterm, n, buffer);
    }
    free(buffer);
    fclose(fp);
  }
}
