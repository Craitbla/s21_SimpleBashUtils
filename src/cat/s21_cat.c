#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct flg {
  int b, e, n, s, t, v;
} flags_sample;

int flags_parser(int argc, char *argv[], flags_sample *flags);
int read_file(char *argv[], flags_sample *flags);

int main(int argc, char *argv[]) {
  flags_sample flags = {0};
  int err_pointer = 1;
  err_pointer = flags_parser(argc, argv, &flags);
  if (flags.b == 1) {
    flags.n = 0;
  }
  if (err_pointer == 1) {
    while (optind < argc) {
      err_pointer = read_file(argv, &flags);
      optind++;
    }
  }
  if (err_pointer == 0) {
    printf("error");
  }
  return err_pointer;
}

int flags_parser(int argc, char *argv[], flags_sample *flags) {
  int catcher;
  int err_pointer = 1;
  static struct option long_options[] = {
      {"number-nonblank", 0, 0, 'b'},
      {"number", 0, 0, 'n'},
      {"squeeze-blank", 0, 0, 's'},
      {0, 0, 0, 0},
  };
  const char *short_options = "+benstvET";

  while ((catcher = getopt_long(argc, argv, short_options, long_options,
                                NULL)) != -1) {
    switch (catcher) {
      case 'b':
        flags->b = 1;
        break;
      case 'e':
        flags->e = 1;
        flags->v = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 't':
        flags->t = 1;
        flags->v = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'E':
        flags->e = 1;
        break;
      case 'T':
        flags->t = 1;
        break;
      default:
        err_pointer = 0;
        break;
    }
  }
  return err_pointer;
}

int read_file(char *argv[], flags_sample *flags) {
  FILE *file;
  int err_pointer = 1;

  file = fopen(argv[optind], "r");

  if (file != NULL) {
    int str_counter = 1;
    int empty_str_counter = 0;
    int last_char = '\n';
    int cur_char;
    while ((cur_char = fgetc(file)) != EOF) {
      if (flags->s && cur_char == '\n' && last_char == '\n') {
        empty_str_counter++;
        if (empty_str_counter > 1) {
          continue;
        }
      } else {
        empty_str_counter = 0;
      }
      if (last_char == '\n' && ((flags->b && cur_char != '\n') || flags->n)) {
        printf("%6d\t", str_counter++);
      }
      if (flags->t && cur_char == '\t') {
        printf("^");
        cur_char = 'I';
      }
      if (flags->e && cur_char == '\n') {
        printf("$");
      }
      if (flags->v) {
        if (cur_char > 127 && cur_char < 160) {
          printf("M-^");
        }
        if ((cur_char < 32 && cur_char != '\n' && cur_char != '\t') ||
            cur_char == 127) {
          printf("^");
        }
        if ((cur_char < 32 || (cur_char > 126 && cur_char < 160)) &&
            cur_char != '\n' && cur_char != '\t') {
          cur_char = cur_char > 126 ? cur_char - 64 : cur_char + 64;
        }
      }

      printf("%c", cur_char);
      last_char = cur_char;
    }
    fclose(file);
  } else {
    err_pointer = 0;
  }
  return err_pointer;
}
