#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void insertion_sort(int *data, int n)
{
  int i;
  int j;
  int tmp;

  for (i = 1; i < n; i++) {
    for (j = i; j > 0; j--) {
      if (data[j] < data[j-1]) {
	tmp = data[j];
	data[j] = data[j-1];
	data[j-1] = tmp;
      }
    }
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
  
  insertion_sort(data, n);

  for (i = 0; i < n; i++)
  {
    printf ("%d ", data[i]);
  }
  printf ("\n");

  return 0;
}
