#include "../include/hashmap.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief The djb2 hashing algorithm by Dan Bernstein.
 * @param str The string to hash.
 * @return A large unsigned integer representing the string.
 */
static unsigned long hash_string(const char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }
  return hash;
}

Hashmap *hashmap_create(size_t capacity) {
  Hashmap *map = (Hashmap *)calloc(1, sizeof(Hashmap));
  if (map == NULL) {
    return NULL;
  }

  map->buckets = (HashmapItem **)calloc(capacity, sizeof(HashmapItem *));
  if (map->buckets == NULL) {
    free(map);
    return NULL;
  }

  map->capacity = capacity;

  return map;
}

/**
 * @brief Resizes the hashmap when the load factor exceeds a certain threshold.
 * This function doubles the capacity of the hashmap and rehashes all existing
 * items into the new buckets. It returns 0 on success and -1 on failure.
 * @param h Pointer to the Hashmap to resize.
 * @return 0 on success, -1 on failure.
 */
static int resize_hashmap(Hashmap *h) {
  if (h == NULL)
    return -1;

  size_t new_capacity = h->capacity * 2;
  HashmapItem **new_buckets =
      (HashmapItem **)calloc(new_capacity, sizeof(HashmapItem *));
  if (new_buckets == NULL)
    return -1;

  for (size_t i = 0; i < h->capacity; i++) {
    HashmapItem *item = h->buckets[i];
    while (item) {
      HashmapItem *next_item = item->next;

      unsigned long hash = hash_string(item->key);
      size_t new_index = hash % new_capacity;

      item->next = new_buckets[new_index];
      new_buckets[new_index] = item;

      item = next_item;
    }
  }

  free(h->buckets);
  h->buckets = new_buckets;
  h->capacity = new_capacity;

  return 0;
}

void hashmap_free(Hashmap *map) {
  if (map == NULL) {
    return;
  }

  for (size_t i = 0; i < map->capacity; i++) {
    HashmapItem *item = map->buckets[i];
    while (item) {
      HashmapItem *next_item = item->next;
      free(item->key);
      free(item);
      item = next_item;
    }
  }

  free(map->buckets);
  free(map);
}

int hashmap_put(Hashmap *map, const char *key, int value) {
  if (map == NULL || key == NULL)
    return -1;

  if (map->count >= map->capacity) {
    if (resize_hashmap(map) == -1)
      return -1;
  }

  unsigned long hash = hash_string(key);
  size_t index = hash % map->capacity;

  HashmapItem *new_item = (HashmapItem *)calloc(1, sizeof(HashmapItem));
  if (new_item == NULL)
    return -1;

  new_item->key = strdup(key);
  if (new_item->key == NULL) {
    free(new_item);
    return -1;
  }
  new_item->value = value;

  new_item->next = map->buckets[index];
  map->buckets[index] = new_item;

  map->count++;

  return 0;
}

int hashmap_get(Hashmap *map, const char *key) {
  if (map == NULL || key == NULL) {
    return -1;
  }

  unsigned long hash = hash_string(key);
  size_t index = hash % map->capacity;

  HashmapItem *item = map->buckets[index];
  while (item) {
    if (strcmp(item->key, key) == 0) {
      return item->value;
    }
    item = item->next;
  }

  return -1;
}
