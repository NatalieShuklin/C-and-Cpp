/**
 * @brief represents a generic HashMap data structure


#include "HashMap.h"
#include "Pair.h"
#include "Vector.h"

/**
 * @brief one number of element for calloc use, and push back index use
 */
#define ONE_SIZE 1
/**
 * @brief initial vector size
 */
#define INIT 0
/**
 * @brief return value of success in erase function
 */
#define SUCCESS_ERASE 1
/**
 * @brief return value of FAIL in erase function
 */
#define FAIL_ERASE 0
/**
 * @brief return value of FAIL in  function
 */
#define FAILURE 0
/**
 * @brief return value of failure in function VectorPushBack
 */
#define FAIL_PUSH 0
/**
 * @brief return value of contained value in map
 */
#define CONTAINS 1
/**
 * @brief return value of not contained value in map
 */
#define NOT_CONTAINS 0
/**
 * @brief return value of successful process in function
 */
#define SUCCESS 1
/**
 * @brief return value of failed process in function
 */
#define FAILED 0
/**
 * @brief return value of failed load function
 */
#define FAILED_LOAD -1
/**
 * @brief non positive number validation
 */
#define IS_NON_POS 0

/**
 * Allocates dynamically new hash map element.
 * @param hash_func a function which "hashes" keys.
 * @param pair_cpy a function which copies pairs.
 * @param pair_cmp a function which compares pairs.
 * @param pair_free a function which frees pairs.
 * @return pointer to dynamically allocated HashMap.
 * @if_fail return NULL.
 */
HashMap *HashMapAlloc(
    HashFunc hash_func, HashMapPairCpy pair_cpy,
    HashMapPairCmp pair_cmp, HashMapPairFree pair_free) {
  if (hash_func == NULL || pair_cmp == NULL || pair_free == NULL || pair_cpy
      == NULL) {
    return NULL;
  }
  HashMap *new_hashmap = (HashMap *) calloc(ONE_SIZE, sizeof(HashMap));
  if (new_hashmap == NULL) {
    return NULL;
  }
  new_hashmap->size = INIT;
  new_hashmap->capacity = HASH_MAP_INITIAL_CAP;
  new_hashmap->hash_func = hash_func;
  new_hashmap->pair_cmp = pair_cmp;
  new_hashmap->pair_cpy = pair_cpy;
  new_hashmap->pair_free = pair_free;
  new_hashmap->buckets = (Vector **) calloc(HASH_MAP_INITIAL_CAP, sizeof
      (Vector *));
  if (new_hashmap->buckets == NULL) {
    free(new_hashmap);
    return NULL;
  }
  return new_hashmap;
}

/**
 * Frees a vector and the elements the vector itself allocated.
 * @param p_hash_map pointer to dynamically allocated pointer to hash_map.
 */
void HashMapFree(HashMap **p_hash_map) {
  if (p_hash_map != NULL && *p_hash_map != NULL) {
    for (size_t i = 0; i < (*p_hash_map)->capacity; i++) {
      VectorFree(&(((*p_hash_map)->buckets)[i]));
      (*p_hash_map)->buckets[i] = NULL;

    }
    free((*p_hash_map)->buckets);
    (*p_hash_map)->buckets = NULL;
    free((*p_hash_map));
    (*p_hash_map) = NULL;
  }
}

/**
 * @brief function rehashes the hash map after erase performed
 * @param hash_map the hash map to rehash on
 * @return the rehashed map
 */
HashMap *RehashAfterErase(HashMap *hash_map) {
  for (size_t i = 0; i < hash_map->capacity * HASH_MAP_GROWTH_FACTOR; i++) {
    if (hash_map->buckets[i] != NULL) {
      for (size_t j = 0; j < hash_map->buckets[i]->size; j++) {
        if (hash_map->buckets[i]->data != NULL) {
          void *temp = hash_map->pair_cpy(VectorAt(hash_map->buckets[i], j));
          if (VectorErase(hash_map->buckets[i], j) == FAIL_ERASE) {
            return FAILURE;
          }
          Pair *p = (Pair *) temp;
          size_t index =
              (hash_map->hash_func(p->key)) & (hash_map->capacity - ONE_SIZE);
          if (hash_map->buckets[index] == NULL) {
            Vector *new_vec = VectorAlloc(hash_map->pair_cpy,
                                          hash_map->pair_cmp,
                                          hash_map->pair_free);
            if (new_vec == NULL) {
              return FAILURE;
            }
            hash_map->buckets[index] = new_vec;
            VectorPushBack(new_vec, p);
            hash_map->pair_free(&temp);
          } else {
            VectorPushBack(hash_map->buckets[index], p);
            hash_map->pair_free(&temp);
          }
        }
      }
    }
  }
  return hash_map;
}

/**
 * @brief function rehashes if needed after insert performed into hash table
 * @param hash_map the hash map
 * @return the updated map
 */
HashMap *RehashAfterInsert(HashMap *hash_map) {
  for (size_t i = 0; i < hash_map->capacity / HASH_MAP_GROWTH_FACTOR; i++) {
    if (hash_map->buckets[i] != NULL) {
      for (size_t j = 0; j < hash_map->buckets[i]->size; j++) {
        if (hash_map->buckets[i]->data != NULL) {
          void *temp = hash_map->pair_cpy(VectorAt(hash_map->buckets[i], j));
          VectorErase(hash_map->buckets[i], j);
          Pair *p = (Pair *) temp;
          size_t
              index = (hash_map->hash_func(p->key)) & (hash_map->capacity - 1);
          if (hash_map->buckets[index] == NULL) {
            Vector
                *new_vec = VectorAlloc(hash_map->pair_cpy, hash_map->pair_cmp,
                                       hash_map->pair_free);
            hash_map->buckets[index] = new_vec;
            VectorPushBack(new_vec, p);
            hash_map->pair_free(&temp);
          } else {
            VectorPushBack(hash_map->buckets[index], p);
            hash_map->pair_free(&temp);
          }
        }
      }
    }
  }
  return hash_map;
}

/**
 * The function checks if the given key exists in the hash map.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return 1 if the key is in the hash map, 0 otherwise.
 */
int HashMapContainsKey(HashMap *hash_map, KeyT key) {
  for (size_t i = 0; i < hash_map->capacity; i++) {
    if (hash_map->buckets[i] != NULL) {
      for (size_t j = 0; j < hash_map->buckets[i]->size; j++) {
        if (hash_map->buckets[i]->data[j] != NULL) {
          Pair new = *(Pair *) hash_map->buckets[i]->data[j];
          if (new.key_cmp(key, new.key) == CONTAINS) {
            return CONTAINS;
          }
        }
      }
    }
  }
  return NOT_CONTAINS;
}

/**
 * @brief Overrides a pair in map, when inserting a same key pair
 * @param hash_map the map
 * @param pair the pair inserted
 * @return 1 if succeded override, otherwise 0
 */
int OverridePair(HashMap *hash_map, Pair *pair) {
  Pair *new_pair = hash_map->pair_cpy(pair);
  for (size_t i = 0; i < hash_map->capacity; i++) {
    if (hash_map->buckets[i] != NULL) {
      for (size_t j = 0; j < hash_map->buckets[i]->size; j++) {
        void *temp = VectorAt(hash_map->buckets[i], j);
        Pair *p = (Pair *) temp;
        if (p->key_cmp(p->key, pair->key) == CONTAINS) {
          hash_map->pair_free(&temp);
          temp = NULL;
          hash_map->buckets[i]->data[j] = new_pair;
          return SUCCESS;
        }
      }
    }
  }
  return FAILED;
}

/**
 * Inserts a new pair to the hash map.
 * The function inserts *new*, *copied*, *dynamically allocated* pair,
 * NOT the pair it receives as a parameter.
 * @param hash_map the hash map to be inserted with new element.
 * @param pair a pair the hash map would contain.
 * @return returns 1 for successful insertion, 0 otherwise.
 */
int HashMapInsert(HashMap *hash_map, Pair *pair) {
  if (hash_map == NULL || pair == NULL) {
    return FAILED;
  }
  if (HashMapContainsKey(hash_map, pair->key) == CONTAINS) {
    return OverridePair(hash_map, pair);
  }
  size_t index = (hash_map->hash_func(pair->key)) & (hash_map->capacity - ONE_SIZE);
  if (hash_map->buckets[index] == NULL) {
    Vector *new_vec = VectorAlloc(hash_map->pair_cpy, hash_map->pair_cmp,
                                  hash_map->pair_free);
    hash_map->buckets[index] = new_vec;
    VectorPushBack(new_vec, pair);
  } else {
    VectorPushBack(hash_map->buckets[index], pair);
  }
  hash_map->size++;
  if ((double) hash_map->size / hash_map->capacity >
      HASH_MAP_MAX_LOAD_FACTOR) {
    hash_map->capacity *= HASH_MAP_GROWTH_FACTOR;
      Vector **temp = realloc(hash_map->buckets, hash_map->capacity * (sizeof
              (Vector *)));
      hash_map->buckets = temp;
      for(size_t i = hash_map->capacity/HASH_MAP_GROWTH_FACTOR; i<hash_map->capacity; i++){
          hash_map->buckets[i]=NULL;
      }
    if (RehashAfterInsert(hash_map) == NULL) {
      return FAILED;
    }
  }
  return SUCCESS;
}

/**
 * The function checks if the given value exists in the hash map.
 * @param hash_map a hash map.
 * @param value the value to be checked.
 * @return 1 if the value is in the hash map, 0 otherwise.
 */
int HashMapContainsValue(HashMap *hash_map, ValueT value) {
  for (size_t i = 0; i < hash_map->capacity; i++) {
    if (hash_map->buckets[i] != NULL) {
      for (size_t j = 0; j < hash_map->buckets[i]->size; j++) {
        if (hash_map->buckets[i]->data[j] != NULL) {
          Pair new = *(Pair *) hash_map->buckets[i]->data[j];
          if (new.value_cmp(value, new.value) == CONTAINS) {
            return CONTAINS;
          }
        }
      }
    }
  }
  return NOT_CONTAINS;
}

/**
 * The function returns the value associated with the given key.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return the value associated with key if exists, NULL otherwise.
 */
ValueT HashMapAt(HashMap *hash_map, KeyT key) {
  for (size_t i = 0; i < hash_map->capacity; i++) {
    if (hash_map->buckets[i] != NULL) {
      for (size_t j = 0; j < hash_map->buckets[i]->size; j++) {
        if (hash_map->buckets[i]->data[j] != NULL) {
          Pair new = *(Pair *) hash_map->buckets[i]->data[j];
          if (new.key_cmp(key, new.key) == CONTAINS) {
            return new.value;
          }
        }
      }
    }
  }
  return NULL;
}

/**
 * The function erases the pair associated with key.
 * @param hash_map a hash map.
 * @param key a key of the pair to be erased.
 * @return 1 if the erasing was done successfully, 0 otherwise.
 */
int HashMapErase(HashMap *hash_map, KeyT key) {
  if (hash_map == NULL || key == NULL
      || HashMapContainsKey(hash_map, key) == 0) {
    return FAILED;
  }
  for (size_t i = 0; i < hash_map->capacity; i++) {
    if (hash_map->buckets[i] != NULL) {
      for (size_t j = 0; j < hash_map->buckets[i]->size; j++) {
        void *temp = VectorAt(hash_map->buckets[i], j);
        Pair *p = (Pair *) temp;
        if (p->key_cmp(key, p->key) == CONTAINS) {
          VectorErase(hash_map->buckets[i], j);
          hash_map->size--;
          if ((double) hash_map->size / hash_map->capacity <
              HASH_MAP_MIN_LOAD_FACTOR) {
            hash_map->capacity /= HASH_MAP_GROWTH_FACTOR;
            if (RehashAfterErase(hash_map) == NULL) {
              return FAILED;
            }
            for (size_t m = hash_map->capacity;
                 m < hash_map->capacity * VECTOR_GROWTH_FACTOR; m++) {
              VectorFree(&(hash_map->buckets[m]));
              hash_map->buckets[m] = NULL;
            }
            Vector **vec_array = realloc(hash_map->buckets, hash_map->capacity *
                (sizeof(Vector *)));
            hash_map->buckets = vec_array;
          }
          return SUCCESS;
        }
      }
    }
  }
  return FAILED;
}

/**
 * This function returns the load factor of the hash map.
 * @param hash_map a hash map.
 * @return the hash map's load factor, -1 if the function failed.
 */
double HashMapGetLoadFactor(HashMap *hash_map) {
  if (hash_map == NULL || hash_map->capacity <= IS_NON_POS) {
    return FAILED_LOAD;
  }
  return (double) hash_map->size / hash_map->capacity;
}

/**
 * This function deletes all the elements in the hash map.
 * @param hash_map a hash map to be cleared.
 */
void HashMapClear(HashMap *hash_map) {
  if (hash_map != NULL) {
    size_t capacity = hash_map->capacity;
    for (size_t i = 0; i < hash_map->capacity; i++) {
      if (hash_map->buckets[i] != NULL) {
        for (size_t j = 0; j < hash_map->buckets[i]->size; j++) {
          hash_map->size--;
          if ((double) hash_map->size / capacity
              < HASH_MAP_MIN_LOAD_FACTOR) {
            capacity /= HASH_MAP_GROWTH_FACTOR;
          }
          VectorClear(hash_map->buckets[i]);
        }
      }
    }
    for (size_t m = capacity; m < hash_map->capacity; m++) {
      VectorFree(&((hash_map->buckets)[m]));
    }
    hash_map->capacity = capacity;
    hash_map->size = INIT;
  }
}