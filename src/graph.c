#include "../include/graph.h"
#include <stdlib.h>

Graph *graph_create(size_t initial_capacity) {
  Graph *graph = (Graph *)calloc(1, sizeof(Graph));
  if (!graph) {
    return NULL;
  }

  graph->nodes = (Node **)calloc(initial_capacity, sizeof(Node *));
  if (!graph->nodes) {
    free(graph);
    return NULL;
  }

  graph->node_count = 0;
  graph->capacity = initial_capacity;

  return graph;
}

void graph_free(Graph *g) {
  if (g == NULL) {
    return;
  }

  for (size_t i = 0; i < g->node_count; i++) {
    Node *node = g->nodes[i];
    if (node) {
      free(node->name);
      Edge *edge = node->edges;
      while (edge) {
        Edge *next_edge = edge->next;
        free(edge);
        edge = next_edge;
      }
      free(node);
    }
  }

  free(g->nodes);
  free(g);
  g = NULL;
}
