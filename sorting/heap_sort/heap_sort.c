#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

static void insert_heap(int *heap, int i)
{
  int j;
  int tmp;

  while (i != 1) {
    j = i / 2;
    if (heap[j] > heap[i]) {
      tmp = heap[j];
      heap[j] = heap[i];
      heap[i] = tmp;
    }
    i = j;
  }
}

static int extract_min(int *heap, int *n)
{
  int min = heap[1];
  int size = *n;
  int i, j;
  int tmp;

  heap[1] = heap[size];
  i = 1;
  while (1) {
    j = i * 2;
    if (j >= size) break;
    if ((j + 1 < size) && (heap[j] > heap[j + 1]))
      j++;

    if (heap[i] > heap[j]) {
      tmp = heap[i];
      heap[i] = heap[j];
      heap[j] = tmp;
    }

    i = j;
  }

  *n = *n - 1;

  return min;
}

static void heapify(int *heap, int n)
{
  int i;

  for (i = 1; i <= n; i++) {
    insert_heap(heap, i);
  }
}

static void output(int *data, int n)
{
  int i;
  for (i = 0; i < n; i++)
  {
    printf ("%d ", data[i]);
  }
  printf ("\n");
}

void heap_sort(int *data, int n)
{
  int sorted_i;
  int min;
  int *heap = malloc(sizeof(int) * (n+1));
  int heap_size = n;

  // create heap
  memcpy(heap + 1, data, sizeof(int) * n);
  heapify(heap, n);
  output(heap + 1, n);

  for (sorted_i = 0; sorted_i < n; sorted_i++) {
    // find minimum element from data
    min = extract_min(heap, &heap_size);

    // insert the element into sorted data
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
  
  heap_sort(data, n);

  output(data, n);

  return 0;
}
