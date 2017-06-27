#include <stdio.h>

#define MAX_VERTICES 1000
#define MAX_COST 100000

int vertex_n;

int cost[MAX_VERTICES][MAX_VERTICES];

void init_cost (int n)
{
  int i, j;

  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      if (i != j)
        cost[i][j] = MAX_COST;
      else
	cost[i][j] = 0;
    }
  }
}

void add_cost (int start, int end, int weight)
{
  cost[start][end] = weight;
}

void input (void)
{
  int edge_n;
  int start, end;
  int weight;

  scanf ("%d\n", &edge_n);
  scanf ("%d\n", &vertex_n);

  init_cost (vertex_n);

  while (edge_n) {
    scanf ("%d %d %d\n", &start, &end, &weight);
    add_cost (start, end, weight);
    edge_n--;
  }
}

void all_pair_shortest (void)
{
  int i, j, k;

  for (k = 0; k < vertex_n; k++) {
    for (i = 0; i < vertex_n; i++) {
      for (j = 0; j < vertex_n; j++) {
	if (cost[i][j] > cost[i][k] + cost[k][j])
	  cost[i][j] = cost[i][k] + cost[k][j];
      }
    }
  }
}

void print_all_pair (void)
{
  int i, j;

  for (i = 0; i < vertex_n; i++) {
    for (j = 0; j < vertex_n; j++) {
      if (cost[i][j] >= MAX_COST)
	printf ("     n/a");
      else
        printf ("%8d", cost[i][j]);
    }
    printf ("\n");
  }
  printf ("\n");
}

int main ()
{
  input ();
  all_pair_shortest ();
  print_all_pair ();

  return 0;
}
