#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void swap(int *v1, int *v2)
{
  int tmp;
  tmp = *v1;
  *v1 = *v2;
  *v2 = tmp;
}

static int partition(int *data, int n)
{
  int pivot = data[n-1];
  int i;
  int first_high = 0;

  for (i = 0; i < n-1; i++) {
    if (data[i] < pivot) {
      swap(&data[i], &data[first_high]);
      first_high++;
    }
  }

  swap(&data[first_high], &data[n-1]);

  return first_high;
}

void quick_sort(int *data, int n)
{
  int pivot;

  if (n <= 1)
    return;

  pivot = partition(data, n);
  quick_sort(data, pivot);
  quick_sort(data + pivot + 1, n - pivot - 1);
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
  
  quick_sort(data, n);

  for (i = 0; i < n; i++)
  {
    printf ("%d ", data[i]);
  }
  printf ("\n");

  return 0;
}
