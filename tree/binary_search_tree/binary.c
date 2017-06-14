#include <stdio.h>
#include <stdlib.h>

struct element {
  int key;
};

struct tree_node {
  struct element data;
  struct tree_node *left_child;
  struct tree_node *right_child;
};

struct tree_node *root = NULL;

void binary_insert(int data_val)
{
  struct tree_node **parent = &root;

  while (*parent != NULL) {
    if (data_val < (*parent)->data.key) {
      parent = &(*parent)->left_child;
    } else if (data_val > (*parent)->data.key) {
      parent = &(*parent)->right_child;
    } else {
      printf ("duplicated data\n");
      return;
    }
  }

  *parent = malloc (sizeof(struct tree_node));
  (*parent)->left_child = NULL;
  (*parent)->right_child = NULL;
  (*parent)->data.key = data_val;
}

struct tree_node *binary_search(int query)
{
  struct tree_node *current = root;

  while (current != NULL) {
    if (current->data.key == query) {
      return current;
    } else if (query < current->data.key) {
      current = current->left_child;
    } else if (query > current->data.key) {
      current = current->right_child;
    }
  }

  return NULL;
}

void binary_delete(int del_val)
{
  struct tree_node **current = &root;
  struct tree_node *tmp;
  struct tree_node **candidate;

  while ((*current) != NULL) {
    if ((*current)->data.key == del_val) {
      /* 1. leaf node
       * 2. only has one child
       * 3. has two children */
      if (((*current)->left_child == NULL)
	  && ((*current)->right_child == NULL)) {
	free (*current);
	*current = NULL;
      } else if (((*current)->left_child == NULL)
	  || ((*current)->right_child == NULL)) {
	tmp = *current;
	if ((*current)->left_child) {
	  *current = (*current)->left_child;
	} else {
	  *current = (*current)->right_child;
	}
	free (tmp);
      } else {
	candidate = &(*current)->left_child;
	while ((*candidate)->right_child != NULL)
	  candidate = &(*candidate)->right_child;

	(*current)->data.key = (*candidate)->data.key;
	if (((*candidate)->left_child == NULL)
	    && ((*candidate)->right_child == NULL)) {
	  free(*candidate);
	  *candidate = NULL;
	} else {
	  tmp = *candidate;
	  if ((*candidate)->left_child) {
	    *candidate = (*candidate)->left_child;
	  } else {
	    *candidate = (*candidate)->right_child;
	  }
	  free (tmp);
	}
      }
    } else if (del_val < (*current)->data.key) {
      current = &((*current)->left_child);
    } else if (del_val > (*current)->data.key) {
      current = &((*current)->right_child);
    }
  }
}

void print_binary_search_tree(struct tree_node *current)
{
  if (current == NULL)
    return;
  print_binary_search_tree (current->left_child);
  printf ("%d ", current->data.key);
  print_binary_search_tree (current->right_child);
}

int main ()
{
  int test_data[] = {23, 1, 5, 6, 14, 53, 25, 41, 33, 2, 7};
  int i;
  struct tree_node *find;

  for (i = 0; i < sizeof(test_data)/sizeof(int); i++) {
    binary_insert (test_data[i]);
  }

  find = binary_search(25);
  if (find != NULL)
    printf ("find %d\n", find->data.key);
  else
    printf ("can not find\n");

  binary_delete(25);
  find = binary_search(7);
  if (find != NULL)
    printf ("find %d\n", find->data.key);

  print_binary_search_tree (root);
  printf ("\n");

  return 0;
}
