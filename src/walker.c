#include "../include/walker.h"
#include "../include/lexer.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

/**
 * @brief Robustly checks if a filename ends with a specific extension.
 * @param filename The name of the file (e.g., "utils.py" or ".gitignore")
 * @param ext The extension to check for, including the dot (e.g., ".py")
 * @return true if it matches, false otherwise.
 */
static bool has_extension(const char *filename, const char *ext) {
  if (!filename || !ext)
    return false;

  const char *dot = strrchr(filename, '.');

  if (!dot)
    return false;

  if (dot == filename)
    return false;

  return strcmp(dot, ext) == 0;
}

int walk_directory(const char *directory, const char *base_dir, Graph *g,
                   Hashmap *map) {
  DIR *dir = opendir(directory);
  if (!dir) {
    return -1;
  }

  struct dirent *entry;

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);

    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
      continue;
    }

    if (S_ISDIR(path_stat.st_mode)) {
      if (walk_directory(path, base_dir, g, map) == -1) {
        continue;
      }

    } else if (S_ISREG(path_stat.st_mode)) {
      if (has_extension(entry->d_name, ".py")) {
        if (process_python_file(path, base_dir, g, map) == -1) {
          fprintf(stderr, "Error processing file: %s\n", path);
        }
      }
    }
  }

  closedir(dir);
  return 0;
}
