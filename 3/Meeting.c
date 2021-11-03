/**
 * @brief describes a meeting between 2 people
 * 
 */

#include "Meeting.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief defines one size for calloc
 */
#define ONE 1

/**
 * Allocating (dynamically) new meeting with (at least) the following
 * input data:
 * @param person_1 (struct Person *) pointer to the first person in the meeting.
 * @param person_2 (struct Person *) pointer to the second person in the
 * meeting.
 * @param measure (double) the time of the meeting in minutes.
 * @param distance (double) the distance the two people where in.
 * @return (struct Meeting *) pointer to dynamically allocated meeting.
 * @if_fails returns NULL.
 * @assumption the inputs would be valid.
 */
Meeting *MeetingAlloc(Person *person_1, Person *person_2, double measure,
                      double distance) {
  Meeting *new_meeting = (Meeting *) calloc(ONE, sizeof(Meeting));
  if (new_meeting == NULL) {
    return NULL;
  }
  new_meeting->person_1 = person_1;
  new_meeting->person_2 = person_2;
  new_meeting->measure = measure;
  new_meeting->distance = distance;
  return new_meeting;
}

/**
 * Frees everything the meeting has allocated and the pointer itself.
 * @param p_meeting (struct Meeting **) pointer to dynamically allocated meeting.
 * @assumption you can not assume anything.
 */
void MeetingFree(Meeting **p_meeting) {
  if (p_meeting != NULL) {
    if (*p_meeting != NULL) {
      free(*p_meeting);
      *p_meeting = NULL;
    }
    p_meeting = NULL;
  }
}

/**
 * Returns a pointer to one of the persons in the meeting.
 * @param meeting (struct Meeting *) the meeting we would like to
 * get its person.
 * @param person_ind (size_t) the index of the person we
 * want (can be either 1 or 2).
 * @return (struct Person *) pointer to the person we want.
 * person_ind == 1 ==> return person_1 (according the person_1 given in
 * MeetingAlloc).
 * person_ind == 2 ==> return person_2 (according to the person_2 given in
 * MeetingAlloc).
 * @if_failds return NULL
 * @assumption you can not assume anything.
 */
Person *MeetingGetPerson(const Meeting *const meeting, size_t person_ind) {
  if (meeting == NULL) {
    return NULL;
  }
  if (person_ind != 1 && person_ind != 2) {
    return NULL;
  }
  if (person_ind == 1) {
    return meeting->person_1;
  } else {
    return meeting->person_2;
  }

}
