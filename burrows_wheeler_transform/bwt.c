/*
* String search using the Burrows-Wheeler transform in C
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>

#define MAX_LEN 2048
#define FILE_LEN 8192

/*
*  Pair
*
*/
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
const char *create_suffix_array(char *str, int len) {
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

    printf("%d : [%s]\n", p[i].num, p[i].str);
  }

  printf("-- Quick Sort --\n");
  // quick_sort
  qsort(p, len + 1, sizeof(Pair), compare);

  // create suffix array
  printf("-- After Sort --\n");
  char SA[len];
  memset(SA, 0, sizeof(SA));

  for (int i = 0; i < len + 1; i++) {
    printf("%d [%s]\n", p[i].num, p[i].str);
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

  const char *_SA = &SA[0];
  return _SA;
}

/*
* create_inverse_suffix_array
*
*/
const char *create_inverse_suffix_array(char *str, int len) {
  printf("-- starting --\n");
  Pair p[len];
  char strs[len];
  sprintf(strs, "%s", str);
  char tmp;

  // create_char_pair
  // 一文字ずつペア
  printf("-- Before Sort --\n");
  for (int i = 0; i < len; i++) {
    p[i].num = i;
    char a = strs[i];
    const char *s = &a;
    strcpy(p[i].str, s);

    printf("{num: %d,str: %c}\n", p[i].num, a);
  }

  printf("-- Quick Sort --\n");
  qsort(p, len, sizeof(Pair), compare);

  // create suffix array
  printf("-- After Sort --\n");
  char SA[len];
  memset(SA, 0, sizeof(SA));

  char ary[len];
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
    ary[i] = p[index].str[0];
    printf("{num: %d,str: %c}\n", p[index].num, p[index].str[0]);
    index = p[index].num;
  }

  const char *_SA = &ary[0];
  return _SA;
}

/*
*　Usage
*  -t :  Burrows-Wheeler transform
*  -i :  Inverse Burrows-Wheeler transform
*  -h :  show help
*/
int main(int argc, char **argv) {
  int c;
  char *str;
  int optcode;
  while ((c = getopt(argc, argv, "t:r:s:m:h")) != -1) {
    switch (c) {
      case 't':
        printf("{optionName:%c,input_file_name:%s}\n", optopt, optarg);
        str = optarg;
        optcode = 0;
        break;
      case 'r':
        printf("{optionName:%c,input_file_name:%s}\n", optopt, optarg);
        str = optarg;
        optcode = 1;
        break;
      case 's':
        printf("{optionName:%c,input_stream:%s}\n", optopt, optarg);
        str = optarg;
        optcode = 2;
        break;
      case 'm':
        printf("{optionName:%c,input_stream:%s}\n", optopt, optarg);
        str = optarg;
        optcode = 3;
        break;
      case 'h':
        printf("Usage\n");
        printf("-t :  Burrows-Wheeler transform\n");
        printf("-i :  Inverse Burrows-Wheeler transform\n");
        break;
      default:
        printf("invalid option\n");
        break;
    }
  }
  if (str == NULL) {
    printf("arguments required\n");
    return 0;
  }

  // Read File
  FILE *fp;
  char s[FILE_LEN];
  int len;

  if (optcode == 0 || optcode == 1) {
    if ((fp = fopen(str, "r")) == NULL) {
      printf("errno %d\n", errno);
      return 0;
    }
    while (fgets(s, FILE_LEN, fp) != NULL) {
      len = strlen(s);
      printf("{fileInput:%s,fileLength:%d}\n", s, len);
    }
    fclose(fp);
  }

  const char *SA;
  switch (optcode) {
    case 0:  // BWT
      SA = create_suffix_array(s, len);
      break;
    case 1:  // RBWT
      SA = create_inverse_suffix_array(s, len);
      break;
    case 2:
      len = strlen(str);
      SA = create_suffix_array(str, len);
      break;
    case 3:
      len = strlen(str);
      SA = create_inverse_suffix_array(str, len);
      break;
  }

  printf("-- Result --\n");
  printf("%s\n", SA);

  return 0;
}