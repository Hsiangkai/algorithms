#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct twothree_node *twothree_pointer;

struct twothree_node {
  int data_l;
  int data_r;
  twothree_pointer left_child;
  twothree_pointer middle_child;
  twothree_pointer right_child;
};

twothree_pointer root;

static void new_root(twothree_pointer *root, int value, twothree_pointer child)
{
  twothree_pointer nroot;
  nroot = malloc (sizeof(struct twothree_node));
  nroot->data_l = value;
  nroot->data_r = INT_MAX;
  nroot->left_child = *root;
  nroot->middle_child = child;
  nroot->right_child = NULL;

  *root = nroot;
}

static int compare(twothree_pointer node, int query)
{
  if (query < node->data_l)
    return 1;
  else if (query > node->data_l && query < node->data_r)
    return 2;
  else if (query > node->data_r)
    return 3;

  return 4;
}

twothree_pointer parent_stack[100];
int stack_index;

static twothree_pointer find_node(twothree_pointer tree, int query)
{
  twothree_pointer parent = tree;
  stack_index = 0;
  while (tree) {
    parent = tree;
    parent_stack[stack_index++] = parent;
    switch (compare(tree, query)) {
      case 1:
	tree = tree->left_child;
	break;
      case 2:
	tree = tree->middle_child;
	break;
      case 3:
	tree = tree->right_child;
	break;
      case 4:
	return NULL;
    }
  }
  return parent;
}

static void put_in(twothree_pointer node, int value, twothree_pointer child)
{
  if (value > node->data_l) {
    node->data_r = value;
    node->right_child = child;
  } else if (value < node->data_l) {
    node->data_r = node->data_l;
    node->right_child = node->middle_child;
    node->data_l = value;
    node->middle_child = child;
  }
}

static void split(twothree_pointer *node, int *middle_value, twothree_pointer *child)
{
  int small;
  int mid;
  int large;
  twothree_pointer left_child = NULL;
  twothree_pointer middle_left_child = NULL;
  twothree_pointer middle_right_child = NULL;
  twothree_pointer right_child = NULL;
  twothree_pointer new_node;

  if (*middle_value > (*node)->data_r) {
    large = *middle_value;
    mid = (*node)->data_r;
    small = (*node)->data_l;
    left_child = (*node)->left_child;
    middle_left_child = (*node)->middle_child;
    if ((*node)->right_child != NULL) {
      if (large > (*node)->right_child->data_l) {
        middle_right_child = (*node)->right_child;
	right_child = *child;
      } else {
	middle_right_child = *child;
        right_child = (*node)->right_child;
      }
    }
  } else if (*middle_value < (*node)->data_l) {
    large = (*node)->data_r;
    mid = (*node)->data_l;
    small = *middle_value;
    if ((*node)->left_child != NULL) {
      if (small > (*node)->left_child->data_l) {
	middle_left_child = *child;
        left_child = (*node)->left_child;
      } else {
        middle_left_child = (*node)->left_child;
	left_child = *child;
      }
    }
    middle_right_child = (*node)->middle_child;
    right_child = (*node)->right_child;
  } else {
    large = (*node)->data_r;
    mid = *middle_value;
    small = (*node)->data_l;
    left_child = (*node)->left_child;
    right_child = (*node)->right_child;
    if ((*node)->middle_child != NULL) {
      if (mid < (*node)->middle_child->data_l) {
	middle_left_child = *child;
	middle_right_child = (*node)->middle_child;
      } else {
	middle_left_child = (*node)->middle_child;
	middle_right_child = *child;
      }
    }
  }

  (*node)->data_l = small;
  (*node)->data_r = INT_MAX;
  (*node)->left_child = left_child;
  (*node)->middle_child = middle_left_child;
  (*node)->right_child = NULL;

  new_node = malloc (sizeof(struct twothree_node));
  new_node->data_l = large;
  new_node->data_r = INT_MAX;
  new_node->left_child = middle_right_child;
  new_node->middle_child = right_child;
  new_node->right_child = NULL;

  *middle_value = mid;
  *child = new_node;
}

static twothree_pointer delete(void)
{
  if (stack_index == 0)
    return NULL;

  stack_index--;
  return parent_stack[stack_index];
}

void twothree_insert(twothree_pointer *tree, int value)
{
  twothree_pointer insert;
  twothree_pointer child = NULL;

  if (*tree == NULL) {
    new_root(tree, value, NULL);
  } else {
    insert = find_node(*tree, value);
    if (insert == NULL)
      return;

    while (1) {
      if (insert->data_r == INT_MAX) {
	put_in(insert, value, child);
	break;
      } else {
	split (&insert, &value, &child);
	if (insert == *tree) {
	  new_root(tree, value, child);
	  break;
	}
        insert = delete();
      }
    }
  }
}

twothree_pointer twothree_search(twothree_pointer t, int query)
{
  while (t) {
    switch (compare(t, query)) {
      case 1:
	t = t->left_child;
	break;
      case 2:
	t = t->middle_child;
	break;
      case 3:
	t = t->right_child;
	break;
      case 4:
	return t;
    }
  }
  return NULL;
}

int main()
{
  int test_data[] = {23, 1, 5, 6, 14, 53, 25, 41, 33, 2, 7};
  int i;
  twothree_pointer find;
  int key;

  for (i = 0; i < sizeof(test_data)/sizeof(int); i++) {
    twothree_insert (&root, test_data[i]);
  }

  key = 33;
  find = twothree_search(root, key);
  if (find != NULL) {
    if (find->data_l == key)
      printf ("find %d\n", find->data_l);
    else
      printf ("find %d\n", find->data_r);
  } else
    printf ("can not find\n");

  return 0;
}
