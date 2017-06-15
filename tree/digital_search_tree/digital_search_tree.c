#include <stdio.h>
#include <stdlib.h>

typedef struct digital_tree *digital_pointer;
struct digital_tree {
  int value;
  digital_pointer left_child;
  digital_pointer right_child;
};

digital_pointer root;

int mask[] = {
  0x1, 0x2, 0x4, 0x8,
  0x10, 0x20, 0x40, 0x80,
  0x100, 0x200, 0x400, 0x800,
  0x1000, 0x2000, 0x4000, 0x8000
};

void digital_insert(digital_pointer *root, int value)
{
  digital_pointer *current = root;
  digital_pointer new_node;
  int i;

  i = 0;
  while (*current) {
    if (value == (*current)->value)
      return;

    if (value & mask[i])
      current = &((*current)->right_child);
    else
      current = &((*current)->left_child);

    i++;
  }

  new_node = malloc(sizeof(struct digital_tree));
  new_node->value = value;
  new_node->left_child = NULL;
  new_node->right_child = NULL;
  *current = new_node;
}

digital_pointer digital_search(digital_pointer root, int value)
{
  digital_pointer current = root;
  int i;

  i = 0;
  while (current) {
    if (value == current->value)
      return current;
    else if (value & mask[i])
      current = current->right_child;
    else
      current = current->left_child;

    i++;
  }

  return NULL;
}

int main()
{
  int test_data[] = {1, 4, 9, 8, 3, 0};
  int i;
  digital_pointer found;

  for (i = 0; i < sizeof(test_data)/sizeof(int); i++) {
    digital_insert(&root, test_data[i]);
  }

  for (i = 0; i < sizeof(test_data)/sizeof(int); i++) {
    found = digital_search(root, test_data[i]);
    if (found == NULL)
      printf ("can not find %d\n", test_data[i]);
    else
      printf ("find %d\n", found->value);
  }

  return 0;
}
