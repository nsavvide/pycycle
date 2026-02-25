#include "../include/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Skips leading spaces and tabs on a line of text.
 */
static const char *skip_whitespace(const char *line) {
  while (*line == ' ' || *line == '\t') {
    line++;
  }
  return line;
}

/**
 * @brief Converts "src/app/models.py" -> "app.models"
 */
static char *filepath_to_modulename(const char *filepath,
                                    const char *base_dir) {
  const char *relative_path = filepath;
  size_t base_len = strlen(base_dir);
  if (strncmp(filepath, base_dir, base_len) == 0) {
    relative_path += base_len;
    while (*relative_path == '/' || *relative_path == '\\')
      relative_path++;
  }

  char *module_name = strdup(relative_path);
  if (!module_name)
    return NULL;

  char *ext = strstr(module_name, ".py");
  if (ext)
    *ext = '\0';

  char *init = strstr(module_name, "/__init__");
  if (!init)
    init = strstr(module_name, "\\__init__");
  if (init)
    *init = '\0';

  for (int i = 0; module_name[i] != '\0'; i++) {
    if (module_name[i] == '/' || module_name[i] == '\\') {
      module_name[i] = '.';
    }
  }

  return module_name;
}

/**
 * @brief Safely gets or creates a Node ID from the Hashmap & Graph.
 */
static int get_or_create_node(Graph *g, Hashmap *map, const char *module_name) {
  int id = hashmap_get(map, module_name);
  if (id == -1) {
    id = graph_add_node(g, module_name);
    hashmap_put(map, module_name, id);
  }
  return id;
}

int process_python_file(const char *filepath, const char *base_dir, Graph *g,
                        Hashmap *map) {
  char *current_module = filepath_to_modulename(filepath, base_dir);
  if (!current_module)
    return -1;

  int current_id = get_or_create_node(g, map, current_module);
  free(current_module);

  FILE *file = fopen(filepath, "r");
  if (!file) {
    return -1;
  }

  char line[1024]; // 1024 chars is more than enough for a Python import line
  int line_number = 1;
  while (fgets(line, sizeof(line), file)) {

    const char *ptr = skip_whitespace(line);

    // CHECK 1: Is it an "import X" statement?
    if (strncmp(ptr, "import ", 7) == 0) {
      ptr += 7; // Skip the word "import "

      /* TODO: Handle multiple imports in one line (e.g., "import os, sys") and
       * "import X as Y" cases. */
      size_t len = strcspn(ptr, " \t\r\n,");
      if (len >= 256) {
        len = 255; // Prevent buffer overflow
      }

      char module_name[256];
      strncpy(module_name, ptr, len);
      module_name[len] = '\0';

      int target_id = get_or_create_node(g, map, module_name);
      graph_add_edge(g, current_id, target_id, line_number);
    }
    // CHECK 2: Is it a "from Y import Z" statement?
    else if (strncmp(ptr, "from ", 5) == 0) {
      ptr += 5; // Skip the word "from "

      size_t len = strcspn(ptr, " \t\r\n,");
      if (len >= 256) {
        len = 255; // Prevent buffer overflow
      }

      char module_name[256];
      strncpy(module_name, ptr, len);
      module_name[len] = '\0';

      int target_id = get_or_create_node(g, map, module_name);
      graph_add_edge(g, current_id, target_id, line_number);

      line_number++;
    }
  }

  fclose(file);
  return 0;
}
