#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void selection_sort(int *data, int n)
{
  int i;
  int sorted_i;
  int min;
  int min_index;

  for (sorted_i = 0; sorted_i < n; sorted_i++) {
    // find minimum element from data
    min = data[sorted_i];
    min_index = sorted_i;
    for (i = sorted_i + 1; i < n; i++) {
      if (data[i] < min) {
        min = data[i];
	min_index = i;
      }
    }
    // insert the element into sorted data
    data[min_index] = data[sorted_i];
    data[sorted_i] = min;
  }
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
  
  selection_sort(data, n);

  for (i = 0; i < n; i++)
  {
    printf ("%d ", data[i]);
  }
  printf ("\n");

  return 0;
}
