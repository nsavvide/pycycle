#include "../include/graph.h"
#include "../include/hashmap.h"
#include "../include/walker.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <python_project_directory>\n", argv[0]);
    return 1;
  }

  const char *target_dir = argv[1];

  Graph *g = graph_create(1024);
  Hashmap *map = hashmap_create(1024);

  if (!g || !map) {
    fprintf(stderr, "Critical: Memory allocation failed during startup.\n");
    return 1;
  }

  printf("Starting PyCycle Analysis...\n");
  printf("Target Directory: %s\n", target_dir);

  if (walk_directory(target_dir, target_dir, g, map) != 0) {
    fprintf(stderr, "Fatal: Could not access directory: %s\n", target_dir);
    graph_free(g);
    hashmap_free(map);
    return 1;
  }

  printf("Modules Found: %zu\n", g->node_count);
  printf("Searching for cycles...\n");

  graph_find_cycles(g);

  printf("\nAnalysis complete.\n");

  graph_free(g);
  hashmap_free(map);

  return 0;
}
