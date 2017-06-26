#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 100
#define MAX_COST 10000

static int vertex_n;
static int cost[MAX_VERTICES][MAX_VERTICES];
static int found[MAX_VERTICES];
static int dist[MAX_VERTICES];

static void init_graph(void)
{
  int i, j;

  vertex_n = 0;
  for (i = 0; i < MAX_VERTICES; i++) {
    for (j = 0; j < MAX_VERTICES; j++) {
      cost[i][j] = MAX_COST;
      if (i == j)
	cost[i][j] = 0;
    }
  }
}

static void add_edge(int start, int end, int weight)
{
  cost[start][end] = weight;
}

static void input(void)
{
  int edge_n;
  int start, end;
  int weight;

  scanf ("%d\n", &edge_n);
  scanf ("%d\n", &vertex_n);

  while (edge_n) {
    scanf ("%d %d %d\n", &start, &end, &weight);
    add_edge (start, end, weight);
    edge_n--;
  }
}

static int found_next(void)
{
  int i;
  int min_vertex = -1;
  int min_distance = INT_MAX;
  
  for (i = 0; i < vertex_n; i++) {
    if (found[i] == 0) {
      if (dist[i] < min_distance) {
	min_distance = dist[i];
	min_vertex = i;
      }
    }
  }

  return min_vertex;
}

static void dijkstra_shortest_path(int start_vertex)
{
  int i, j;
  int next_vertex;

  /* init*/
  for (i = 0; i < vertex_n; i++) {
    found[i] = 0;
    dist[i] = cost[start_vertex][i];
  }

  found[start_vertex] = 1;
  dist[start_vertex] = 0;

  for (i = 0; i < vertex_n - 1; i++) {
    next_vertex = found_next ();
    if (next_vertex == -1)
      break;

    found[next_vertex] = 1;

    /* Update distance for other vertices. */
    for (j = 0; j < vertex_n; j++) {
      if (found[j] == 0) {
	if (dist[j] > dist[next_vertex] + cost[next_vertex][j]) {
	  dist[j] = dist[next_vertex] + cost[next_vertex][j];
	}
      }
    }
  }
}

int main()
{
  init_graph ();
  input ();
  dijkstra_shortest_path (0);

  int i;
  for (i = 0; i < vertex_n; i++) {
    printf ("%d %d %d\n", 0, i, dist[i]);
  }

  return 0;
}
