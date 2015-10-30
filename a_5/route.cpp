#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#define INT_MAX 2147483647
using namespace std;

void print_graph(int *ADJ, int V) {
  int i, j;
  for (i = 0; i < V; i++) {
    for (j = 0; j < V; j++) {
      cout << ADJ[V*i+j] << " ";
    }
    cout << endl;
  }
}

/* Djikstra's Algorithm Implementation */
int *djikstra(int *ADJ, int source, int V) {

}

/* Bellman Ford Algorithm Implementation
 * To be completed */
int bellman_ford() {

}

int main() {
  int i, j, c, source;
  static int *ADJ = NULL, *OUT; /* Adjacency matrix */
  int V = 5;              /* Vertex count */
  ADJ = new int[5*5];
  srand(time(NULL));
  /* Random graph generation */
  for (i = 0; i < V; i++) {
    for (j = 0; j < V; j++) {
      if (i==j) 
        ADJ[V*i+j] = 0;
      else
        ADJ[V*i+j] = rand()%20;
    }
  }

  cout << "Place your choice for routing protocol algorithm" << endl;
  cout << "1 -  Djikstra, 2 - Bellman Ford" << endl;
  cin >> c;
  cout << " Define the source point: " << endl;
  cin >> source;

  switch(c) {
    case 1: OUT = djikstra(ADJ, V, source);
            break;
    case 2: /* TODO */
            break;
    default:
  }
  return 0;
}
