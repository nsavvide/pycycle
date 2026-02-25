#ifndef PYCYCLE_LEXER_H
#define PYCYCLE_LEXER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "graph.h"
#include "hashmap.h"

/**
 * @brief Reads a Python file, extracts its imports, and updates the graph.
 * @param filepath The full path to the .py file (e.g., "src/app/main.py")
 * @param base_dir The root directory being scanned (e.g., "src/")
 * @param g Pointer to the Graph.
 * @param map Pointer to the Hashmap.
 * @return 0 on success, -1 on failure.
 */
int process_python_file(const char *filepath, const char *base_dir, Graph *g, Hashmap *map);

#ifdef __cplusplus
}
#endif

#endif /* PYCYCLE_LEXER_H */
