#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct two34 *two34_pointer;
struct two34 {
  int data_l;
  int data_m;
  int data_r;
  two34_pointer left_child;
  two34_pointer left_mid_child;
  two34_pointer right_mid_child;
  two34_pointer right_child;
};

enum e_node_type {
  two_node,
  three_node,
  four_node
};

enum e_compare_result {
  equal,
  leaf,
  lchild,
  lmchild,
  rmchild,
  rchild,
  unknown_result
};

static void new_root(two34_pointer *t, int value)
{
  two34_pointer new_node;
  new_node = malloc(sizeof(struct two34));
  new_node->data_l = value;
  new_node->data_m = INT_MAX;
  new_node->data_r = INT_MAX;
  new_node->left_child = NULL;
  new_node->left_mid_child = NULL;
  new_node->right_mid_child = NULL;
  new_node->right_child = NULL;

  *t = new_node;
}

static int is_four_node(two34_pointer node)
{
  if (node->data_r != INT_MAX)
    return 1;

  return 0;
}

static void split_root(two34_pointer *root)
{
  two34_pointer left_child;
  two34_pointer right_child;

  new_root(&left_child, (*root)->data_l);
  left_child->left_child = (*root)->left_child;
  left_child->left_mid_child = (*root)->left_mid_child;
  new_root(&right_child, (*root)->data_r);
  right_child->left_child = (*root)->right_mid_child;
  right_child->left_mid_child = (*root)->right_child;

  (*root)->data_l = (*root)->data_m;
  (*root)->data_m = INT_MAX;
  (*root)->data_r = INT_MAX;
  (*root)->left_child = left_child;
  (*root)->left_mid_child = right_child;
  (*root)->right_mid_child = NULL;
  (*root)->right_child = NULL;
}

static enum e_node_type node_type(two34_pointer node)
{
  if (node->data_m == INT_MAX)
    return two_node;
  else if (node->data_r == INT_MAX)
    return three_node;
  else
    return four_node;
}

static void split_child_of2(two34_pointer *node, two34_pointer *parent)
{
  two34_pointer new_node;

  new_node = malloc (sizeof(struct two34));

  if ((*node)->data_l < (*parent)->data_l) {
    (*parent)->data_m = (*parent)->data_l;
    (*parent)->data_l = (*node)->data_m;
    (*parent)->right_mid_child = (*parent)->left_mid_child;
    (*parent)->left_mid_child = new_node;
    new_node->data_l = (*node)->data_r;
    new_node->data_m = INT_MAX;
    new_node->data_r = INT_MAX;
    new_node->left_child = (*node)->right_mid_child;
    new_node->left_mid_child = (*node)->right_child;
    new_node->right_mid_child = NULL;
    new_node->right_child = NULL;
    (*node)->data_m = INT_MAX;
    (*node)->data_r = INT_MAX;
    (*node)->right_mid_child = NULL;
    (*node)->right_child = NULL;
  } else {
    (*parent)->data_m = (*node)->data_m;
    (*parent)->right_mid_child = new_node;
    new_node->data_l = (*node)->data_r;
    new_node->data_m = INT_MAX;
    new_node->data_r = INT_MAX;
    new_node->left_child = (*node)->right_mid_child;
    new_node->left_mid_child = (*node)->right_child;
    new_node->right_mid_child = NULL;
    new_node->right_child = NULL;
    (*node)->data_m = INT_MAX;
    (*node)->data_r = INT_MAX;
    (*node)->right_mid_child = NULL;
    (*node)->right_child = NULL;
  }
}

static void split_child_of3(two34_pointer *node, two34_pointer *parent)
{
  two34_pointer new_node;

  new_node = malloc (sizeof(struct two34));

  if ((*node)->data_l < (*parent)->data_l) {
    (*parent)->data_r = (*parent)->data_m;
    (*parent)->data_m = (*parent)->data_l;
    (*parent)->data_l = (*node)->data_m;
    (*parent)->right_child = (*parent)->right_mid_child;
    (*parent)->right_mid_child = (*parent)->left_mid_child;
    (*parent)->left_mid_child = new_node;
    new_node->data_l = (*node)->data_r;
    new_node->data_m = INT_MAX;
    new_node->data_r = INT_MAX;
    new_node->left_child = (*node)->right_mid_child;
    new_node->left_mid_child = (*node)->right_child;
    new_node->right_mid_child = NULL;
    new_node->right_child = NULL;
    (*node)->data_m = INT_MAX;
    (*node)->data_r = INT_MAX;
  } else if ((*node)->data_l < (*parent)->data_m) {
    (*parent)->data_r = (*parent)->data_m;
    (*parent)->data_m = (*node)->data_m;
    (*parent)->right_child = (*parent)->right_mid_child;
    (*parent)->right_mid_child = new_node;
    new_node->data_l = (*node)->data_r;
    new_node->data_m = INT_MAX;
    new_node->data_r = INT_MAX;
    new_node->left_child = (*node)->right_mid_child;
    new_node->left_mid_child = (*node)->right_child;
    new_node->right_mid_child = NULL;
    new_node->right_child = NULL;
    (*node)->data_m = INT_MAX;
    (*node)->data_r = INT_MAX;
    (*node)->right_mid_child = NULL;
    (*node)->right_child = NULL;
  } else if ((*node)->data_l > (*parent)->data_m) {
    (*parent)->data_r = (*node)->data_m;
    (*parent)->right_child = new_node;
    new_node->data_l = (*node)->data_r;
    new_node->data_m = INT_MAX;
    new_node->data_r = INT_MAX;
    new_node->left_child = (*node)->right_mid_child;
    new_node->left_mid_child = (*node)->right_child;
    new_node->right_mid_child = NULL;
    new_node->right_child = NULL;
    (*node)->data_m = INT_MAX;
    (*node)->data_r = INT_MAX;
    (*node)->right_mid_child = NULL;
    (*node)->right_child = NULL;
  }
}

static enum e_compare_result compare(two34_pointer node, int key)
{
  if ((key == node->data_l)
      || (key == node->data_m)
      || (key == node->data_r)) {
    return equal;
  } else if ((node->left_child == NULL)
      && (node->left_mid_child == NULL)
      && (node->right_mid_child == NULL)
      && (node->right_child == NULL)) {
    return leaf;
  } else if (key < node->data_l) {
    return lchild;
  } else if ((key > node->data_l) && (key < node->data_m)) {
    return lmchild;
  } else if ((key > node->data_m) && (key < node->data_r)) {
    return rmchild;
  } else if (key > node->data_r) {
    return rchild;
  }

  return unknown_result;
}

static void put_in(two34_pointer node, int value)
{
  if (node_type(node) == two_node) {
    if (value > node->data_l) {
      node->data_m = value;
    } else {
      node->data_m = node->data_l;
      node->data_l = value;
    }
  } else { /* three_node */
    if (value > node->data_m) {
      node->data_r = value;
    } else if ((value < node->data_m) && (value > node->data_l)) {
      node->data_r = node->data_m;
      node->data_m = value;
    } else if (value < node->data_l) {
      node->data_r = node->data_m;
      node->data_m = node->data_l;
      node->data_l = value;
    }
  }
}

void insert234(two34_pointer *tree, int value)
{
  two34_pointer current;
  two34_pointer parent;

  if (*tree == NULL) {
    new_root(tree, value);
  } else {
    if (is_four_node(*tree))
      split_root(tree);

    current = *tree;
    parent = NULL;
    while (1) {
      if (is_four_node(current)) {
	/* split */
	if (node_type(parent) == two_node) {
	  split_child_of2(&current, &parent);
	} else if (node_type(parent) == three_node) {
	  split_child_of3(&current, &parent);
	}
	current = parent;
      }

      parent = current;
      switch (compare(current, value)) {
	case equal:
	  return;
	case leaf:
	  put_in(current, value);
	  return;
	case lchild:
	  current = current->left_child;
	  break;
	case lmchild:
	  current = current->left_mid_child;
	  break;
	case rmchild:
	  current = current->right_mid_child;
	  break;
	case rchild:
	  current = current->right_child;
	  break;
	default:
	  break;
      }
    }
  }
}

two34_pointer two34_search(two34_pointer t, int query)
{
  while (t) {
    switch (compare(t, query)) {
      case equal:
	return t;
      case leaf:
	return NULL;
      case lchild:
	t = t->left_child;
	break;
      case lmchild:
	t = t->left_mid_child;
	break;
      case rmchild:
	t = t->right_mid_child;
	break;
      case rchild:
	t = t->right_child;
	break;
      default:
	break;
    }
  }

  return NULL;
}

two34_pointer root;

int main()
{
  int test_data[] = {23, 1, 5, 6, 14, 53, 25, 41, 33, 2, 7};
  int i;
  two34_pointer find;
  int key;

  for (i = 0; i < sizeof(test_data)/sizeof(int); i++) {
    insert234 (&root, test_data[i]);
  }

  for (i = 0; i < sizeof(test_data)/sizeof(int); i++) {
    key = test_data[i];
    find = two34_search(root, key);
    if (find != NULL) {
      if (find->data_l == key)
	printf ("find %d\n", find->data_l);
      else if (find->data_m == key)
	printf ("find %d\n", find->data_m);
      else
	printf ("find %d\n", find->data_r);
    } else
      printf ("can not find\n");
  }

  key = 100;
  find = two34_search(root, key);
  if (find != NULL) {
    if (find->data_l == key)
      printf ("find %d\n", find->data_l);
    else if (find->data_m == key)
      printf ("find %d\n", find->data_m);
    else
      printf ("find %d\n", find->data_r);
  } else
    printf ("can not find %d\n", key);

  return 0;
}
