#ifndef PYCYCLE_GRAPH_H
#define PYCYCLE_GRAPH_H

#ifdef __cplusplus
extern "C" {
#endif

#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_RESET "\x1b[0m"
#define STYLE_BOLD "\x1b[1m"

#include <stdbool.h>
#include <stddef.h>

typedef struct Edge Edge;
typedef struct Node Node;
typedef struct Graph Graph;

/**
 * @struct Edge
 * @brief Represents a single directed import from one module to another.
 */
struct Edge {
  int target_id;   /**< The integer ID of the imported module */
  int line_number; /**< The line number in the source file where this import
                      occurs */
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

/**
 * @brief Adds a new node with the given name to the graph.
 * @param g Pointer to the Graph.
 * @param name The name of the module to add as a node.
 * @return The integer ID of the newly added node, or -1 on failure.
 */
int graph_add_node(Graph *g, const char *name);

/**
 * @brief Adds a directed edge from one node to another in the graph.
 * @param g Pointer to the Graph.
 * @param from_id The integer ID of the source node (the importer).
 * @param to_id The integer ID of the target node (the imported module).
 * @param line_number The line number in the source file where this import
 * occurs.
 * @return 0 on success, or -1 on failure (e.g., invalid IDs).
 */
int graph_add_edge(Graph *g, int from_id, int to_id, int line_number);

/**
 * @brief Traverses the graph to find and print all circular dependencies.
 * @param g Pointer to the Graph.
 */
void graph_find_cycles(Graph *g);

#ifdef __cplusplus
}
#endif

#endif /* PYCYCLE_GRAPH_H */
