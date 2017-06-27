#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 1000

typedef struct node *node_pointer;
struct node {
  int vertex;
  node_pointer next;
};

struct node_head {
  int degree;
  node_pointer next;
};

struct node_head graph[MAX_VERTICES];
int vertex_n;

void init_graph (int n)
{
  int i;

  for (i = 0; i < n; i++) {
    graph[i].degree = 0;
    graph[i].next = NULL;
  }
}

int stack_top = -1;

void push_to_stack (int vertex)
{
  graph[vertex].degree = stack_top;
  stack_top = vertex;
}

int pop_from_stack (void)
{
  int ret = stack_top;
  if (ret == -1)
    return ret;

  stack_top = graph[stack_top].degree;

  return ret;
}

void add_edge (int start, int end)
{
  node_pointer *current;

  current = &(graph[start].next);
  while (*current) {
    current = &((*current)->next);
  }
  node_pointer new_node = malloc (sizeof (struct node));
  new_node->vertex = end;
  new_node->next = NULL;
  *current = new_node;

  graph[end].degree++;
}

void input (void)
{
  int edge_n;
  int start, end;

  scanf ("%d\n", &edge_n);
  scanf ("%d\n", &vertex_n);

  init_graph (vertex_n);

  while (edge_n) {
    scanf ("%d %d\n", &start, &end);
    add_edge (start, end);
    edge_n--;
  }
}

void prepare_top_sort (void)
{
  int i;

  for (i = 0; i < vertex_n; i++) {
    if (graph[i].degree == 0) {
      push_to_stack (i);
    }
  }
}

void topology_sort (void)
{
  node_pointer current;
  int ret;
  while ((ret = pop_from_stack ()) != -1) {
    printf ("%d ", ret);
    /* delete all edges from current vertex */
    current = graph[ret].next;
    while (current != NULL) {
      graph[current->vertex].degree--;
      if (graph[current->vertex].degree == 0) {
	push_to_stack (current->vertex);
      }
      current = current->next;
    }
  }
  printf ("\n");
}

int main ()
{
  input ();
  prepare_top_sort ();
  topology_sort ();

  return 0;
}
