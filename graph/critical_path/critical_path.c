#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 1000

typedef struct node *node_pointer;
struct node {
  int vertex;
  int duration;
  int early;
  int late;
  int critical;
  node_pointer next;
};
struct node_head {
  int degree;
  node_pointer next;
};

struct node_head graph[MAX_VERTICES];
struct node_head igraph[MAX_VERTICES];
int vertex_n;
int topological_order[MAX_VERTICES];
int topological_n;
int earliest[MAX_VERTICES];
int latest[MAX_VERTICES];
int stack_top;

static void init_graph (int n)
{
  int i;
  for (i = 0; i < n; i++) {
    graph[i].degree = 0;
    graph[i].next = NULL;
    earliest[i] = 0;
  }
}

static void add_edge (int start, int end, int duration)
{
  node_pointer *current;
  node_pointer new_node;

  current = &(graph[start].next);
  while (*current) {
    if ((*current)->vertex == end) {
      printf ("duplicated edge\n");
      return;
    }

    current = &((*current)->next);
  }

  new_node = malloc (sizeof (struct node));
  new_node->vertex = end;
  new_node->duration = duration;
  new_node->next = NULL;
  new_node->early = 0;
  new_node->late = 0;
  new_node->critical = 0;

  *current = new_node;

  /* increase in-degree */
  graph[end].degree++;
}

static void input (void)
{
  int edge_n;
  int start, end;
  int duration;
  
  scanf ("%d\n", &edge_n);
  scanf ("%d\n", &vertex_n);

  init_graph (vertex_n);

  while (edge_n) {
    scanf ("%d %d %d\n", &start, &end, &duration);
    add_edge (start, end, duration);
    edge_n--;
  }
}

static void push_stack (int vertex)
{
  graph[vertex].degree = stack_top;
  stack_top = vertex;
}

static int pop_stack (void)
{
  int ret = stack_top;
  stack_top = graph[stack_top].degree;

  return ret;
}

static void prepare_stack (void)
{
  int i;

  stack_top = -1;

  for (i = 0; i < vertex_n; i++) {
    if (graph[i].degree == 0) {
      push_stack (i);
    }
  }
}

static void topological_sort (void)
{
  node_pointer current;
  int top;

  topological_n = 0;
  while ((top = pop_stack()) != -1) {
    topological_order[topological_n++] = top;
    current = graph[top].next;
    while (current != NULL) {
      /* delete the edge */
      graph[current->vertex].degree--;
      /* update earliest[] */
      if (earliest[current->vertex] < earliest[top] + current->duration) {
	earliest[current->vertex] = earliest[top] + current->duration;
      }
      /* push the vertex without incoming edges */
      if (graph[current->vertex].degree == 0) {
	push_stack (current->vertex);
      }

      current = current->next;
    }
  }
}

static void inverse_graph (void)
{
  node_pointer current;
  node_pointer *update;
  node_pointer new_node;
  int i;

  for (i = 0; i < vertex_n; i++) {
    current = graph[i].next;
    while (current) {

      update = &(igraph[current->vertex].next);
      while (*update) {
	update = &((*update)->next);
      }
      new_node = malloc (sizeof (struct node));
      new_node->vertex = i;
      new_node->duration = current->duration;
      new_node->next = NULL;
      *update = new_node;

      current = current->next;
    }
  }
}

static void calc_latest (void)
{
  int i;
  node_pointer current;

  /* init latest[] */
  for (i = 0; i < vertex_n; i++) {
    latest[i] = earliest[vertex_n - 1];
  }

  inverse_graph ();

  /* calculate latest[] backward */
  for (i = topological_n - 1; i >= 0; i--) {
    current = igraph[i].next;
    while (current) {
      if (latest[current->vertex] > latest[i] - current->duration) {
	latest[current->vertex] = latest[i] - current->duration;
      }
      current = current->next;
    }
  }
}

static void calc_early_late (void)
{
  int i;
  node_pointer current;
  for (i = 0; i < vertex_n; i++) {
    current = graph[i].next;
    while (current) {
      current->early = earliest[i];
      current->late = latest[current->vertex] - current->duration;
      if (current->early == current->late)
	current->critical = 1;

      current = current->next;
    }
  }
}

static void output_topological_order (void)
{
  int i;
  printf ("topological order:\n");
  for (i = 0; i < topological_n; i++) {
    printf ("%d ", topological_order[i]);
  }
  printf ("\n");
}

static void output_critical_edges (void)
{
  int i;
  node_pointer current;

  printf ("critical edges:\n");
  for (i = 0; i < vertex_n; i++) {
    current = graph[i].next;
    while (current) {
      if (current->critical)
	printf ("%d %d\n", i, current->vertex);
      current = current->next;
    }
  }
}

static void output_earliest_latest (void)
{
  int i;
  printf ("vertex earliest:\n");
  for (i = 0; i < vertex_n; i++) {
    printf ("%d ", earliest[i]);
  }
  printf ("\n");

  printf ("vertex latest:\n");
  for (i = 0; i < vertex_n; i++) {
    printf ("%d ", latest[i]);
  }
  printf ("\n");
}

int main () 
{
  input ();
  prepare_stack ();
  /* get topological order and calculate vertex earliest */
  topological_sort ();
  /* calculate vertex latest */
  calc_latest ();
  /* calculate edges' early & late */
  calc_early_late ();

  output_topological_order ();
  output_earliest_latest ();
  output_critical_edges ();

  return 0;
}
