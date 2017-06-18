#include <stdio.h>
#include <math.h>

#define MAX_SIZE 100

int deap[MAX_SIZE];
int deap_size = 1;

static int is_max_heap(int index)
{
  while (1) {
    if (index == 2)
      return 0;
    else if (index == 3)
      return 1;

    index = index >> 1;
  }
}

static int height(int index)
{
  int h = 0;

  while (index != 1) {
    index = index >> 1;
    h++;
  }

  return h;
}

static int min_partner(int index)
{
  return index - (int)pow(2, (height(index) - 1));
}

static int max_partner(int index)
{
  int partner = index + (int)pow(2, height(index) - 1);

  if (partner > deap_size)
    partner = partner / 2;

  return partner;
}

static void min_insert(int index, int value)
{
  int parent;

  while (index != 2) {
    parent = index / 2;
    if (deap[parent] > value) {
      deap[index] = deap[parent];
    } else {
      break;
    }

    index = index / 2;
  }

  deap[index] = value;
}

static void max_insert(int index, int value)
{
  int parent;

  while (index != 3) {
    parent = index / 2;
    if (deap[parent] < value) {
      deap[index] = deap[parent];
    } else {
      break;
    }

    index = index / 2;
  }

  deap[index] = value;
}

void deap_insert(int value)
{
  int partner;
  int insert_index;

  deap_size++;
  insert_index = deap_size;

  if (insert_index == 2) {
    deap[insert_index] = value;
    return;
  }

  if (is_max_heap(insert_index)) {
    partner = min_partner(insert_index);
    if (deap[partner] > value) {
      deap[insert_index] = deap[partner];
      min_insert(partner, value);
    } else {
      max_insert(insert_index, value);
    }
  } else {
    partner = max_partner(insert_index);
    if (deap[partner] < value) {
      deap[insert_index] = deap[partner];
      max_insert(partner, value);
    } else {
      min_insert(insert_index, value);
    }
  }
}

static void modified_insert_deap(int index, int value)
{
  int partner;

  if (index == 2) {
    deap[index] = value;
    return;
  }

  if (is_max_heap(index)) {
    partner = min_partner(index);
    if (deap[partner] > value) {
      deap[index] = deap[partner];
      min_insert(partner, value);
    } else {
      max_insert(index, value);
    }
  } else {
    partner = max_partner(index);
    if (deap[partner] < value) {
      deap[index] = deap[partner];
      max_insert(partner, value);
    } else {
      min_insert(index, value);
    }
  }
}

int delete_min()
{
  int min = deap[2];
  int value;
  int current_index = 2;
  int min_child;

  if (deap_size == 1)
    return -1;

  value = deap[deap_size];
  deap_size--;

  while (1) {
    min_child = current_index * 2;
    if (min_child > deap_size)
      break;

    if (min_child + 1 <= deap_size) {
      if (deap[min_child + 1] < deap[min_child])
	min_child++;
    }

    deap[current_index] = deap[min_child];
    current_index = min_child;
  }

  modified_insert_deap(current_index, value);

  return min;
}

int delete_max()
{
  int max = deap[3];
  int value;
  int current_index = 3;
  int max_child;

  if (deap_size == 1)
    return -1;

  if (deap_size == 2) {
    deap_size--;
    return deap[2];
  }

  value = deap[deap_size];
  deap_size--;

  while (1) {
    max_child = current_index * 2;
    if (max_child > deap_size)
      break;

    if (max_child + 1 <= deap_size) {
      if (deap[max_child + 1] > deap[max_child])
	max_child++;
    }

    deap[current_index] = deap[max_child];
    current_index = max_child;
  }

  modified_insert_deap(current_index, value);

  return max;
}

int main()
{
  int test_data[] = {30, 9, 45, 50, 20, 30, 10, 15, 12, 40, 70, 7};
  int i;
  int value;

  for (i = 0; i < sizeof(test_data)/sizeof(int); i++) {
    deap_insert(test_data[i]);
  }
  printf ("%d %d\n", deap[2], deap[3]);

  for (i = 0; i < sizeof(test_data)/sizeof(int); i++) {
    value = delete_max();
    printf ("%d ", value);
  }
  printf ("\n");

  return 0;
}
