#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100
#define MAX_EDGES    1000

typedef struct node *node_pointer;
struct node {
  int weight;
  int marked;
  int vertex_a;
  int vertex_b;
  node_pointer next_a;
  node_pointer next_b;
};

node_pointer graph[MAX_VERTICES];
int vertex_n = 0;
node_pointer edge_heap[MAX_EDGES];
int edge_heap_size = 0;
int vertex_set[MAX_VERTICES];

static void add_edge_heap(node_pointer edge)
{
  edge_heap_size++;
  
  if (edge_heap_size == 1) {
    edge_heap[1] = edge;
    return;
  }

  edge_heap[edge_heap_size] = edge;

  int parent;
  int current = edge_heap_size;
  node_pointer tmp;

  while (current != 1) {
    parent = current / 2;
    if (edge_heap[parent]->weight > edge_heap[current]->weight) {
      tmp = edge_heap[parent];
      edge_heap[parent] = edge_heap[current];
      edge_heap[current] = tmp;
    } else
      break;

    current = parent;
  }
}

static node_pointer extract_min(void)
{
  if (edge_heap_size == 0)
    return NULL;

  node_pointer min_edge = edge_heap[1];
  node_pointer tmp = edge_heap[edge_heap_size];
  edge_heap_size--;

  int current = 1;
  int child = current * 2;

  while (child <= edge_heap_size) {
    if (child + 1 <= edge_heap_size) {
      if (edge_heap[child]->weight > edge_heap[child+1]->weight)
	child++;
    }

    if (edge_heap[child]->weight < tmp->weight)
      edge_heap[current] = edge_heap[child];
    else
      break;

    current = child;
    child = current * 2;
  }
  edge_heap[current] = tmp;

  return min_edge;
}

/* i and j must be root of trees. */
static void edge_union(int i, int j)
{
  if (i == j)
    return;

  int node_number = vertex_set[i] + vertex_set[j];

  if (vertex_set[i] < vertex_set[j]) { /* tree i has more nodes than tree j. */
    vertex_set[j] = i;
    vertex_set[i] = node_number;
  } else {
    vertex_set[i] = j;
    vertex_set[j] = node_number;
  }
}

static int edge_find(int i)
{
  int current = i;
  int root;
  int next;

  while (vertex_set[current] > 0) {
    current = vertex_set[current];
  }
  root = current;

  current = i;
  while (current != root) {
    next = vertex_set[current];
    vertex_set[current] = root;
    current = next;
  }

  return root;
}

static void init_graph(void)
{
  int i;
  for (i = 0; i < MAX_VERTICES; i++) {
    graph[i] = NULL;
    vertex_set[i] = -1;
  }
}

static void swap(int *start, int *end)
{
  int tmp;

  if (*start > *end) {
    tmp = *start;
    *start = *end;
    *end = tmp;
  }
}

static int find_edge(int start, int end)
{
  swap(&start, &end);

  node_pointer current = graph[start];

  while (current) {
    if ((current->vertex_a == start) && (current->vertex_b == end))
      return 1;
    else if (current->vertex_a == start)
      current = current->next_a;
    else if (current->vertex_b == start)
      current = current->next_b;
  }

  return 0;
}

static void add_edge(int start, int end, int weight)
{
  swap(&start, &end);

  if (find_edge(start, end))
    return;

  /* create new edge */
  node_pointer new_edge = malloc(sizeof(struct node));
  new_edge->weight = weight;
  new_edge->vertex_a = start;
  new_edge->vertex_b = end;
  new_edge->marked = 0;
  new_edge->next_a = NULL;
  new_edge->next_b = NULL;

  node_pointer *current = &graph[start];
  if (*current == NULL)
    vertex_n++;

  while (*current) {
    if ((*current)->vertex_a == start)
      current = &((*current)->next_a);
    else if ((*current)->vertex_b == start)
      current = &((*current)->next_b);
  }
  *current = new_edge;

  current = &graph[end];
  if (*current == NULL)
    vertex_n++;

  while (*current) {
    if ((*current)->vertex_a == end)
      current = &((*current)->next_a);
    else if ((*current)->vertex_b == end)
      current = &((*current)->next_b);
  }
  *current = new_edge;
}

static void input(void)
{
  int nedge;
  int start, end;
  int weight;

  scanf("%d\n", &nedge);

  while (nedge) {
    scanf("%d %d %d\n", &start, &end, &weight);
    add_edge(start, end, weight);
    nedge--;
  }
}

node_pointer mst[MAX_EDGES];
int mst_n = 0;

static int is_cycle(node_pointer edge)
{
  int set_a;
  int set_b;

  set_a = edge_find(edge->vertex_a);
  set_b = edge_find(edge->vertex_b);
  if (set_a == set_b)
    return 1;

  return 0;
}

static int next_vertex(int root, node_pointer edge)
{
  int set_root;
  int set_a;
  int set_b;

  set_root = edge_find(root);
  set_a = edge_find(edge->vertex_a);
  set_b = edge_find(edge->vertex_b);

  if (set_root != set_a)
    return edge->vertex_a;
  else
    return edge->vertex_b;
}

static void add_to_set(node_pointer edge)
{
  int set_a;
  int set_b;

  set_a = edge_find(edge->vertex_a);
  set_b = edge_find(edge->vertex_b);

  edge_union(set_a, set_b);
}

static void prim_mst(int start_vertex)
{
  int new_vertex = start_vertex;
  node_pointer min_edge;
  node_pointer edge;

  do {
    /* add candidates to heap */
    edge = graph[new_vertex];
    while (edge != NULL) {
      if (edge->marked == 0) {
        add_edge_heap (edge);
	edge->marked = 1;
      }

      if (edge->vertex_a == new_vertex)
	edge = edge->next_a;
      else if (edge->vertex_b == new_vertex)
	edge = edge->next_b;
    }

    /* extract min edge */
    while (1) {
      min_edge = extract_min ();
      if (min_edge == NULL) {
	printf ("There is no minimum spanning tree.\n");
	return;
      }

      if (is_cycle (min_edge) == 0) {
	/* add to minimum spanning tree */
	mst[mst_n++] = min_edge;
	new_vertex = next_vertex(start_vertex, min_edge);
	add_to_set (min_edge);
	break;
      }
    }

  } while (mst_n != vertex_n - 1);
}

static void print_mst(void)
{
  int i;
  for (i = 0; i < mst_n; i++) {
    printf ("%d %d %d\n", mst[i]->vertex_a, mst[i]->vertex_b, mst[i]->weight);
  }
  printf ("\n");
}

int main()
{
  init_graph();
  input();
  prim_mst(0);
  print_mst();

  return 0;
}
