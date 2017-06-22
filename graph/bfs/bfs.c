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

static void init_graph(void)
{
  int i;
  
  for (i = 0; i < MAX_VERTICES; i++) {
    graph[i] = NULL;
  }
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

int visited[MAX_VERTICES];

static void print(int vertex)
{
  printf ("%d ", vertex);
}

int queue[MAX_VERTICES];
int head = 0;
int tail = 0;

static void enqueue(int in)
{
  if (((tail + 1) % MAX_VERTICES) == head)
    return;

  queue[tail] = in;
  tail++;
  if (tail >= MAX_VERTICES)
    tail = 0;
}

static int dequeue(void)
{
  if (head == tail)
    return -1;

  int out = queue[head];
  head++;
  if (head == MAX_VERTICES)
    head = 0;

  return out;
}

static void bfs(int vertex, void (*action)(int))
{
  node_pointer current;
  int current_vertex;

  if (graph[vertex]) {
    action(vertex);
    enqueue(vertex);
    visited[vertex] = 1;
  }

  while ((current_vertex = dequeue()) != -1) {
    current = graph[current_vertex];
    while (current) {
      if (!current->marked) {
	if (current->vertex_a == current_vertex) {
	  if (!visited[current->vertex_b]) {
	    action(current->vertex_b);
	    enqueue(current->vertex_b);
	    visited[current->vertex_b] = 1;
	  }
	} else if (current->vertex_b == current_vertex) {
	  if (!visited[current->vertex_a]) {
	    action(current->vertex_a);
	    enqueue(current->vertex_a);
	    visited[current->vertex_a] = 1;
	  }
	}
	current->marked = 1;
      }

      if (current->vertex_a == current_vertex)
	current = current->next_a;
      else if (current->vertex_b == current_vertex)
	current = current->next_b;
    }
  }
}

int main()
{
  init_graph();
  input();

  bfs(0, print);
  printf("\n");

  return 0;
}

