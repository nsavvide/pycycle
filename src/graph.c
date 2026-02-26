#include "../include/graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_cycle_trace(Graph *g, int *path_stack, int stack_depth,
                              int trigger_id) {
  printf("\n%s%s CIRCULAR DEPENDENCY DETECTED%s\n", STYLE_BOLD, COLOR_RED,
         COLOR_RESET);
  printf("%s--------------------------------%s\n", COLOR_RED, COLOR_RESET);

  bool start_printing = false;
  for (int i = 0; i < stack_depth; i++) {
    int node_id = path_stack[i];
    if (node_id == trigger_id)
      start_printing = true;

    if (start_printing) {
      Node *n = g->nodes[node_id];
      int next_id = (i + 1 < stack_depth) ? path_stack[i + 1] : trigger_id;
      int line = 0;

      for (Edge *e = n->edges; e; e = e->next) {
        if (e->target_id == next_id) {
          line = e->line_number;
          break;
        }
      }
      printf("  %s->%s %s%-20s%s %s(line [%d])%s\n", COLOR_RED, COLOR_RESET,
             STYLE_BOLD, n->name, COLOR_RESET, COLOR_YELLOW, line, COLOR_RESET);
    }
  }
  printf("  %s->%s %s%s%s %s(CLOSED LOOP)%s\n", COLOR_RED, COLOR_RESET,
         STYLE_BOLD, COLOR_CYAN, g->nodes[trigger_id]->name, COLOR_RED,
         COLOR_RESET);
  printf("%s--------------------------------%s\n", COLOR_RED, COLOR_RESET);
}

static void dfs_find_cycles(Graph *g, int node_id, int *path_stack, int depth) {
  Node *node = g->nodes[node_id];

  if (node->in_stack) {
    print_cycle_trace(g, path_stack, depth, node_id);
    return;
  }
  if (node->visited)
    return;

  node->visited = true;
  node->in_stack = true;
  path_stack[depth] = node_id;

  for (Edge *edge = node->edges; edge; edge = edge->next) {
    dfs_find_cycles(g, edge->target_id, path_stack, depth + 1);
  }

  node->in_stack = false; // Backtrack
}

/**
 * @brief Resizes the graph's nodes array when capacity is reached.
 * This function doubles the capacity of the nodes array and copies existing
 * nodes to the new array. It returns 0 on success and -1 on failure.
 * @param g Pointer to the Graph to resize.
 * @return 0 on success, -1 on failure.
 */
static int resize_graph(Graph *g) {
  if (g == NULL) {
    return -1;
  }

  size_t new_capacity = g->capacity * 2;
  Node **new_nodes = (Node **)realloc(g->nodes, new_capacity * sizeof(Node *));
  if (new_nodes == NULL) {
    return -1;
  }

  g->nodes = new_nodes;
  g->capacity = new_capacity;
  return 0;
}

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
}

int graph_add_node(Graph *g, const char *name) {
  if (g == NULL) {
    return -1;
  }

  if (*name == '\0') {
    return -1;
  }

  if (g->node_count >= g->capacity) {
    if (resize_graph(g) == -1) {
      return -1;
    }
  }

  Node *new_node = (Node *)calloc(1, sizeof(Node));
  if (new_node == NULL) {
    return -1;
  }

  new_node->name = strdup(name);
  if (new_node->name == NULL) {
    free(new_node);
    return -1;
  }

  g->nodes[g->node_count] = new_node;
  g->node_count++;

  return (int)(g->node_count - 1); // index 0
}

int graph_add_edge(Graph *g, int from_id, int to_id, int line_number) {
  if (from_id < 0 || from_id >= (int)g->node_count || to_id < 0 ||
      to_id >= (int)g->node_count) {
    return -1;
  }

  Node *from_node = g->nodes[from_id];
  Edge *new_edge = (Edge *)calloc(1, sizeof(Edge));
  if (new_edge == NULL) {
    return -1;
  }

  new_edge->target_id = to_id;
  new_edge->next = from_node->edges;
  new_edge->line_number = line_number;
  from_node->edges = new_edge;

  return 0;
}

void graph_find_cycles(Graph *g) {
  if (!g || g->node_count == 0)
    return;

  for (size_t i = 0; i < g->node_count; i++) {
    g->nodes[i]->visited = false;
    g->nodes[i]->in_stack = false;
  }

  int *path_stack = (int *)calloc(g->node_count, sizeof(int));
  if (!path_stack)
    return;

  for (size_t i = 0; i < g->node_count; i++) {
    if (!g->nodes[i]->visited) {
      dfs_find_cycles(g, (int)i, path_stack, 0);
    }
  }

  free(path_stack);
}

void graph_export_dot(Graph *g, const char *filename) {
  FILE *f = fopen(filename, "w");
  if (!f) {
    fprintf(stderr, "Error: Could not open %s for writing.\n", filename);
    return;
  }

  fprintf(f, "digraph PyCycle {\n");
  fprintf(f, "  rankdir=LR;\n");
  fprintf(f, "  node [shape=box, style=filled, fillcolor=lightgray];\n\n");

  for (size_t i = 0; i < g->node_count; i++) {
    Node *n = g->nodes[i];
    for (Edge *e = n->edges; e; e = e->next) {
      fprintf(f, "  \"%s\" -> \"%s\" [label=\"line %d\"];\n", n->name,
              g->nodes[e->target_id]->name, e->line_number);
    }
  }

  fprintf(f, "}\n");
  fclose(f);

  printf("\nGraph exported to \x1b[36m%s\x1b[0m\n", filename);
  printf("   Tip: Render it using '\x1b[33mdot -Tpng %s -o graph.png\x1b[0m'\n",
         filename);
}
