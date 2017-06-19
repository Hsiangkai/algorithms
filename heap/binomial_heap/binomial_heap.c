#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct binomial_node *binomial_pointer;
struct binomial_node {
  int degree;
  binomial_pointer child;
  binomial_pointer parent;
  binomial_pointer left_link;
  binomial_pointer right_link;
  int data;
};

binomial_pointer root = NULL;

void binomial_insert(int value)
{
  binomial_pointer new_node;

  new_node = malloc (sizeof (struct binomial_node));
  new_node->degree = 0;
  new_node->child = NULL;
  new_node->parent = NULL;
  new_node->left_link = new_node;
  new_node->right_link = new_node;
  new_node->data = value;

  if (!root) {
    root = new_node;
    return;
  }

  new_node->left_link = root->left_link;
  root->left_link->right_link = new_node;
  root->left_link = new_node;
  new_node->right_link = root;

  if (root->data > new_node->data) {
    root = new_node;
  }
}

#define MAX_DEGREE 100
binomial_pointer tree[MAX_DEGREE];

static void join_min_trees(binomial_pointer *current, binomial_pointer joined)
{
  binomial_pointer child;
  binomial_pointer sibling;
  binomial_pointer parent;
  binomial_pointer cur = *current;

  if (cur->data < joined->data) {
    parent = cur;
    child = cur->child;
    sibling = joined;
  } else {
    parent = joined;
    child = joined->child;
    sibling = cur;
  }

  parent->degree++;
  sibling->parent = parent;
  sibling->right_link->left_link = sibling->left_link;
  sibling->left_link->right_link = sibling->right_link;
  if (parent->child == NULL) {
    parent->child = sibling;
    sibling->left_link = sibling;
    sibling->right_link = sibling;
  } else {
    sibling->right_link = child->right_link;
    child->right_link = sibling;
    sibling->right_link->left_link = sibling;
    sibling->left_link = child;
  }

  *current = parent;
}

#define MAX_NODE 1000

int binomial_delete_min(void)
{
  int min = root->data;
  binomial_pointer root_child = root->child;
  binomial_pointer current;
  binomial_pointer temp;
  int degree;
  binomial_pointer buffer[MAX_NODE];
  int i;
  int n;

  /* delete root */
  if (root->left_link == root) {
    free(root);
    root = NULL;
  } else {
    root->right_link->left_link = root->left_link;
    root->left_link->right_link = root->right_link;
    temp = root;
    root = root->left_link;
    free(temp);
  }

  for (i = 0; i < MAX_DEGREE; i++) {
    tree[i] = NULL;
  }

  if (root_child != NULL) {
    for (i = 0, current = root_child; ; current = current->left_link) {
      buffer[i++] = current;
      if (current == root_child->right_link)
	break;
    }
    n = i;

    for (i = 0; i < n; i++) {
      current = buffer[i];
      for (degree = current->degree;
	  tree[degree];
	  degree++) {
	join_min_trees(&current, tree[degree]);
	tree[degree] = NULL;
      }
      tree[degree] = current;
    }
  }

  if (root != NULL) {
    for (i = 0, current = root; ; current = current->left_link) {
      buffer[i++] = current;
      if (current == root->right_link)
	break;
    }
    n = i;

    for (i = 0; i < n; i++) {
      current = buffer[i];
      for (degree = current->degree;
	  tree[degree];
	  degree++) {
	join_min_trees(&current, tree[degree]);
	tree[degree] = NULL;
      }
      tree[degree] = current;
    }
  }

  /* update root */
  int min_value = INT_MAX;
  binomial_pointer min_tree = NULL;
  for (i = 0; i < MAX_DEGREE; i++) {
    if (tree[i] != NULL) {
      if (tree[i]->data < min_value) {
	min_value = tree[i]->data;
	min_tree = tree[i];
      }
    }
  }
  root = min_tree;
  if (root != NULL) {
    root->parent = NULL;
    root->left_link = root;
    root->right_link = root;
    for (i = 0; i < MAX_DEGREE; i++) {
      if (tree[i] != NULL && (tree[i] != root)) {
	tree[i]->parent = NULL;
	tree[i]->right_link = root->right_link;
	root->right_link->left_link = tree[i];
	root->right_link = tree[i];
	tree[i]->left_link = root;
      }
    }
  }

  return min;
}

int main()
{
  int test_data[] = {30, 9, 45, 50, 20, 30, 10, 15, 12, 40, 70, 7};
  int i;
  int value;

  for (i = 0; i < sizeof(test_data)/sizeof(int); i++) {
    binomial_insert(test_data[i]);
  }

  for (i = 0; i < sizeof(test_data)/sizeof(int); i++) {
    value = binomial_delete_min();
    printf ("%d ", value);
  }
  printf ("\n");

  return 0;
}
