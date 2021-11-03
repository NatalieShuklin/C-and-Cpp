/**
 * @brief Vector.c describes a generic vector library

 */

#include "Vector.h"
/**
 * @brief return value of failure in function VectorFind, VectorGetLoadFactor
 */
#define FAIL -1
/**
 * @brief return value of failure in function VectorPushBack
 */
#define FAIL_PUSH 0
/**
 * @brief one number of element for calloc use, and push back index use
 */
#define ONE_SIZE 1
/**
 * @brief initial vector size
 */
#define INIT 0
/**
 * @brief cmp equal value
 */
#define EQUALS 1
/**
 * @brief return value of success in function VectorPushBack
 */
#define SUCCESS_PUSH 1
/**
 * @brief return value of success in erase function
 */
#define SUCCESS_ERASE 1
/**
 * @brief return value of FAIL in erase function
 */
#define FAIL_ERASE 0

/**
 * Allocates dynamically new vector element.
 * @param elem_copy_func func which copies the element stored in the vector (returns
 * dynamically allocated copy).
 * @param elem_cmp_func func which is used to compare elements stored in the vector.
 * @param elem_free_func func which frees elements stored in the vector.
 * @return pointer to dynamically allocated vector.
 * @if_fail return NULL.
 */
Vector *VectorAlloc(VectorElemCpy elem_copy_func, VectorElemCmp
elem_cmp_func, VectorElemFree elem_free_func) {
  if (elem_cmp_func == NULL || elem_copy_func == NULL ||
      elem_free_func == NULL) {
    return NULL;
  }
  Vector *new_vector = (Vector *) calloc(ONE_SIZE, sizeof(Vector));
  if (new_vector == NULL) {
    return NULL;
  }
  new_vector->capacity = VECTOR_INITIAL_CAP;
  new_vector->size = INIT;
  new_vector->data = calloc(VECTOR_INITIAL_CAP, sizeof(void *));
  if (new_vector->data == NULL) {
    free(new_vector);
    return NULL;
  }
  new_vector->elem_cmp_func = elem_cmp_func;
  new_vector->elem_copy_func = elem_copy_func;
  new_vector->elem_free_func = elem_free_func;
  return new_vector;
}

/**
 * Frees a vector and the elements the vector itself allocated.
 * @param p_vector pointer to dynamically allocated pointer to vector.
 */
void VectorFree(Vector **p_vector) {
  if (p_vector != NULL && *p_vector != NULL) {
    for (size_t i = 0; i < (*p_vector)->size; i++) {

      (*p_vector)->elem_free_func(&((*p_vector)->data[i]));
      (*p_vector)->data[i] = NULL;
    }
    free((*p_vector)->data);
    (*p_vector)->data = NULL;
    free(*p_vector);
    *p_vector = NULL;
  }
}

/**
 * Returns the element at the given index.
 * @param vector pointer to a vector.
 * @param ind the index of the element we want to get.
 * @return the element the given index if exists (the element itself, not a copy of it)
 * , NULL otherwise.
 */
void *VectorAt(Vector *vector, size_t ind) {
  if (vector == NULL || vector->size <= ind) {
    return NULL;
  }
  return vector->data[ind];
}

/**
 * Gets a value and checks if the value is in the vector.
 * @param vector a pointer to vector.
 * @param value the value to look for.
 * @return the index of the given value if it is in the
 * vector ([0, vector_size - 1]).
 * Returns -1 if no such value in the vector.
 */
int VectorFind(Vector *vector, void *value) {
  if (vector == NULL || value == NULL) {
    return FAIL;
  }
  for (size_t i = 0; i < vector->size; i++) {
    if (vector->elem_cmp_func(value, vector->data[i]) == EQUALS) {
      return (int) i;
    }
  }
  return FAIL;
}

/**
 * Adds a new value to the back (index vector_size) of the vector.
 * @param vector a pointer to vector.
 * @param value the value to be added to the vector.
 * @return 1 if the adding has been done successfully, 0 otherwise.
 */
int VectorPushBack(Vector *vector, void *value) {
  if (vector == NULL || value == NULL) {
    return FAIL_PUSH;
  }
  vector->size++;
  if ((double) (vector->size) / vector->capacity > VECTOR_MAX_LOAD_FACTOR) {
    vector->capacity *= VECTOR_GROWTH_FACTOR;
    void **temp = realloc(vector->data, vector->capacity * (sizeof(void *)));
    vector->data = temp;
    for(size_t i = vector->capacity/VECTOR_GROWTH_FACTOR; i<vector->capacity;
    i++) {
      vector->data[i]=NULL;
    }
  }
  vector->data[vector->size - ONE_SIZE] = vector->elem_copy_func(value);
  return SUCCESS_PUSH;
}

/**
 * This function returns the load factor of the vector.
 * @param vector a vector.
 * @return the vector's load factor, -1 if the function failed.
 */
double VectorGetLoadFactor(Vector *vector) {
  if (vector == NULL || vector->capacity <= 0) {
    return FAIL;
  }
  return (double) vector->size / vector->capacity;
}

/**
 * Removes the element at the given index from the vector.
 * @param vector a pointer to vector.
 * @param ind the index of the element to be removed.
 * @return 1 if the removing has been done successfully, 0 otherwise.
 */
int VectorErase(Vector *vector, size_t ind) {
  if (vector == NULL || ind >= vector->size) {
    return FAIL_ERASE;
  }
  vector->elem_free_func(&((vector->data)[ind]));
  vector->data[ind] = NULL;
  for (size_t i = ind; i < vector->size - ONE_SIZE; i++) {
    vector->data[i] = vector->data[i + ONE_SIZE];
  }
  vector->data[vector->size - ONE_SIZE] = NULL;
  vector->size--;
  if ((double) (vector->size) / vector->capacity < VECTOR_MIN_LOAD_FACTOR) {
    vector->capacity /= VECTOR_GROWTH_FACTOR;
    void **temp =
        (void **) realloc(vector->data, vector->capacity * sizeof(void *));
    vector->data = temp;
  }
  return SUCCESS_ERASE;
}

/**
 * Deletes all the elements in the vector.
 * @param vector vector a pointer to vector.
 */
void VectorClear(Vector *vector) {
  if (vector != NULL) {
    size_t size = vector->size;
    size_t cap = vector->capacity;
    for (size_t i = 0; i < vector->capacity; i++) {
      if (vector->data[i] != NULL) {
        vector->elem_free_func(&((vector->data)[i]));
        size--;
        if ((double) (size) / cap < VECTOR_MIN_LOAD_FACTOR) {
          cap /= VECTOR_GROWTH_FACTOR;
        }
      }
    }
    vector->size = INIT;
    vector->capacity = cap;
  }
}


