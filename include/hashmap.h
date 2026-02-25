#ifndef PYCYCLE_HASHMAP_H
#define PYCYCLE_HASHMAP_H

#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Hashmap Hashmap;
typedef struct HashmapItem HashmapItem;

/**
 * @struct HashmapItem
 * @brief Represents a single key-value pair in the hashmap.
 * Uses a linked list node structure to handle hash collisions (Separate
 * Chaining).
 */
struct HashmapItem {
  char *key;         /**< The module name (e.g., "app.models.user") */
  int value;         /**< The corresponding Graph Node ID */
  HashmapItem *next; /**< Pointer to the next item in the same bucket */
};

/**
 * @struct Hashmap
 * @brief The registry mapping Python module string names to integer IDs.
 */
struct Hashmap {
  HashmapItem **buckets; /**< Array of pointers to HashmapItems (the buckets) */
  size_t capacity;       /**< Total number of buckets allocated */
  size_t count;          /**< Total number of items stored in the map */
};

/**
 * @brief Allocates and initializes a new Hashmap.
 * @param capacity The number of buckets (should ideally be a prime number).
 * @return Pointer to the allocated Hashmap, or NULL if memory fails.
 */
Hashmap *hashmap_create(size_t capacity);

/**
 * @brief Safely frees the hashmap, including all duplicated string keys and
 * items.
 * @param map Pointer to the Hashmap.
 */
void hashmap_free(Hashmap *map);

/**
 * @brief Inserts a new key-value pair into the hashmap.
 * @param map Pointer to the Hashmap.
 * @param key The module name string to insert.
 * @param value The Graph Node ID associated with this module.
 * @return 0 on success, -1 on memory allocation failure or invalid inputs.
 */
int hashmap_put(Hashmap *map, const char *key, int value);

/**
 * @brief Retrieves the integer ID for a given module name.
 * @param map Pointer to the Hashmap.
 * @param key The module name string to look up.
 * @return The integer ID if found, or -1 if the key does not exist.
 */
int hashmap_get(Hashmap *map, const char *key);

#ifdef __cplusplus
}
#endif

#endif // PYCYCLE_HASHMAP_H
