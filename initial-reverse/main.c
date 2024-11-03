#include "stack.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define TEMP_DIR "reverse_temp"
#define TEMP_FILE_SIZE 1024 * 10
#define TEMP_PATH TEMP_DIR "/tmp%d.txt"
#define TEMP_FILENAME_SIZE 34 // reverse_temp/txt8.txt

FILE *inp, *out, *tmp;
int tmp_count = -1;
struct stat st = {0};

void delete_temps();

void cleanup() {
  if (inp)
    fclose(inp);
  if (out && out != stdout)
    fclose(out);
  // Delete all the temp files
  delete_temps();
}

void cmp_files() {
  char ch1, ch2;
  bool same = true;
  do {
    ch1 = fgetc(inp);
    ch2 = fgetc(out);

    if (ch1 != ch2)
      same = false;
  } while (same && (ch1 != EOF || ch1 != EOF));

  if (same && ch1 == EOF && ch2 == EOF) {
    fprintf(stderr, "reverse: input and output file must differ\n");
    exit(1);
  }

  fseek(inp, 0, SEEK_SET);
  fseek(out, 0, SEEK_SET);
}

char *get_next_tmp_filename(bool next) {
  char *name = malloc(TEMP_FILENAME_SIZE);
  if (!name) {
    fprintf(stderr, "Allocation failed");
    exit(1);
  }
  if (next)
    tmp_count++;
  name[TEMP_FILENAME_SIZE - 1] = '\0';
  snprintf(name, TEMP_FILENAME_SIZE, TEMP_PATH, tmp_count);
  if (!next)
    tmp_count--;

  /* printf("Writing to %s\n", name); */
  return name;
}

void store_tmp(stack *s) {
  char *name = get_next_tmp_filename(1);
  tmp = fopen(name, "w");

  while (!st_empty(s)) {
    fprintf(tmp, "%s", st_top(s));
    st_pop(s);
  }

  fclose(tmp);
  free(name);
}

void group_temps() {
  while (tmp_count != -1) {
    // Open the first tmp
    char *name = get_next_tmp_filename(0);
    tmp = fopen(name, "r");

    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), tmp)) > 0) {
      fwrite(buffer, 1, bytes, out);
    }

    fclose(tmp);
  }
}

void delete_temps() {
  struct dirent *entry;
  DIR *dp = opendir(TEMP_DIR);

  if (!dp) {
    return;
  }

  while ((entry = readdir(dp)) != NULL) {
    // Skip the special entries "." and ".."
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    // Construct the full path
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", TEMP_DIR, entry->d_name);

    // If entry is a file, delete it
    if (remove(full_path) != 0) {
      perror("remove");
    }
  }

  closedir(dp);

  // Finally, remove the directory itself
  if (rmdir(TEMP_DIR) != 0) {
    perror("rmdir");
  }
}
void reverse() {
  stack *s = st_init();
  char *buf = NULL;
  size_t bufsize = 0;

  // Create a directory if there are none
  if (stat(TEMP_DIR, &st) == -1) {
    mkdir(TEMP_DIR, 0700);
  }

  bool dirty = false;
  while (getline(&buf, &bufsize, inp) != -1) {
    st_push(s, buf);
    buf = NULL;
    dirty = true;

    // If the size string made it to some length, store it in the next tmp file
    if (st_str_size(s) >= TEMP_FILE_SIZE) {
      store_tmp(s);
    }
  }

  // Store the remaining data
  if (!st_empty(s))
    store_tmp(s);

  /* printf("Made it here\n"); */
  // Print to output file
  group_temps();
}

int main(int argc, char **argv) {
  atexit(cleanup);
  if (argc > 3) {
    fprintf(stderr, "usage: reverse <input> <output>\n");
    exit(1);
  }

  if (argc == 3 && strcmp(argv[1], argv[2]) == 0) {
    fprintf(stderr, "reverse: input and output file must differ\n");
    exit(1);
  }

  inp = argc >= 2 ? fopen(argv[1], "r") : stdin;
  out = argc == 3 ? fopen(argv[2], "w") : stdout;

  if (!inp) {
    fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
    exit(1);
  }

  if (!out) {
    fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
    exit(1);
  }

  cmp_files();
  reverse();
}
