#include <stdio.h>

#define MAX_SIZE 100
#define FALSE 0
#define TRUE 1
#define SWAP(x, y, t) ((t) = (x), (x) = (y), (y) = (t))

int heap[MAX_SIZE];
int heap_size = 0;

enum level_type {
  MIN_LEVEL,
  MAX_LEVEL
};

static enum level_type level(int index)
{
  while (index >= 4) {
    index = index >> 2;
  }

  if (index == 1)
    return MIN_LEVEL;
  else
    return MAX_LEVEL;
}

static void adjust_max(int index, int value)
{
  int grandparent = index / 4;
  while (grandparent) {
    if (heap[grandparent] < value) {
      heap[index] = heap[grandparent];
      index = grandparent;
      grandparent = index / 4;
    } else {
      break;
    }
  }

  heap[index] = value;
}

static void adjust_min(int index, int value)
{
  int grandparent = index / 4;
  while (grandparent) {
    if (heap[grandparent] > value) {
      heap[index] = heap[grandparent];
      index = grandparent;
      grandparent = index / 4;
    } else {
      break;
    }
  }

  heap[index] = value;
}

static void min_max_insert(int value)
{
  int parent;

  heap_size++;
  if (heap_size == 1) {
    heap[heap_size] = value;
  } else {
    parent = heap_size / 2;

    if (level(parent) == MAX_LEVEL) {
      if (value > heap[parent]) {
	heap[heap_size] = heap[parent];
	adjust_max(parent, value);
      } else {
	adjust_min(heap_size, value);
      }
    } else {
      if (value < heap[parent]) {
	heap[heap_size] = heap[parent];
	adjust_min(parent, value);
      } else {
	adjust_max(heap_size, value);
      }
    }
  }
}

static int min_child_grandchild(int index)
{
  int min = heap[index * 2];
  int min_index = index * 2;
  int end_index = index * 4 + 4;
  int i;

  for (i = index * 2 + 1; i <= heap_size && i < end_index; i++) {
    if (min > heap[i]) {
      min = heap[i];
      min_index = i;
    }
  }

  return min_index;
}

static void reconstruct_min_heap(int index, int value)
{
  if ((index * 2) > heap_size) {  /* There is no child. */
    heap[index] = value;
    return;
  }

  int min_index = min_child_grandchild(index);
  int parent_index;
  int temp;

  if (value <= heap[min_index]) {
    heap[index] = value;
  } else {
    if (level(min_index) == MIN_LEVEL) {
      heap[index] = heap[min_index];
      parent_index = min_index / 2;
      if (value > heap[parent_index]) {
	temp = heap[parent_index];
	heap[parent_index] = value;
	value = temp;
      }
      reconstruct_min_heap(min_index, value);
    } else {
      heap[index] = heap[min_index];
      heap[min_index] = value;
    }
  }
}

static int min_delete(void)
{
  int min = heap[1];
  int value = heap[heap_size];
  heap_size--;

  reconstruct_min_heap(1, value);

  return min;
}

static int max_child_grandchild(int index)
{
  int max = heap[index * 2];
  int max_index = index * 2;
  int end_index = index * 4 + 4;
  int i;

  for (i = index * 2 + 1; i <= heap_size && i < end_index; i++) {
    if (max < heap[i]) {
      max = heap[i];
      max_index = i;
    }
  }

  return max_index;
}

static void reconstruct_max_heap(int index, int value)
{
  if ((index * 2) > heap_size) {
    heap[index] = value;
    return;
  }

  int max_index = max_child_grandchild(index);
  int parent_index;
  int temp;

  if (value >= heap[max_index]) {
    heap[index] = value;
  } else {  /* value < heap[max_index] */
    if (level(max_index) == MIN_LEVEL) {
      heap[index] = heap[max_index];
      heap[max_index] = value;
    } else {  /* MAX_LEVEL */
      heap[index] = heap[max_index];
      parent_index = max_index / 2;
      if (value < heap[parent_index]) {
	temp = heap[parent_index];
	heap[parent_index] = value;
	value = temp;
      }
      reconstruct_max_heap(max_index, value);
    }
  }
}

static int max_delete(void)
{
  int max = heap[1];
  int max_index = 1;
  int value;
  int i;

  for (i = 2; i <= heap_size && i < 4; i++) {
    if (max < heap[i]) {
      max = heap[i];
      max_index = i;
    }
  }

  if (max_index != heap_size) {
    value = heap[heap_size];
  } else {
    value = heap[heap_size-1];
  }
  heap_size--;

  reconstruct_max_heap(max_index, value);

  return max;
}

int main()
{
  int test_data[] = {30, 9, 45, 50, 20, 30, 10, 15, 12, 40, 70, 7};
  int i;
  int value;

  for (i = 0; i < sizeof(test_data)/sizeof(int); i++) {
    min_max_insert(test_data[i]);
  }
  printf ("%d\n", heap[1]);

  for (i = 0; i < sizeof(test_data)/sizeof(int); i++) {
    value = max_delete();
    printf ("%d ", value);
  }
  printf ("\n");

  return 0;
}
