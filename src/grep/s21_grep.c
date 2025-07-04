#include "s21_grep.h"

int main(int argc, char *argv[]) {
  flags_sample flags = {0};
  flags.err_pointer = 1;
  flags_parser(argc, argv, &flags);
  if (flags.err_pointer == 1) {
    while (optind < argc) {
      read_file(argv, &flags);
      optind++;
    }
  }
  return 0;
}

void flags_parser(int argc, char *argv[], flags_sample *flags) {
  int catcher = 0;
  const char *short_options = "e:ivclnhsf:o";

  while ((catcher = getopt_long(argc, argv, short_options, NULL, NULL)) != -1) {
    switch_flags(catcher, flags);
  }
  if ((flags->v || flags->c || flags->l) && flags->o) flags->o = 0;
  if (!flags->e && !flags->f) {
    if (optind < argc) {
      strcat(flags->arguments, argv[optind]);
    }
    optind++;
  }

  if (flags->e || flags->f) {
    flags->arguments[strlen(flags->arguments) - 1] = '\0';
  }
  if (argc - optind > 1) flags->many_files_flag = 1;
}

void switch_flags(int catcher, flags_sample *flags) {
  switch (catcher) {
    case 'e':
      flags->e = 1;
      strcat(flags->arguments, optarg);
      strcat(flags->arguments, "|");
      break;
    case 'i':
      flags->i = 1;
      break;
    case 'v':
      flags->v = 1;
      break;
    case 'c':
      flags->c = 1;
      break;
    case 'l':
      flags->l = 1;
      break;
    case 'n':
      flags->n = 1;
      break;
    case 'h':
      flags->h = 1;
      break;
    case 's':
      flags->s = 1;
      break;
    case 'f':
      flags->f = 1;
      strcpy(flags->file_path, optarg);
      flag_f_proc(flags);
      break;
    case 'o':
      flags->o = 1;
      break;
    default:
      flags->err_pointer = 0;
      break;
  }
}

void read_file(char *argv[], flags_sample *flags) {
  FILE *file;
  regex_t reg;
  regmatch_t o_struct;
  int cflag = REG_EXTENDED;
  if (flags->i) cflag = REG_EXTENDED | REG_ICASE;
  regcomp(&reg, flags->arguments, cflag);

  file = fopen(argv[optind], "r");
  if (file != NULL) {
    int match_counter = 0;
    int str_counter = 0;

    while (fgets(flags->str, MAX_LEN, file) != NULL) {
      int match = regexec(&reg, flags->str, 1, &o_struct, 0);
      str_counter++;
      if (!match) match_counter++;
      if (flags->v) match = !match;
      file_name_print(argv, match, flags);
      classic_grep_print(match, flags, str_counter);

      if (!match && flags->o) flag_o_print(flags, reg, o_struct, str_counter);
    }
    if (flags->c) flag_c_print(argv, flags, &match_counter, str_counter);
    if (flags->l) flag_l_print(argv, flags, match_counter);
    fclose(file);
  } else {
    if (!flags->s) printf("error file\n");
  }
  regfree(&reg);
}

void file_name_print(char *argv[], int match, flags_sample *flags) {
  if ((!match) && flags->many_files_flag && !flags->l && !flags->h &&
      !flags->c) {
    printf("%s:", argv[optind]);
  }
}

void classic_grep_print(int match, flags_sample *flags, int str_counter) {
  if (!match && !flags->l && !flags->c && !flags->o) {
    if (flags->n) printf("%d:", str_counter);
    printf("%s", flags->str);
    if (flags->str[strlen(flags->str) - 1] != '\n') printf("\n");
  }
}

void flag_o_print(flags_sample *flags, regex_t reg, regmatch_t o_struct,
                  int str_counter) {
  if (flags->n) printf("%d:", str_counter);
  char *pointer = flags->str;
  while ((regexec(&reg, pointer, 1, &o_struct, 0) == 0)) {
    printf("%.*s\n", (int)(o_struct.rm_eo - o_struct.rm_so),
           pointer + o_struct.rm_so);
    pointer += o_struct.rm_eo;
  }
}

void flag_c_print(char *argv[], flags_sample *flags, int *match_counter,
                  int str_counter) {
  if (flags->many_files_flag && !flags->h) printf("%s:", argv[optind]);
  if (!flags->l && !flags->v)
    printf("%d\n", *match_counter);
  else if (!flags->l && flags->v)
    printf("%d\n", str_counter - *match_counter);
  if (flags->l) {
    if (!flags->v) {
      if (*match_counter > 0) {
        *match_counter = 1;
        printf("%d\n%s\n", *match_counter, argv[optind]);
      } else
        printf("%d\n", *match_counter);
    } else {
      *match_counter = 1;
      printf("%d\n%s\n", *match_counter, argv[optind]);
    }
  }
}

void flag_l_print(char *argv[], flags_sample *flags, int match_counter) {
  if ((match_counter == 0 && flags->v) || (match_counter > 0 && !flags->c))
    printf("%s\n", argv[optind]);
}

void flag_f_proc(flags_sample *flags) {
  FILE *arg_file;
  arg_file = fopen(flags->file_path, "r");
  if (arg_file != NULL) {
    while (!feof(arg_file)) {
      if (fgets(flags->str, MAX_LEN, arg_file) != NULL) {
        if (flags->str[strlen(flags->str) - 1] == '\n' &&
            strlen(flags->str) != 1) {
          flags->str[strlen(flags->str) - 1] = '\0';
        }
        strcat(flags->arguments, flags->str);
        strcat(flags->arguments, "|");
      }
    }
    fclose(arg_file);
  }
}