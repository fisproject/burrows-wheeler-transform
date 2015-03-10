/*
* String search using the Move-To-Front in C
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
int *move_to_front(char *list, int point) {
  int len = strlen(list);
  char tmp;

  tmp = list[point];
  for (int i = point; i > 0; i--) {
    list[i] = list[i - 1];
  }
  list[0] = tmp;

  return 0;
}

/*
* move_to_front_transform
*
*/
const char *move_to_front_transform(char *str, int len) {
  char strList[len];
  memset(strList, 0, sizeof(strList));
  int count = 0;

  printf("-- Create Char List --\n");
  for (int i = 0; i < len; i++) {
    char a = str[i];
    // printf("%d->%c\n", i,a);

    if (i == 0) {
      strList[count] = a;
      count++;
    }
    for (int j = 0; j < i; j++) {
      if (strList[j] == a) break;

      if (j == i - 1) {
        strList[count] = a;
        count++;
        break;
      }
    }
  }

  printf("charList -> [%s]\n", strList);
  int retLen = strlen(strList);

  printf("-- Converting --\n");
  char MTF[MAX_BUF_SIZE];
  memset(MTF, 0, sizeof(MTF));
  count = 0;

  for (int i = 0; i < len; i++) {
    char a = str[i];
    printf("search -> [%c] state -> [%s]\n", a, strList);

    for (int j = 0; j < retLen; j++) {
      if (strList[j] == a) {
        // ascii  a -> 65
        sprintf(MTF, "%s %d", MTF, j);
        move_to_front(strList, j);
        count++;
        break;
      }
    }
  }

  const char *m = MTF;
  return m;
}

/*
* get_options
*
*/
char *get_options(int argc, char *const *argv) {
  int c;
  while ((c = getopt(argc, argv, "t:i:s:r:vh")) != -1) {
    switch (c) {
      case 't':
        printf("{option: '%c', mode: 'Move_to_front', input_file: '%s'}\n", optopt, optarg);
        mode_mtf_file = 1;
        return optarg;

      case 'i':
        printf("{option: '%c', mode: 'Inverse Move_to_front', input_file: '%s'}\n", optopt, optarg);
        mode_imtf_file = 1;
        return optarg;

      case 's':
        printf("{option: '%c', mode: 'Move_to_front', input_stream: '%s'}\n", optopt, optarg);
        mode_mtf_stream = 1;
        return optarg;

      case 'r':
        printf("{option: '%c', mode: 'Inverse Move_to_front', input_stream: '%s'}\n", optopt, optarg);
        mode_imtf_stream = 1;
        return optarg;

      case 'v':
        show_version = 1;
        return "show_version";

      case 'h':
        show_help = 1;
        return "show_help";

      default:
        return NULL;
    }
  }
  return NULL;
}

/*
* Main
*
*/
int main(int argc, char **argv) {
  // Parse options
  char *input = get_options(argc, argv);
  if (input == NULL) {
    printf("[Error] Input file or stream required.\n");
    return 1;
  }

  // Read File
  FILE *fp;
  char s[MAX_FILE_LEN];
  int len;
  const char *ret;

  if (mode_mtf_file || mode_imtf_file) {
    if ((fp = fopen(input, "r")) == NULL) {
      printf("[Error] errno %d\n", errno);
      return errno;
    }
    while (fgets(s, MAX_FILE_LEN, fp) != NULL) {
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
    ret = move_to_front_transform(s, len);
  }

  if (mode_imtf_file) {
    // WIP
    // ret = create_inverse_suffix_array(s,len);
  }

  if (mode_mtf_stream) {
    ret = move_to_front_transform(input, strlen(input));
  }

  if (mode_imtf_stream) {
    // WIP
    // ret = create_inverse_suffix_array(input, strlen(input));
  }

  printf("-- Result --\n%s\n", ret);
  return 0;
}