/*
* String search using the Burrows-Wheeler transform in C
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>

#define VERSION "0.0.1"
#define MAX_LEN 2048
#define MAX_FILE_LEN 8192

static int mode_bwt_file;
static int mode_ibwt_file;
static int mode_bwt_stream;
static int mode_ibwt_stream;
static int show_version;
static int show_help;

typedef struct {
  int num;
  char str[MAX_LEN];
} Pair;

/*
* compare string
*
*/
int compare(const void *x, const void *y) {
  return strcmp(((Pair *)x)->str, ((Pair *)y)->str);
  // return strncmp(((Pair*)x)->str ,((Pair*)y)->str);
}

/*
* create_suffix_array
*
*/
static void create_suffix_array(char *str, int len, char *SA) {
  // add $ -> len+1
  Pair p[len + 1];
  char strs[len + 1];
  sprintf(strs, "%s%s", str, "$");
  char tmp;

  // create_char_pair
  printf("-- Before Sort --\n");
  for (int i = 0; i < len + 1; i++) {
    p[i].num = i;
    const char *ctmp = strs;
    strcpy(p[i].str, ctmp);

    // shift
    tmp = strs[len];
    int j;
    for (j = len; j > 0; j--) {
      strs[j] = strs[j - 1];
    }
    strs[j] = tmp;

    printf("{num: %d, char: '%s'\n", p[i].num, p[i].str);
  }

  printf("-- Quick Sort --\n");
  // quick_sort
  qsort(p, len + 1, sizeof(Pair), compare);

  // create suffix array
  printf("-- After Sort --\n");

  for (int i = 0; i < len + 1; i++) {
    printf("{num: %d, char: '%s'}\n", p[i].num, p[i].str);
    const char *ptr = &p[i].str[len];
    const char *preptr;

    if (i != 0) {
      preptr = &p[i - 1].str[len];
    } else {
      preptr = "";
    }

    // printf("%s:%s\n",ptr, preptr);

    // suffix add
    if (SA == NULL) {
      sprintf(SA, "%c", p[i].str[len]);
    } else {
      sprintf(SA, "%s%c", SA, p[i].str[len]);
    }
  }
}

/*
* TODO : create_inverse_suffix_array
*
*/
static void create_inverse_suffix_array(char *str, int len, char *SA) {
  printf("-- starting --\n");
  Pair p[len];
  char strs[len];
  sprintf(strs, "%s", str);
  char tmp;

  // create_char_pair
  printf("-- Before Sort --\n");
  for (int i = 0; i < len; i++) {
    p[i].num = i;
    char a = strs[i];
    const char *s = &a;
    strcpy(p[i].str, s);

    printf("{num: %d, char: %c}\n", p[i].num, a);
  }

  printf("-- Quick Sort --\n");
  qsort(p, len, sizeof(Pair), compare);

  // create suffix array
  printf("-- After Sort --\n");
  int index, first = 0;

  // get index
  for (int i = 0; i < len; i++) {
    char a = p[i].str[0];
    if (a == '$') {
      first = i;
      break;
    }
  }

  for (int i = 0, index = first; i < len; i++) {
    SA[i] = p[index].str[0];
    printf("{num: %d, char: %c}\n", p[index].num, p[index].str[0]);
    index = p[index].num;

    if (SA == NULL) {
      sprintf(SA, "%c", p[i].str[len]);
    } else {
      sprintf(SA, "%s%c", SA, p[i].str[len]);
    }
  }
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
        printf("{option: '%c', mode: 'Burrows-Wheeler Transform', input_file: '%s'}\n", optopt, optarg);
        mode_bwt_file = 1;
        return optarg;

      case 'i':
        printf("{option: '%c', mode: 'Inverse Burrows-Wheeler Transform', input_file: '%s'}\n", optopt, optarg);
        mode_ibwt_file = 1;
        return optarg;

      case 's':
        printf("{option: '%c', mode: 'Burrows-Wheeler Transform', input_stream: '%s'}\n", optopt, optarg);
        mode_bwt_stream = 1;
        return optarg;

      case 'r':
        printf("{option: '%c', mode: 'Inverse Burrows-Wheeler Transform', input_stream: '%s'}\n", optopt, optarg);
        mode_ibwt_stream = 1;
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
* main
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

  if (mode_bwt_file || mode_ibwt_file) {
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

  char SA[len];
  memset(&SA, 0, sizeof(SA));

  if (show_help) {
    printf("\nOptions:\n");
    printf("  -t :  Burrows-Wheeler Transform. Input File.\n");
    printf("  -i :  Inverse Burrows-Wheeler Transform. Input File.\n");
    printf("  -s :  Burrows-Wheeler Transform. Input Stream.\n");
    printf("  -r :  Inverse Burrows-Wheeler Transform. Input Stream.\n");
    printf("  -v :  Show Version.\n");
    printf("  -h :  Show Help.\n");
    return 0;
  }

  if (show_version) {
    printf("\nVersion:\n%s\n", VERSION);
    return 0;
  }

  if (mode_bwt_file) {
    create_suffix_array(s, len, SA);
  }

  if (mode_ibwt_file) {
    create_inverse_suffix_array(s, len, SA);
  }

  if (mode_bwt_stream) {
    create_suffix_array(input, strlen(input), SA);
  }

  if (mode_ibwt_stream) {
    create_inverse_suffix_array(input, strlen(input), SA);
  }

  printf("-- Result --\n%s\n", SA);
  return 0;
}
