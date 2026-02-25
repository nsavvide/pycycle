#ifndef PYCYCLE_WALKER_H
#define PYCYCLE_WALKER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "graph.h"
#include "hashmap.h"

/**
 * @brief Recursively walks a directory, finding all .py files and passing them to the Lexer.
 * @param directory The current directory path being scanned.
 * @param base_dir The root directory of the project.
 * @param g Pointer to the Graph.
 * @param map Pointer to the Hashmap.
 * @return 0 on success, -1 on failure.
 */
int walk_directory(const char *directory, const char *base_dir, Graph *g, Hashmap *map);

#ifdef __cplusplus
}
#endif

#endif /* PYCYCLE_WALKER_H */
