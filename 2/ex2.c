
 * ex2.c represents a double linked list creation and process


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "ex2.h"
#include <string.h>

/**
 * @brief error message for incorrect list input
 */
#define ERROR_INPUT_LIST "ERROR: The LinkedList input is not valid.\n"
/**
 * @brief error message for incorrect node input
 */
#define ERROR_INPUT_NODE "ERROR: The Node input is not valid.\n"
/**
 * @brief error message for memory allocation error for Node
 */
#define ERROR_ALLOC_NODE "ERROR: Memory allocation error occurred for Node"\
".\n"
/**
 * @brief error message for memory allocation error for data of a node
 */
#define ERROR_ALLOC_DATA "ERROR: Memory allocation error occurred for the "\
"data of a node.\n"
/**
 * @brief error message for memory allocation error for LinkedList
 */
#define ERROR_ALLOC_LIST "ERROR: Memory allocation error occurred "\
"for the linked list.\n"
/**
 * @brief error message for memory allocation error for the grade averages array
 */
#define ERROR_ALLOC_ARR "ERROR: Memory allocation error occurred "\
"for the array of averages.\n"
/**
 * @brief error message for invalid input file
 */
#define ERROR_FILE_OPEN "ERROR: Invalid input file, check it's name or "\
"path.\n"
/**
 * @brief error message for invalid file name
 */
#define ERROR_FILE_NAME "ERROR: The file name is invalid.\n"
/**
 * @brief max length of a line in a file is 1024
 */
#define MAX_LINE_LEN 1025
/**
 * @brief indicates an start of list
 */
#define START_LIST "s"
/**
 * @brief indicates an end of list
 */
#define END_LIST "e"
/**
 * @brief represents the comma delimiter for input parsing
 */
#define COMMA_DELIM ","
/**
 * @brief error message for invalid num_of_elements pointer in avg. function
 */
#define ERROR_NUM_ELEM "ERROR: The num of elements pointer is invalid.\n"
/**
 * @brief new line char in windows
 */
#define NEWLINE_WIN "\n"
/**
 * @brief new line char in linux
 */
#define NEWLINE_LINUX "\r\n"

/**
 * Adds a node as the head of the list
 * Assumptions:
    * You cannot assume the pointers are valid

 * In case of errors:
    * Invalid pointer - print error to stderr and return from function.

 * @param list pointer to list to add a node to
 * @param node pointer to the node to add to the list
 */
void AddToStartLinkedList(LinkedList *const list, Node *const node) {
  //check input pointers are valid
  if (list == NULL) {
    fprintf(stderr, ERROR_INPUT_LIST);
    return;
  }
  if (node == NULL) {
    fprintf(stderr, ERROR_INPUT_NODE);
    return;
  }
  Node *temp = list->head;
  list->head = node;
  node->next = temp;
  if (temp != NULL) {
    temp->prev = node;
  }
  if (list->tail == NULL) {
    list->tail = node;
  }
}

/**
 * Adds a node as the tail of the list
 *
 * Assumptions:
    * You cannot assume the pointers are valid

 * In case of errors:
    * Invalid pointer - print error to stderr and return from function.

 * @param list pointer to list to add a node to
 * @param node pointer to the node to add to the list
 */
void AddToEndLinkedList(LinkedList *const list, Node *const node) {
  //check input pointers are valid
  if (list == NULL) {
    fprintf(stderr, ERROR_INPUT_LIST);
    return;
  }
  if (node == NULL) {
    fprintf(stderr, ERROR_INPUT_NODE);
    return;
  }
  Node *temp = list->tail;
  list->tail = node;
  if (temp != NULL) {
    temp->next = node;
  }
  node->prev = temp;
  if (list->head == NULL) {
    list->head = node;
  }
}

/**
 * removes a node from the list and frees it's resources
 *
 * Assumptions:
    * You cannot assume the pointers are valid
    * You cannot assume the node is in the list. In this case don't free its
    * resources.
    * To be sure - you are supposed to compare the given node to the nodes in
    * the list using == operator

 * In case of errors:
    * Invalid pointer - print informative error message to stderr and return
    * from function.

 * @param list pointer to list to remove a node from
 * @param node pointer to the node to remove from the list
 */
void RemoveNode(LinkedList *const list, Node *const node) {
  bool found = false;
  if (list == NULL) {
    fprintf(stderr, ERROR_INPUT_LIST);
    return;
  }
  if (node == NULL) {
    fprintf(stderr, ERROR_INPUT_NODE);
    return;
  }
  Node *temp = list->head;
  while (temp != NULL) {
    if (temp == node) {
      found = true;
      break;
    }
    temp = temp->next;
  }
  if (found == false) {
    return;
  }
  // If node to be deleted is head node
  if (list->head == node)
    list->head = node->next;
  if (list->tail == node)
    list->tail = node->prev;
  // Change next only if node to be deleted is NOT the last node
  if (node->next != NULL)
    node->next->prev = node->prev;
  // Change prev only if node to be deleted is NOT the first node
  if (node->prev != NULL)
    node->prev->next = node->next;
  free(node->data);
  node->data = NULL;
  free(node);
}

/**
 * Frees the resources (all dynamic allocations) of the given list.
 *
 * Assumptions:
    * You cannot assume the pointer is valid

 * In case of errors:
    * Invalid pointer - This means there is nothing to free, just return
    * from function.
    * Node is not in the list - Return from the function. Don't free its
    * resources!

 * @param list the list to free.
 */
void FreeLinkedList(LinkedList *const list) {
  if (list == NULL) {
    return;
  }
  Node *temp = list->head;
  Node *next;
  while (temp != NULL) {
    if (temp->data != NULL) {
      free(temp->data);
      temp->data = NULL;
    }
    next = temp->next;
    free(temp);
    temp = NULL;
    temp = next;
  }
  list->head = NULL;
  list->tail = NULL;
  free(list);
}

/**
 * @brief calculates the sum of data array of a node in the linked list
 * @param node -the node to calculate the data sum on
 * @return the sum of the data array of the node
 */
int GetSumOfDataArr(Node *const node) {
  int sum = 0;
  if (node != NULL && node->data != NULL) {
    for (unsigned long i = 0; i < node->len; i++) {
      sum += node->data[i];
    }
  }
  return sum;
}

/**
 * calculates the grade average of every node in the linked list
 * @param list: LinkedList pointer to linked list from whom to calculate
 * grade averages
 * @param num_elements_in_returned_array: pointer to unsigned long, that
 * should contain the number of nodes in the list
 * (for nodes with data != NULL) when the function finishes execution. To
 * be clear - at the start of the function,
 * (*num_elements_in_returned_array) is a garbage value, and this function
 * should set its value.
 * Note: Nodes with NULL data should not be count in the calculation.
 * Therefore *num_elements_in_returned_array <= numOfNodes.
 *
 * Assumptions:
    * You cannot assume the pointers are valid
    * You don't know the number of nodes in the linkedlist in advance
    * You can assume that there is at least one node with data != NULL
    * (i.e. len > 0)
    * You can assume that for each node 'len' is indeed the length of 'data'
    *  (in particular len=0 <=> data=NULL).
 * In case of errors:
    * Invalid pointer - print informative message to stderr, free resources
    * allocated by function, return NULL from function.
    * Allocation fail - print informative message to stderr, free resources
    * allocated by function, return NULL from function.
    * Node with NULL data - don't count the node in the calculation.
 * @return array of grade averages per node. To be clear - this means you
 * should calculate the grade average of every node and save it into an
 * array, which you will return from the function
 */
double *GetAverages(LinkedList *const list,
                    size_t *const num_elements_in_returned_array) {
  if (list == NULL) {
    fprintf(stderr, ERROR_INPUT_LIST);
    return NULL;
  }
  if (num_elements_in_returned_array == NULL) {
    fprintf(stderr, ERROR_NUM_ELEM);
    return NULL;
  }
  unsigned long capacity_array = 1;
  int data_sum = 0;
  Node *temp = list->head;
  unsigned long arr_index = 0;
  unsigned long count_averages = 0;
  double *average_arr = (double *) calloc(1, sizeof(double));
  if (average_arr == NULL) {
    fprintf(stderr, ERROR_ALLOC_ARR);
    return NULL;
  }
  while (temp != NULL) {
    data_sum = GetSumOfDataArr(temp);
    if (temp->len != 0) {
      count_averages++;
      if (count_averages > capacity_array) {
        capacity_array++;
        double *prev_averages = average_arr;
        average_arr = (double *) realloc(average_arr, sizeof(double) *
            capacity_array);
        if (average_arr == NULL) {
          fprintf(stderr, ERROR_ALLOC_ARR);
          free(prev_averages);
          prev_averages = NULL;
          return NULL;
        }
      }
      average_arr[arr_index] = (double) data_sum / (double) temp->len;
      arr_index++;
    }
    temp = temp->next;
  }
  *num_elements_in_returned_array = count_averages;
  return average_arr;
}

/**
 * @brief verifies if file exists and opens it
 * @param filename - the name of the file to be opened
 * @return the opened file, NULL if an error occurred and informative message
 */
FILE *CheckFileInput(const char *const filename) {
  if (filename == NULL) {
    fprintf(stderr, ERROR_FILE_NAME);
    return NULL;
  }
  FILE *input = fopen(filename, "r");
  if (input == NULL) {
    fprintf(stderr, ERROR_FILE_OPEN);
    return NULL;
  }
  return input;
}

/**
 * @brief allocates memory for newNode
 * @return the new allocated Node
 */
Node *AllocateNode() {
  Node *new_node = (Node *) calloc(1, sizeof(Node));
  if (new_node == NULL) {
    fprintf(stderr, ERROR_ALLOC_NODE);
    return NULL;
  }
  return new_node;
}

/**
 * @brief creates new Linked list, allocates memory for it
 * @return the linked list that was allocated, else NULL if an error
 * occurred
 */
LinkedList *CreateList() {
  LinkedList *list = (LinkedList *) calloc(1, sizeof(LinkedList));
  if (list == NULL) {
    fprintf(stderr, ERROR_ALLOC_LIST);
    return NULL;
  }
  return list;
}

/**
 * @brief parses line by line of the program input file
 * saves everything into the current list
 * allocates nodes and their data
 * build the Linked list correctly, by parsing each line into Node
 * then adding it to the Linked List
 * @param input the input file to parse
 * @param list the list to add the nodes to
 * @return the updated list, NULL if an memory allocation error occurred
 * frees memory allocared by it
 */
LinkedList *ParseFile(FILE *input, LinkedList *list) {
  char *token;
  char line[1025];
  bool start = false;
  bool end = false;
  unsigned long capacity_tokens;
  unsigned long data_index;
  while (fgets(line, MAX_LINE_LEN, input) != NULL) {
    data_index = 0;
    capacity_tokens = 1;
    Node *node = AllocateNode();
    if (node == NULL) {
      return NULL;
    }
    token = strtok(line, COMMA_DELIM);
    if (strcmp(token, START_LIST) == 0) {
      start = true;
      end = false;
    }
    if (strcmp(token, END_LIST) == 0) {
      end = true;
      start = false;
    }
    token = strtok(NULL, COMMA_DELIM);
    if (token != NULL && strcmp(token, NEWLINE_LINUX) != 0
        && strcmp(token, NEWLINE_WIN) != 0) {
      node->data = (int *) calloc(1, sizeof(int));
      if (node->data == NULL) {
        free(node);
        fprintf(stderr, ERROR_ALLOC_DATA);
        return NULL;
      }
    }
    while (token != NULL && strcmp(token, NEWLINE_LINUX) != 0
        && strcmp(token, NEWLINE_WIN) != 0) {
      int grade = (int) strtol(token, NULL, 10);
      if (node->len == capacity_tokens) {
        capacity_tokens++;
        int *prev_data = node->data;
        node->data = (int *) realloc(node->data, capacity_tokens * sizeof(int));
        if (node->data == NULL) {
          fprintf(stderr, ERROR_ALLOC_DATA);
          free(node);
          node = NULL;
          free(prev_data);
          prev_data = NULL;
          return NULL;
        }
      }
      node->data[data_index] = grade;
      data_index++;
      token = strtok(NULL, COMMA_DELIM);
      node->len++;
    }
    if (start == true) {
      AddToStartLinkedList(list, node);
    }
    if (end == true) {
      AddToEndLinkedList(list, node);
    }
  }
  return list;
}

/**
 * opens a file from a given filename and parses it's contents into a
 * LinkedList.
 *
 * Assumptions:
     * You cannot assume the filename is OK (meaning - you cannot assume it is
     * a valid path to a file).
     * You cannot assume anything on the number of ints in a row of the file,
     * just that it is >= 1
     * You can assume that if the file opened, then it is exactly in the format
     * specified in the exercise PDF.
     * You can assume max line length of 1024.
 *
 * In case of errors:
    * Invalid pointer - print informative message to stderr, free resources
    * allocated by function, return NULL from function.
    * File open fail - print informative message to stderr, free resources
    * allocated by function, return NULL from function.
    * Allocation fail - print informative message to stderr,
    * free resources allocated by function, return NULL from function.
 * Think - in what mode should we open this file? (read? write? append?)
 *
 * @param file filename of input file that needs to be parsed
 * @return pointer to LinkedList instance, whose nodes and data arrays were
 * parsed from the given file.
 */
LinkedList *ParseLinkedList(const char *const filename) {
  FILE *input = CheckFileInput(filename);
  if (input == NULL) {
    return NULL;
  }
  LinkedList *list = CreateList();
  if (list == NULL) {
    fclose(input);
    return NULL;
  }
  if (ParseFile(input, list) == NULL) {
    FreeLinkedList(list);
    fclose(input);
    return NULL;
  }
  fclose(input);
  return list;
}