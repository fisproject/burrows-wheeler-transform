/*
* String search using the Move-To-Front in C
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>

#define FILE_LEN 1024
#define BUF_SIZE 1024

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
  char MTF[BUF_SIZE];
  memset(MTF, 0, sizeof(MTF));
  count = 0;

  for (int i = 0; i < len; i++) {
    char a = str[i];
    printf("search -> [%c] listState -> [%s]\n", a, strList);

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

  const char *_MTF = MTF;
  return _MTF;
}

/*
*　Usage
*  -t :  Move_to_front Transform
*  -i :  Inverse Move_to_front Transform
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
        printf("%c-option, input-stream:%s \n", optopt, optarg);
        str = optarg;
        optcode = 2;
        break;
      case 'm':
        printf("%c-option, input-stream:%s \n", optopt, optarg);
        str = optarg;
        optcode = 3;
        break;
      case 'h':
        printf("Usage\n");
        printf("-t :  Move_to_front Transform\n");
        printf("-i :  Inverse Move_to_front Transform\n");
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

  const char *ret;

  switch (optcode) {
    case 0:  // MFT
      ret = move_to_front_transform(s, len);
      break;
    case 1:  // IMFT
      // ret = create_inverse_suffix_array(s,len);
      break;
    case 2:
      len = strlen(str);
      // ret = create_suffix_array(str,len);
      break;
    case 3:
      len = strlen(str);
      // ret = create_inverse_suffix_array(str,len);
      break;
  }

  printf("-- Result --\n");
  printf("%s\n", ret);
}