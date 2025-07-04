#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_LEN 15000

typedef struct flg {
  int e, i, v, c, l, n, h, s, f, o;
  int many_files_flag;
  char str[MAX_LEN];
  char arguments[MAX_LEN];
  char file_path[MAX_LEN];
  int err_pointer;
} flags_sample;

void flags_parser(int argc, char *argv[], flags_sample *flags);
void switch_flags(int catcher, flags_sample *flags);
void flag_f_proc(flags_sample *flags);
void read_file(char *argv[], flags_sample *flags);
void file_name_print(char *argv[], int match, flags_sample *flags);
void flag_o_print(flags_sample *flags, regex_t reg, regmatch_t start, int str_counter);
void flag_c_print(char *argv[], flags_sample *flags, int *match_counter, int str_counter);
void flag_l_print(char *argv[], flags_sample *flags, int match_counter);
void classic_grep_print(int match, flags_sample *flag, int str_counter);