#ifndef PYCYCLE_GRAPH_H
#define PYCYCLE_GRAPH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

typedef struct Edge Edge;
typedef struct Node Node;
typedef struct Graph Graph;
typedef struct HashmapItem HashmapItem;

/**
 * @struct Edge
 * @brief Represents a single directed import from one module to another.
 */
struct Edge {
  int target_id; /**< The integer ID of the imported module */
  Edge *next; /**< Pointer to the next edge in the linked list (for chaining) */
};

/**
 * @struct Node
 * @brief Represents a single module in the graph, containing its name and a
 * linked list of outgoing edges.
 */
struct Node {
  char *name;   /**< The name of the module */
  Edge *edges;  /**< Pointer to the head of the linked list of outgoing edges */
  bool visited; /**< Flag for DFS: has this node been fully processed? */
  bool in_stack; /**< Flag for DFS: is this node in the current traversal path?
                  */
};

/**
 * @struct Graph
 * @brief Represents the entire graph of modules and their imports.
 */
struct Graph {
  Node **nodes;      /**< Dynamic array of pointers to Nodes */
  size_t node_count; /**< Current number of nodes in the graph */
  size_t capacity;   /**< Current capacity of the nodes array */
};

/**
 * @struct HashmapItem
 * @brief Represents a single entry in the hashmap for module name to ID
 * mapping.
 */
struct HashmapItem {
  char *key; /**< The module name */
  int value; /**< The corresponding node ID (integer) */
  HashmapItem
      *next; /**< Pointer to the next item in the bucket (for chaining) */
};

/**
 * @brief Allocates and initializes a new Graph.
 * @param initial_capacity The starting size of the node array.
 * @return Pointer to the allocated Graph, or NULL if memory fails.
 */
Graph *graph_create(size_t initial_capacity);

/**
 * @brief Safely frees all memory inside the graph (Nodes, Edges, and the
 * Graph itself).
 * @param g Pointer to the Graph.
 */
void graph_free(Graph *g);

#ifdef __cplusplus
}
#endif

#endif /* PYCYCLE_GRAPH_H */
