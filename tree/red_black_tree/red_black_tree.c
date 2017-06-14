#include <stdio.h>
#include <stdlib.h>

typedef enum e_red_block_color {
  red,
  black
} red_black_color;

typedef enum e_compare_result {
  equal,
  leaf,
  lchild,
  rchild
} compare_result;

typedef enum e_rotation_type {
  LL,
  LR,
  RL,
  RR
} rotation_type;

typedef enum e_node_type {
  two_node,
  three_node
} node_type;

typedef struct red_black *red_black_pointer;
struct red_black {
  int value;
  red_black_pointer parent;
  red_black_pointer left_child;
  red_black_color left_color;
  red_black_pointer right_child;
  red_black_color right_color;
};

red_black_pointer root;

static void new_root(red_black_pointer *root, int value)
{
  red_black_pointer node = malloc(sizeof(struct red_black));
  node->value = value;
  node->parent = NULL;
  node->left_child = NULL;
  node->left_color = black;
  node->right_child = NULL;
  node->right_color = black;

  *root = node;
}

static int is_four_node(red_black_pointer node)
{
  if (node == NULL)
    return 0;

  if ((node->left_color == red) && (node->right_color == red))
    return 1;

  return 0;
}

static void split_root(red_black_pointer node)
{
  node->left_color = black;
  node->right_color = black;
}

static compare_result compare(red_black_pointer node, int value)
{
  if (node == NULL)
    return leaf;

  if (value == node->value)
    return equal;
  else if (value < node->value)
    return lchild;
  else
    return rchild;
}

static void left_rotation(red_black_pointer grandparent,
    red_black_pointer parent, red_black_pointer node, rotation_type type)
{
  if (type == LL) {
    grandparent->left_child = parent->right_child;
    grandparent->left_color = parent->right_color;
    parent->right_child = grandparent;
    parent->right_color = red;
    if (grandparent->parent != NULL) {
      if (grandparent->parent->left_child == grandparent) {
	grandparent->parent->left_child = parent;
      } else {
	grandparent->parent->right_child = parent;
      }
    } else {
      root = parent;
    }
    parent->parent = grandparent->parent;
    grandparent->parent = parent;
  } else if (type == LR) {
    grandparent->left_child = node->right_child;
    grandparent->left_color = node->right_color;
    parent->right_child = node->left_child;
    parent->right_color = node->left_color;
    node->left_child = parent;
    node->left_color = red;
    node->right_child = grandparent;
    node->right_color = red;
    if (grandparent->parent != NULL) {
      if (grandparent->parent->left_child == grandparent) {
	grandparent->parent->left_child = node;
      } else {
	grandparent->parent->right_child = node;
      }
    } else {
      root = node;
    }
    node->parent = grandparent->parent;
    grandparent->parent = node;
    parent->parent = node;
  }
}

static void right_rotation(red_black_pointer grandparent,
    red_black_pointer parent, red_black_pointer node, rotation_type type)
{
  if (type == RR) {
    grandparent->right_child = parent->left_child;
    grandparent->right_color = parent->left_color;
    parent->left_child = grandparent;
    parent->left_color = red;
    if (grandparent->parent != NULL) {
      if (grandparent->parent->left_child == grandparent) {
	grandparent->parent->left_child = parent;
      } else {
	grandparent->parent->right_child = parent;
      }
    } else {
      root = parent;
    }
    parent->parent = grandparent->parent;
    grandparent->parent = parent;
  } else if (type == RL) {
    grandparent->right_child = node->left_child;
    grandparent->right_color = node->left_color;
    parent->left_child = node->right_child;
    parent->left_color = node->right_color;
    node->left_child = grandparent;
    node->left_color = red;
    node->right_child = parent;
    node->right_color = red;
    if (grandparent->parent != NULL) {
      if (grandparent->parent->left_child == grandparent) {
	grandparent->parent->left_child = node;
      } else {
	grandparent->parent->right_child = node;
      }
    } else {
      root = node;
    }
    node->parent = grandparent->parent;
    grandparent->parent = node;
    parent->parent = node;
  }
}

static void put_in(red_black_pointer *node, red_black_pointer parent, int value)
{
  red_black_pointer new_node;
  red_black_pointer grandparent;
  int left;

  new_node = malloc(sizeof(struct red_black));
  new_node->value = value;
  new_node->left_child = NULL;
  new_node->right_child = NULL;
  new_node->left_color = black;
  new_node->right_color = black;
  new_node->parent = parent;
  *node = new_node;

  if (parent->left_child == (*node)) {
    parent->left_color = red;
    left = 1;
  } else {
    parent->right_color = red;
    left = 0;
  }

  if (parent->parent != NULL) {
    grandparent = parent->parent;
    if (grandparent->left_child == parent) {
      if (grandparent->left_color == red) {
	if (left)
	  left_rotation(grandparent, parent, *node, LL);
	else
	  left_rotation(grandparent, parent, *node, LR);
      }
    } else if (grandparent->right_child == parent) {
      if (grandparent->right_color == red) {
	if (left)
	  right_rotation(grandparent, parent, *node, RL);
	else
	  right_rotation(grandparent, parent, *node, RR);
      }
    }
  }
}

static node_type get_node_type(red_black_pointer parent)
{
  red_black_pointer grandparent = parent->parent;

  if ((parent->left_color == black) && (parent->right_color == black)) {
    if (grandparent != NULL) {
      if (grandparent->left_child == parent) {
	if (grandparent->left_color == black) {
	  return two_node;
	} else {
	  return three_node;
	}
      } else if (grandparent->right_child == parent) {
	if (grandparent->right_color == black) {
	  return two_node;
	} else {
	  return three_node;
	}
      }
    } else {
      return two_node;
    }
  }

  return three_node;
}

static void split_child_of2(red_black_pointer *node, red_black_pointer parent)
{
  (*node)->left_color = black;
  (*node)->right_color = black;

  if (parent->left_child == *node)
    parent->left_color = red;
  else
    parent->right_color = red;
}

static void split_child_of3(red_black_pointer *node, red_black_pointer parent)
{
  int left;
  red_black_pointer grandparent;

  (*node)->left_color = black;
  (*node)->right_color = black;

  if ((parent->left_color == red) || (parent->right_color == red)) {
    parent->left_color = red;
    parent->right_color = red;
  } else {
    (*node)->left_color = black;
    (*node)->right_color = black;
    if (parent->left_child == *node) {
      parent->left_color = red;
      left = 1;
    } else {
      parent->right_color = red;
      left = 0;
    }

    grandparent = parent->parent;
    if (grandparent->left_child == parent) {
      if (left) {
	left_rotation(grandparent, parent, *node, LL);
      } else {
	left_rotation(grandparent, parent, *node, LR);
      }
    } else {
      if (left) {
	right_rotation(grandparent, parent, *node, RL);
      } else {
	right_rotation(grandparent, parent, *node, RR);
      }
    }
  }
}

void red_black_insert(red_black_pointer *tree, int value)
{
  red_black_pointer *current;
  red_black_pointer parent;

  if (*tree == NULL) {
    new_root(tree, value);
  } else {
    if (is_four_node(*tree))
      split_root(*tree);

    current = tree;
    parent = NULL;
    while (1) {
      if (is_four_node(*current)) {
	/* split (change color or rotation) */
	if (get_node_type(parent) == two_node) {
	  split_child_of2(current, parent);
	} else if (get_node_type(parent) == three_node) {
	  split_child_of3(current, parent);
	}
	parent = (*current)->parent;
      }

      switch (compare(*current, value)) {
	case equal:
	  return;
	case leaf:
	  put_in(current, parent, value);
	  return;
	case lchild:
	  parent = *current;
	  current = &((*current)->left_child);
	  break;
	case rchild:
	  parent = *current;
	  current = &((*current)->right_child);
	  break;
      }
    }
  }
}

red_black_pointer red_black_search(red_black_pointer root, int key)
{
  red_black_pointer current;

  current = root;
  while (current) {
    if (current->value == key)
      return current;
    else if (key < current->value)
      current = current->left_child;
    else
      current = current->right_child;
  }

  return NULL;
}

int main()
{
  int test_data[] = {23, 1, 5, 6, 14, 53, 25, 41, 33, 2, 7};
  int i;
  red_black_pointer find;
  int key;

  for (i = 0; i < sizeof(test_data)/sizeof(int); i++) {
    red_black_insert (&root, test_data[i]);
  }

  for (i = 0; i < sizeof(test_data)/sizeof(int); i++) {
    key = test_data[i];
    find = red_black_search(root, key);
    if (find != NULL) {
      if (find->value == key)
	printf ("find %d\n", find->value);
    } else
      printf ("can not find\n");
  }

  key = 100;
  find = red_black_search(root, key);
  if (find != NULL) {
    if (find->value == key)
      printf ("find %d\n", find->value);
  } else
    printf ("can not find %d\n", key);

  return 0;
}
