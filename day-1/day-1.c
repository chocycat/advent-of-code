#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH (5 * 2) + 3 + 2 // XXXXX   XXXXX\n\0

int comp(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("err: you must supply the input path\n");
    return 1;
  }

  // open file
  FILE *fptr = fopen(argv[1], "r");
  if (fptr == NULL) {
    printf("err: failed to open file handle: %s\n", strerror(errno));
    return 1;
  }

  int count = 0;
  long *ll = malloc(sizeof(long));
  long *lr = malloc(sizeof(long));

  char line[LINE_LENGTH];
  while (fgets(line, sizeof(line), fptr)) {
    char sn1[6], sn2[6];

    snprintf(sn1, 5, "%s", line);
    snprintf(sn2, 5, "%s", &line[8]);

    count++;

    long *temp = realloc(ll, sizeof(long) * count);
    if (temp == NULL) {
      free(ll);
      printf("err: failed to reallocate memory\n");
      return 1;
    }
    ll = temp;

    temp = realloc(lr, sizeof(long) * count);
    if (temp == NULL) {
      free(lr);
      printf("err: failed to reallocate memory\n");
      return 1;
    }
    lr = temp;

    ll[count - 1] = strtol(sn1, NULL, 10);
    lr[count - 1] = strtol(sn2, NULL, 10);
  }

  fclose(fptr);

  // sort both arrays
  qsort(ll, count, sizeof(long), comp);
  qsort(lr, count, sizeof(long), comp);

  int sum = 0;
  int fscore = 0;

  // calculate distance
  for (int i = 0; i < count; i++) {
    unsigned int distance = labs(ll[i] - lr[i]);
    sum += distance;

    // calculate frequency
    int appears = 0;
    for (int j = 0; j < count; j++) {
      if (ll[i] == lr[j]) {
        appears++;
      }
    }

    fscore += (ll[i] * appears);
  }

  free(ll);
  free(lr);

  printf("total distance: %d\n", sum);
  printf("frequency score: %d\n", fscore);
  return 0;
}
