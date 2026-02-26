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

  FILE *file = fopen(filepath, "r");
  if (!file) {
    free(current_module);
    return -1;
  }

  char line[1024]; 
  int line_number = 1;
  while (fgets(line, sizeof(line), file)) {

    const char *ptr = skip_whitespace(line);

    if (strncmp(ptr, "import ", 7) == 0) {
      ptr += 7;

      size_t len = strcspn(ptr, " \t\r\n,");
      if (len >= 256) {
        len = 255;
      }

      char module_name[256];
      strncpy(module_name, ptr, len);
      module_name[len] = '\0';

      int target_id = get_or_create_node(g, map, module_name);
      graph_add_edge(g, current_id, target_id, line_number);
    } else if (strncmp(ptr, "from ", 5) == 0) {
      ptr += 5;

      size_t len = strcspn(ptr, " \t\r\n,");
      if (len >= 256) {
        len = 255;
      }

      char raw_target[256];
      strncpy(raw_target, ptr, len);
      raw_target[len] = '\0';

      char final_target[512];

      if (raw_target[0] == '.') {
        int leading_dots = 0;
        while (raw_target[leading_dots] == '.') {
          leading_dots++;
        }

        char parent_pkg[256];
        strncpy(parent_pkg, current_module, 255);
        parent_pkg[255] = '\0';

        int is_init_py = (strstr(filepath, "__init__.py") != NULL);
        int levels_to_strip = is_init_py ? (leading_dots - 1) : leading_dots;

        while (levels_to_strip > 0 && parent_pkg[0] != '\0') {
          char *last_dot = strrchr(parent_pkg, '.');
          if (last_dot) {
            *last_dot = '\0';
          } else {
            parent_pkg[0] = '\0';
          }
          levels_to_strip--;
        }

        const char *remainder = raw_target + leading_dots;

        if (parent_pkg[0] != '\0' && remainder[0] != '\0') {
          snprintf(final_target, sizeof(final_target), "%s.%s", parent_pkg,
                   remainder);
        } else if (parent_pkg[0] != '\0') {
          snprintf(final_target, sizeof(final_target), "%s", parent_pkg);
        } else {
          snprintf(final_target, sizeof(final_target), "%s", remainder);
        }
      } else {
        strncpy(final_target, raw_target, sizeof(final_target) - 1);
        final_target[sizeof(final_target) - 1] = '\0';
      }

      int target_id = get_or_create_node(g, map, final_target);
      graph_add_edge(g, current_id, target_id, line_number);
    }

    line_number++;
  }

  free(current_module);
  fclose(file);
  return 0;
}
