#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#define INT_MAX 2147483647
using namespace std;

/* Debug Function */
void print_graph(int *ADJ, int V) {
  int i, j;
  cout << "Printing" << endl;
  for (i = 0; i < V; i++) {
    for (j = 0; j < V; j++) {
      cout << ADJ[V*i+j] << " ";
    }
    cout << endl;
  }
}

/* Helper Function for Djikstra */
int get_small_index(int *arr, int V) {
  int idx = 0, small = INT_MAX-1, i;
  for (i = 0; i < V; i++) {
    if(arr[i] < small) {
      small = arr[i];
      idx = i;
    }
  }
  return idx;
}

/* Djikstra's Algorithm Implementation */
int *djikstra(int *ADJ, int source, int V) {
  int *DIST, *TENT, *FIN, i;
  int count = 0, small_idx = 0;
  DIST = new int[V];
  TENT = new int[V];
  FIN = new int[V];
  for (i = 0; i < V; i++) {
    DIST[i] = INT_MAX-1;
    TENT[i] = INT_MAX-1;
    FIN[i] = INT_MAX-1;
  }
  DIST[source] = 0;
  TENT[source] = 0;
  FIN[source] = 0;
  while(count<V) {
    small_idx = get_small_index(TENT, V);
    for (i = 0; i < V; i++) {
      /* Process only if there is a finite solution */
      if (ADJ[V*small_idx+i]>0 && ADJ[V*small_idx+i]!=INT_MAX) {
        if (DIST[i] > DIST[small_idx] + ADJ[V*small_idx+i]) {
          DIST[i] = DIST[small_idx] + ADJ[V*small_idx+i];
          TENT[i] = DIST[i];
        }
      }
    }
    /* Updating the final distance matrix */
    FIN[small_idx] = DIST[small_idx];

    /* So that this node will not be visited again */
    TENT[small_idx] = INT_MAX;
    count++;
  }
  return FIN;
}

/* Bellman Ford Algorithm Implementation
 * From Biki's Laptop
 * To be completed */
int *bellman_ford(int *ADJ, int source, int V) {
  int i, j, k;
  int *TMP, *DIST, *FIN;
  TMP = new int[V];
  DIST = new int[V];
  FIN = new int[V];
  /*Step 1*/
  for (i = 0; i < V; i++) {
    DIST[i] = INT_MAX;
    TMP[i] = 0;
  }
  DIST[source] = 0;

  for (k = 0; k < V; k++) {
    for (i = 0; i < V; i++) {
      for (j = 0; j < V; j++) {
        if (ADJ[V*i+j]!=0 && ADJ[V*i+j]!=INT_MAX) {
          if (DIST[j] > DIST[i] + ADJ[V*i+j]) {
            DIST[j] = DIST[i] + ADJ[V*i+j];
          }
        }
      }
    }
  }

  for (i = 0; i < V; i++) {
    for (j = 0; j < V; j++) {
      if (DIST[i] > DIST[j] + ADJ[V*i+j]) {
        cout << "Presence of a negative cycle" << endl;
        exit(0);
      }
    }
  }

  return DIST;
}

int main() {
  int i, j, c, source;
  static int *ADJ = NULL, *OUT; /* Adjacency matrix */
  int V = 5;              /* Vertex count */
  ADJ = new int[5*5];
  srand(time(NULL));
  /* Random graph generation */
  for (i=0; i < V; i++) {
    for (j=0; j < V; j++) {
      ADJ[V*i+j] = 0;
    }
  }
  for (i = 0; i < V; i++) {
    for (j = i+1; j < V; j++) {
      ADJ[V*i+j] = rand()%20+1;
    }
  }
  for (i = 0; i<V; i++) {
    for(j = 0; j < i; j++) {
      ADJ[V*i+j] = ADJ[V*j+i];
    }
  }

  cout << "Place your choice for routing protocol algorithm" << endl;
  cout << "1 -  Djikstra, 2 - Bellman Ford" << endl;
  cin >> c;
  cout << " Define the source point: " << endl;
  cin >> source;

  print_graph(ADJ, V);
  switch(c) {
    case 1: OUT = djikstra(ADJ, source, V);
            for (i = 0; i < V; i++) {
              cout << OUT[i] << endl;
            }
            break;
    case 2: /* TODO */
            OUT = bellman_ford(ADJ, source, V);
            for (i = 0; i < V; i++) {
              cout << OUT[i] << endl; 
            }
            break;
  }
  return 0;
}
