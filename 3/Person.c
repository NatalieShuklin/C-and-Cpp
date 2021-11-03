/**
 * @brief describes a person

 */
#include "Person.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief defines one size for calloc
 */
#define ONE 1
/**
 * @brief defines zero for sizes of arrays check
 */
#define ZERO 1
/**
 * @brief defines return value in functions
 */
#define RETURN_ONE 1
/**
 * @brief defines return value in functions, if equality occurrs
 */
#define RETURN_EQUALS 0
/**
 * @brief defines return value in functions
 */
#define RETURN_MINUS_ONE -1

/**
 * Allocates dynamically new person with (at least) the following
 * input data:
 * @param id (IdT) the id of the person.
 * @param name (char *) the name of the person (received as string) (ex: "Moshe
 * Stam-shem").
 * @param age (size_t) the age of the person.
 * @param is_sick (int) boolean value (0/1) which indicates if the person is
 * sick.
 * @return (struct Person *) pointer to dynamically allocated person,
 * @if_fails returns NULL.
 * @assumption the input would be valid.
 */
Person *PersonAlloc(IdT id, char *name, size_t age, int is_sick) {
  Person *new_person = (Person *) calloc(ONE, sizeof(Person));
  if (new_person == NULL) {
    return NULL;
  }
  new_person->name = (char *) calloc(strlen(name) + ONE, sizeof(char));
  if (new_person->name == NULL) {
    return NULL;
  }
  strcpy(new_person->name, name);
  new_person->id = id;
  new_person->age = age;
  new_person->is_sick = is_sick;
  new_person->meetings = (Meeting **) calloc(PERSON_INITIAL_SIZE, sizeof
      (Meeting *));
  if (new_person->meetings == NULL) {
    return NULL;
  }
  new_person->num_of_meetings = 0;
  new_person->meetings_capacity = PERSON_INITIAL_SIZE;
  new_person->infection_rate = 0;
  return new_person;
}

/**
 * Frees everything the person has allocated and the pointer itself.
 * @param p_person (struct Person **) pointer to dynamically allocated person.
 * @assumption you can not assume anything.
 */
void PersonFree(Person **p_person) {
  if (p_person != NULL) {
    if (*p_person != NULL) {
      free((*p_person)->name);
      (*p_person)->name = NULL;
      free((*p_person)->meetings);
      (*p_person)->meetings = NULL;
      free(*p_person);
      (*p_person) = NULL;
    }
    p_person = NULL;
  }
}

/**
 * Returns a pointer to the meeting with the given person (the person who has the given ID).
 * @param person (struct Person *) the person we would like to get his/her meeting.
 * @par id (IdT) the id of the person she/he met with.
 * @return (Meeting *) a pointer to meeting, returns NULL
 * if no such exists.
 * @if_fails returns NULL.
 * @assumption you can not assume anything.
 */
Meeting *PersonGetMeetingById(const Person *const person, IdT id) {
  if (person == NULL) {
    return NULL;
  }
  if (person->meetings == NULL || (*(person->meetings)) == NULL ||
      person->num_of_meetings == ZERO) {
    return NULL;
  }
  for (size_t i = 0; i < person->num_of_meetings; i++) {
    if ((person->meetings)[i]->person_2->id == id) {
      return (person->meetings)[i];
    }
  }
  return NULL;
}

/**
 * The function is used to compare people.
 * @param person_1 (struct Person *) person we would like to compare.
 * @param person_2 (struct Person *) person we would like to compare.
 * @return 0 if the two people are to be considered equal,
 *        -1 if person_1 should be before person_2.
 *         1 if person_1 should be after person_2.
 * @if_fails can not fail.
 * @assumption the input would be valid (pointers to existing people).
 */
int PersonCompareById(const Person *person_1, const Person *person_2) {
  if (person_1->id > person_2->id) {
    return RETURN_ONE;
  }
  if (person_1->id < person_2->id) {
    return RETURN_MINUS_ONE;
  }
  return RETURN_EQUALS;
}

/**
 * The function is used to compare people.
 * @param person_1 (struct Person *) person we would like to compare.
 * @param person_2 (struct Person *) person we would like to compare.
 * @return 0 if the two people are to be considered equal,
 *        -1 if person_1 should be before person_2.
 *         1 if person_1 should be after person_2.
 * @if_fails can not fail.
 * @assumption the input would be valid (pointers to existing people).
 */
int PersonCompareByName(const Person *person_1, const Person *person_2) {
  if (strcmp(person_1->name, person_2->name) == 0) {
    return RETURN_EQUALS;
  }
  if (strcmp(person_1->name, person_2->name) < 0) {
    return RETURN_MINUS_ONE;
  }
  return RETURN_ONE;
}

/**
 * The function is used to compare people.
 * @param person_1 (struct Person *) person we would like to compare.
 * @param person_2 (struct Person *) person we would like to compare.
 * @return 0 if the two people are to be considered equal,
 *        -1 if person_1 should be before person_2.
 *         1 if person_1 should be after person_2.
 * @if_fails can not fail.
 * @assumption the input would be valid (pointers to existing people).
 */
int PersonCompareByInfectionRate(const Person *person_1, const Person
*person_2) {
  if (person_1->infection_rate == person_2->infection_rate) {
    return RETURN_EQUALS;
  }
  if (person_1->infection_rate > person_2->infection_rate) {
    return RETURN_MINUS_ONE;
  }
  return RETURN_ONE;
}

/**
 * The function is used to compare people.
 * @param person_1 (struct Person *) person we would like to compare.
 * @param person_2 (struct Person *) person we would like to compare.
 * @return 0 if the two people are to be considered equal,
 *        -1 if person_1 should be before person_2.
 *         1 if person_1 should be after person_2.
 * @if_fails can not fail.
 * @assumption the input would be valid (pointers to existing people).
 */
int PersonCompareByAge(const Person *person_1, const Person *person_2) {
  if (person_1->age == person_2->age) {
    return RETURN_EQUALS;
  }
  if (person_1->age > person_2->age) {
    return RETURN_MINUS_ONE;
  }
  return RETURN_ONE;
}


