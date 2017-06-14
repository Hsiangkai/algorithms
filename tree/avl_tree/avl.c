#include <stdio.h>
#include <stdlib.h>

struct element {
  int key;
};

typedef struct tree_node *tree_pointer;

struct tree_node {
  struct element data;
  int bf;
  tree_pointer left_child;
  tree_pointer right_child;
};

int unbalanced = 0;
struct tree_node *root = NULL;

static void left_rotation(tree_pointer *parent, int *unbalanced)
{
  tree_pointer child;
  tree_pointer grand_child;

  child = (*parent)->left_child;

  if (child->bf == 1) {
    /* LL rotation */
    (*parent)->left_child = child->right_child;
    child->right_child = (*parent);
    (*parent)->bf = 0;
    *parent = child;
  } else {
    /* LR rotation */
    grand_child = child->right_child;
    (*parent)->left_child = grand_child->right_child;
    child->right_child = grand_child->left_child;
    grand_child->left_child = child;
    grand_child->right_child = (*parent);
    if (grand_child->bf == 0) {
      (*parent)->bf = 0;
      child->bf = 0;
    } else if (grand_child->bf == 1) {
      (*parent)->bf = -1;
      child->bf = 0;
    } else if (grand_child->bf == -1) {
      (*parent)->bf = 0;
      child->bf = 1;
    }
    (*parent) = grand_child;
  }
  (*parent)->bf = 0;
  *unbalanced = 0;
}

static void right_rotation(tree_pointer *parent, int *unbalanced)
{
  tree_pointer child;
  tree_pointer grand_child;

  child = (*parent)->right_child;

  if (child->bf == -1) {
    /* LL rotation */
    (*parent)->right_child = child->left_child;
    child->left_child = (*parent);
    (*parent)->bf = 0;
    *parent = child;
  } else {
    /* LR rotation */
    grand_child = child->left_child;
    (*parent)->right_child = grand_child->left_child;
    child->left_child = grand_child->right_child;
    grand_child->right_child = child;
    grand_child->left_child = (*parent);
    if (grand_child->bf == 0) {
      (*parent)->bf = 0;
      child->bf = 0;
    } else if (grand_child->bf == 1) {
      (*parent)->bf = 0;
      child->bf = -1;
    } else if (grand_child->bf == -1) {
      (*parent)->bf = 1;
      child->bf = 0;
    }
    (*parent) = grand_child;
  }
  (*parent)->bf = 0;
  *unbalanced = 0;
}

void avl_insert(tree_pointer *parent, int value, int *unbalanced)
{
  if (*parent == NULL) {
    *parent = malloc (sizeof(struct tree_node));
    (*parent)->data.key = value;
    (*parent)->bf = 0;
    (*parent)->left_child = NULL;
    (*parent)->right_child = NULL;
    *unbalanced = 1;  /* new node added, so we need to re-balance. */
  } else if (value < (*parent)->data.key) {
    avl_insert (&(*parent)->left_child, value, unbalanced);
    if (*unbalanced) {
      switch ((*parent)->bf) {
	case -1:
	  (*parent)->bf = 0;
	  *unbalanced = 0;
	  break;
	case 0:
	  (*parent)->bf = 1;
	  break;
	case 1:
	  left_rotation(parent, unbalanced);
	  break;
      }
    }
  } else if (value > (*parent)->data.key) {
    avl_insert (&(*parent)->right_child, value, unbalanced);
    if (*unbalanced) {
      switch ((*parent)->bf) {
	case -1:
	  right_rotation(parent, unbalanced);
	  break;
	case 0:
	  (*parent)->bf = -1;
	  break;
	case 1:
	  (*parent)->bf = 0;
	  *unbalanced = 0;
	  break;
      }
    }
  } else { /* value == (*parent)->data.key */
    printf ("duplicated\n");
    *unbalanced = 0;
  }
}

void avl_delete()
{
}

tree_pointer avl_search(int query)
{
  tree_pointer current = root;

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

static void print_avl_tree(struct tree_node *current)
{
  if (current == NULL)
    return;
  print_avl_tree (current->left_child);
  printf ("%d ", current->data.key);
  print_avl_tree (current->right_child);
}

int main()
{
  int test_data[] = {23, 1, 5, 6, 14, 53, 25, 41, 33, 2, 7};
  int i;
  tree_pointer find;

  for (i = 0; i < sizeof(test_data)/sizeof(int); i++) {
    avl_insert (&root, test_data[i], &unbalanced);
  }

  print_avl_tree(root);
  printf ("\n");

  find = avl_search(25);
  if (find != NULL)
    printf ("find %d\n", find->data.key);
  else
    printf ("can not find\n");

  return 0;
}
