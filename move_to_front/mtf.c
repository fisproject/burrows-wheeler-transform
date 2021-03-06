/*
* Move-to-front algorithm in C
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>

#define VERSION "0.0.1"
#define MAX_FILE_LEN 1024
#define MAX_BUF_SIZE 1024

static int mode_mtf_file;
static int mode_imtf_file;
static int mode_mtf_stream;
static int mode_imtf_stream;
static int show_version;
static int show_help;

/*
* move_to_front
*
*/
int move_to_front(char *str, char c) {
  char *q, *p;
  int shift = 0;

  p = (char *)malloc(strlen(str) + 1);
  strcpy(p, str);
  q = strchr(p, c);  // returns pointer to location of char c in string str
  shift = q - p;  // no of characters from 0 to position of c in str
  strncpy(str + 1, p, shift);
  str[0] = c;
  free(p);

  return shift;
}

/*
* decode
*
*/
void decode(int *pass, int size, char *sym) {
  int index;
  char c;
  char table[] = "abcdefghijklmnopqrstuvwxyz";

  for (int i = 0; i < size; i++) {
    c = table[pass[i]];
    index = move_to_front(table, c);
    if (pass[i] != index) printf("there is an error");
    sym[i] = c;
  }
  sym[size] = '\0';
}

/*
* encode
*
*/
void encode(char *sym, int size, int *pass) {
  char c;
  char table[] = "abcdefghijklmnopqrstuvwxyz";

  for (int i = 0; i < size; i++) {
    c = sym[i];
    pass[i] = move_to_front(table, c);
  }
}

/*
* get_options
*
*/
static int get_options(int argc, char *const *argv) {
  int c;
  while ((c = getopt(argc, argv, "t:i:s:r:vh")) != -1) {
    switch (c) {
      case 't':
        printf("{option: '%c', mode: 'Move_to_front', input_file: '%s'}\n", optopt, optarg);
        mode_mtf_file = 1;
        return 0;

      case 'i':
        printf("{option: '%c', mode: 'Inverse Move_to_front', input_file: '%s'}\n", optopt, optarg);
        mode_imtf_file = 1;
        return 0;

      case 's':
        printf("{option: '%c', mode: 'Move_to_front', input_stream: '%s'}\n", optopt, optarg);
        mode_mtf_stream = 1;
        return 0;

      case 'r':
        printf("{option: '%c', mode: 'Inverse Move_to_front', input_stream: '%s'}\n", optopt, optarg);
        mode_imtf_stream = 1;
        return 0;

      case 'v':
        show_version = 1;
        return 0;

      case 'h':
        show_help = 1;
        return 0;

      default:
        return -1;
    }
  }
  return -2;
}

/*
* Main
*
*/
int main(int argc, char **argv) {
  // Parse options
  if (get_options(argc, argv) != 0) {
    printf("[Error] invalid option.\n");
    return 1;
  }

  // Read File
  FILE *fp;
  char s[MAX_FILE_LEN];
  int len;
  int pass[MAX_BUF_SIZE] = {0};

  if (mode_mtf_file || mode_imtf_file) {
    if ((fp = fopen(optarg, "r")) == NULL) {
      printf("[Error] errno %d\n", errno);
      return errno;
    }
    while (fgets(s, MAX_FILE_LEN, fp) != NULL) {
      s[strlen(s) - 1] = '\0';
      len = strlen(s);
      printf("{input: %s, length: %d}\n", s, len);
    }
    fclose(fp);
  }

  if (show_help) {
    printf("\nOptions:\n");
    printf("  -t :  Move_to_front. Input File.\n");
    printf("  -i :  Inverse Move_to_front. Input File.\n");
    printf("  -s :  Move_to_front. Input Stream.\n");
    printf("  -r :  Inverse Move_to_front. Input Stream.\n");
    printf("  -v :  Show Version.\n");
    printf("  -h :  Show Help.\n");
    return 0;
  }

  if (show_version) {
    printf("\nVersion:\n%s\n", VERSION);
    return 0;
  }

  if (mode_mtf_file) {
    encode(s, len, pass);
    for (int j = 0; j < len; j++) {
      printf("%d ", pass[j]);
    }
    printf("\n");
  }

  if (mode_imtf_file) {
    // TODO
  }

  if (mode_mtf_stream) {
    encode(optarg, strlen(optarg), pass);
    for (int j = 0; j < strlen(optarg); j++) {
      printf("%d ", pass[j]);
    }
    printf("\n");
  }

  if (mode_imtf_stream) {
    // TODO
  }

  return 0;
}
