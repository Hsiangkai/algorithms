#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void merge_sort(int *data, int n)
{
  int half = n/2;
  int i, j, k;
  int *upper;
  int *merge_data = malloc (sizeof(int) * n);

  if (n == 1)
    return;

  // sort by recursive
  merge_sort(data, half);
  merge_sort(data + half, n - half);
  upper = data + half;

  // merge
  for (i = 0, j = 0, k = 0; i < half && j < (n - half); ) {
    if (data[i] <= upper[j]) {
      merge_data[k++] = data[i++];
    } else if (data[i] > upper[j]) {
      merge_data[k++] = upper[j++];
    }
  }

  while (i < half)
    merge_data[k++] = data[i++];

  while (j < (n - half))
    merge_data[k++] = upper[j++];

  memcpy(data, merge_data, sizeof(int) * n);
}

#define SIZE 1024

int input(int *data)
{
  char buffer[SIZE];
  char *elem;
  int i;

  fgets(buffer, SIZE, stdin); 

  elem = strtok(buffer, " ");
  data[0] = atoi(elem);
  i = 1;
  while ((elem = strtok(NULL, " ")) != NULL) {
    data[i] = atoi(elem);
    i++;
  }

  return i;
}

int main()
{
  int data[100];
  int i;
  int n;

  n = input(data);
  
  merge_sort(data, n);

  for (i = 0; i < n; i++)
  {
    printf ("%d ", data[i]);
  }
  printf ("\n");

  return 0;
}
