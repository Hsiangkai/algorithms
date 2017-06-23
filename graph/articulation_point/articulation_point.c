#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_VERTICES 100

typedef struct node *node_pointer;
struct node {
  int marked;
  int vertex_a;
  int vertex_b;
  node_pointer next_a;
  node_pointer next_b;
};

node_pointer graph[MAX_VERTICES];
int n;

static void swap(int *start, int *end)
{
  if (*start < *end)
    return;

  int tmp = *start;
  *start = *end;
  *end = tmp;
}

static int find_edge(int start, int end)
{
  node_pointer current;

  swap(&start, &end);
  current = graph[start];

  while (current) {
    if ((current->vertex_a == start) && (current->vertex_b == end))
      return 1;
    else if (current->vertex_a == start)
      current = current->next_a;
    else if (current->vertex_b == start)
      current = current->next_b;
    else
      return 0;
  }

  return 0;
}

static void add_edge(int start, int end)
{
  node_pointer *current;
  node_pointer new_node;

  swap(&start, &end);

  if (find_edge(start, end) == 1)
    return;

  new_node = malloc(sizeof(struct node));
  new_node->marked = 0;
  new_node->vertex_a = start;
  new_node->vertex_b = end;
  new_node->next_a = NULL;
  new_node->next_b = NULL;

  current = &(graph[start]);
  while (*current) {
    if ((*current)->vertex_a == start) {
      current = &((*current)->next_a);
    } else if ((*current)->vertex_b == start) {
      current = &((*current)->next_b);
    } else {
      assert("should not be here.");
      return;
    }
  }
  *current = new_node;

  current = &(graph[end]);
  while (*current) {
    if ((*current)->vertex_a == end) {
      current = &((*current)->next_a);
    } else if ((*current)->vertex_b == end) {
      current = &((*current)->next_b);
    } else {
      assert("should not be here.");
      return;
    }
  }
  *current = new_node;
}

static void input(void)
{
  int num;
  int start, end;

  scanf ("%d\n", &num);

  while (num) {
    scanf ("%d %d\n", &start, &end);
    add_edge(start, end);
    num--;
  }
}

int visited[MAX_VERTICES];
int low[MAX_VERTICES];
int dfn[MAX_VERTICES];
int articulation[MAX_VERTICES];
int articulation_n;
int num;

static void init_graph(void)
{
  int i;
  
  for (i = 0; i < MAX_VERTICES; i++) {
    graph[i] = NULL;
    visited[i] = 0;
    low[i] = -1;
    dfn[i] = -1;
  }
  num = 0;
}

static void clean_graph(void)
{
  int i;
  node_pointer current;

  for (i = 0; i < MAX_VERTICES; i++) {
    if (graph[i] != NULL) {
      current = graph[i];
      while (current) {
	if (current->marked != 0)
	  current->marked = 0;

	if (i == current->vertex_a)
	  current = current->next_a;
	if (i == current->vertex_b)
	  current = current->next_b;
      }
    }
  }
}

static void print(int vertex)
{
  printf ("%d ", vertex);
}

static int min(int a, int b)
{
  return (a < b) ? a : b;
}

static void dfs(int vertex, void (*action)(int))
{
  int i;

  if (graph[vertex] == NULL)
    return;

  visited[vertex] = 1;
  /* updae dfn & low */
  dfn[vertex] = num;
  low[vertex] = num;
  num++;
  action(vertex);

  node_pointer current = graph[vertex];
  while (current) {
    if (current->marked == 0) {
      current->marked = 1;
      if (current->vertex_a == vertex) {
	if (visited[current->vertex_b] == 0) {
	  dfs(current->vertex_b, action);
	  /* Update low after visiting children. */
	  low[vertex] = min(low[vertex], low[current->vertex_b]);
	  /* DFS will visit children before current node.
	   * After return from recursive call, the low value of children
	   * is determined. */
	  if (low[current->vertex_b] >= dfn[vertex]) {
	    /* add to articulation points */
	    for (i = 0; i < articulation_n; i++) {
	      if (articulation[i] == vertex)
		break;
	    }
	    if (i == articulation_n)
	      articulation[articulation_n++] = vertex;
	  }
	} else { /* back edge */
	  /* If visiting back edge, updaate low according to DFN of children. */
	  low[vertex] = min(low[vertex], dfn[current->vertex_b]);
	}
      } else if (current->vertex_b == vertex) {
	if (visited[current->vertex_a] == 0) {
	  dfs(current->vertex_a, action);
	  /* Update low after visiting children. */
	  low[vertex] = min(low[vertex], low[current->vertex_a]);
	  if (low[current->vertex_a] >= dfn[vertex]) {
	    /* add to articulation points */
	    for (i = 0; i < articulation_n; i++) {
	      if (articulation[i] == vertex)
		break;
	    }
	    if (i == articulation_n)
	      articulation[articulation_n++] = vertex;
	  }
	} else { /* back edge */
	  /* If visiting back edge, updaate low according to DFN of children. */
	  low[vertex] = min(low[vertex], dfn[current->vertex_a]);
	}
      }
    }

    if (current->vertex_a == vertex)
      current = current->next_a;
    else if (current->vertex_b == vertex)
      current = current->next_b;
  }
}

int main()
{
  init_graph();
  input();

  dfs(3, print);
  printf("\n");

  int i;
  printf ("depth first number: ");
  for (i = 0; i <  num; i++) {
    printf ("%d ", dfn[i]);
  }
  printf ("\n");
  printf ("low value: ");
  for (i = 0; i <  num; i++) {
    printf ("%d ", low[i]);
  }
  printf ("\n");
  printf ("articulation: ");
  for (i = 0; i < articulation_n; i++) {
    printf ("%d ", articulation[i]);
  }
  printf ("\n");

  return 0;
}
