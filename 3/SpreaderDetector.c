/**
 * @brief describes a spreader detector

 */
#include "Meeting.h"
#include "Person.h"
#include "SpreaderDetector.h"
#include "Constants.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief a delimeter for parsing files
 */
#define DELIMITER " "
/**
 * @brief reading mode for file open
 */
#define FILE_OPEN_MODE "r"
/**
 * @brief writing mode for file open
 */
#define FILE_WRITE_MODE "w"
/**
 * @brief describes sick person
 */
#define IS_SICK "SICK"
/**
 * @brief new line in windows
 */
#define NEW_LINE_WIN "\r\n"
/**
 * @brief new line in linux
 */
#define NEW_LINE_LINUX "\n"
/**
 * @brief defines one size for calloc
 */
#define ONE 1
/**
 * @brief describes not found for IdExists
 */
#define FOUND -1
/**
 * @brief describes found for IdExists
 */
#define NOT_FOUND 1
/**
 * @brief Failed functionS SpreaderDetectorAddPerson,IdsValidation,
 * SpreaderDetectorAddMeeting, PRINT TO OUTPUT FILE process
 * return value
 */
#define RETURN_FAILED 0
/**
 * @brief Success function SpreaderDetectorAddPerson,IdsValidation ,
 * SpreaderDetectorAddMeeting PRINT TO OUTPUT FILE
 * process return value
 */
#define RETURN_SUCCESS 1
/**
 * @brief initializer for array of chars
 */
#define ZERO_INIT 0
/**
 * @brief base for strtoul function
 */
#define BASE 10
/**
 * @brief lenght of word SICK for parsing use
 */
#define SICK_LEN 4
/**
 * @brief defines sick person in int
 */
#define SICK_INT 1
/**
 * @brief defines NOT sick person in int
 */
#define NOT_SICK_INT 0
/**
 * @brief check if size of arrays 0
 */
#define SIZE 0
/**
 * @brief index init for special array
 */
#define INDEX_ONE 1
/**
 * @brief infected 0 rate
 */
#define INF_RATE_ZERO 0
/**
 * @brief failed func return value- SpreaderDetectorGetInfectionRateById
 */
#define FAILED_FUNC -1

/**
 * Allocates (dynamically) a spreader detector with no meetings and
 * no people.
 * @return pointer to dynamically allocated SpreaderDetector struct.
 * @if_fails returns NULL.
 * @assumption you can not assume anything.
 */
SpreaderDetector *SpreaderDetectorAlloc() {
  SpreaderDetector *spreader_detector = (SpreaderDetector *) calloc(ONE, sizeof
      (SpreaderDetector));
  if (spreader_detector == NULL) {
    return NULL;
  }
  spreader_detector->people_cap = SPREADER_DETECTOR_INITIAL_SIZE;
  spreader_detector->meeting_cap = SPREADER_DETECTOR_INITIAL_SIZE;
  spreader_detector->people =
      (Person **) calloc(SPREADER_DETECTOR_INITIAL_SIZE, sizeof(Person *));
  if (spreader_detector->people == NULL) {
    return NULL;
  }
  spreader_detector->meetings =
      (Meeting **) calloc(SPREADER_DETECTOR_INITIAL_SIZE, sizeof(Meeting *));
  if (spreader_detector->meetings == NULL) {
    return NULL;
  }
  spreader_detector->meeting_size = 0;
  spreader_detector->people_size = 0;
  return spreader_detector;
}

/**
 * Frees the given spreader detector.
 * @param p_spreader_detector pointer to spreader detector pointer
 * should be freed.
 * @assumption you can not assume anything.
 */
void SpreaderDetectorFree(SpreaderDetector **p_spreader_detector) {
  if (p_spreader_detector != NULL) {
    if (*p_spreader_detector != NULL) {
      free((*p_spreader_detector)->people);
      (*p_spreader_detector)->people = NULL;
      free((*p_spreader_detector)->meetings);
      (*p_spreader_detector)->meetings = NULL;
      free(*p_spreader_detector);
      *p_spreader_detector = NULL;
    }
    p_spreader_detector = NULL;
  }
}

/**
 * @brief check if given person exists in the people list of the spreadDetector
 * @param spreader_detector the spreader to check in
 * @param person the person to look for
 * @return -1 if found, 1 if not found
 */
int IdExists(SpreaderDetector *spreader_detector, Person
*person) {
  for (size_t i = 0; i < spreader_detector->people_size; i++) {
    if ((spreader_detector->people)[i]->id == person->id) {
      return FOUND;
    }
  }
  return NOT_FOUND;
}

/**
 * Adds the given person to the spreader detector.
 * Important - each person is unique (compare by id).
 * @param spreader_detector the spreader detector we wants to add the person to.
 * @param person the person we wants to add to the spreader detector.
 * @return 1 if the person was added successfully, 0 otherwise.
 * @if_fails returns 0.
 * @assumption you can not assume anything.
 */
int SpreaderDetectorAddPerson(SpreaderDetector *spreader_detector, Person
*person) {
  if (spreader_detector == NULL) {
    return RETURN_FAILED;
  }
  if (person == NULL) {
    return RETURN_FAILED;
  }
  if (IdExists(spreader_detector, person) == FOUND) {
    return RETURN_FAILED;
  }
  if (spreader_detector->people_cap == spreader_detector->people_size) {
    spreader_detector->people_cap *= SPREADER_DETECTOR_GROWTH_FACTOR;
    Person **temp = (Person **) realloc(spreader_detector->people, sizeof
        (Person *) * spreader_detector->people_cap);
    if (!temp) {
      free(spreader_detector->people);
      spreader_detector->people = NULL;
      return RETURN_FAILED;
    }
    spreader_detector->people = temp;
    for (size_t i = spreader_detector->people_size;
         i < spreader_detector->people_cap; i++) {
      spreader_detector->people[i] = NULL;
    }
  }
  spreader_detector->people[spreader_detector->people_size] = person;
  spreader_detector->people_size++;
  return RETURN_SUCCESS;
}

/**
 * @brief validates if both of the meeting people arre in the sprea detector
 * people list
 * @param spreader_detector the spread det. to check in
 * @param meeting the meeting with both people to look for
 * @return 1 if found both people, else return 0
 */
int IdsValidation(SpreaderDetector *spreader_detector, Meeting *meeting) {
  int first_id = 0;
  int sec_id = 0;
  for (size_t i = 0; i < spreader_detector->people_size; i++) {
    if (spreader_detector->people[i]->id == meeting->person_1->id) {
      first_id = ONE;
    }
    if (spreader_detector->people[i]->id == meeting->person_2->id) {
      sec_id = ONE;
    }
  }
  if (sec_id == ONE && first_id == ONE) {
    return RETURN_SUCCESS;
  }
  return RETURN_FAILED;
}

/**
 * @brief reallocates memory for meetings of a person, when adding new meetings
 * @param meeting the meeting that added to spreader detector
 * @return the memory allocated for the array of meetings, if failed return
 * NULL
 */
Meeting **MeetingReallocForPerson(Meeting *meeting) {
  meeting->person_1->meetings_capacity *= PERSON_GROWTH_FACTOR;
  Meeting **temp = (Meeting **) realloc(meeting->person_1->meetings, sizeof
      (Meeting *) * meeting->person_1->meetings_capacity);
  if (!temp) {
    free(meeting->person_1->meetings);
    meeting->person_1->meetings = NULL;
    return NULL;
  }
  meeting->person_1->meetings = temp;
  for (size_t i = meeting->person_1->num_of_meetings;
       i < meeting->person_1->meetings_capacity; i++) {
    meeting->person_1->meetings[i] = NULL;
  }
  return meeting->person_1->meetings;
}

/**
 * @brief realloc more place for meetings array of spreader detector
 * @param spreader_detector the spreader
 * @return pointer to memory, if failed return NULL
 */
Meeting **MeetingReallocForSpreaderDec(SpreaderDetector *spreader_detector) {
  spreader_detector->meeting_cap *= SPREADER_DETECTOR_GROWTH_FACTOR;
  Meeting **temp = (Meeting **) realloc(spreader_detector->meetings, sizeof
      (Meeting *) * spreader_detector->meeting_cap);
  if (!temp) {
    free(spreader_detector->meetings);
    spreader_detector->meetings = NULL;
    return RETURN_FAILED;
  }
  spreader_detector->meetings = temp;
  for (size_t i = spreader_detector->meeting_size;
       i < spreader_detector->meeting_cap; i++) {
    spreader_detector->meetings[i] = NULL;
  }
  return spreader_detector->meetings;
}

/**
 * Adds the given meeting to the spreader detector.
 * Important - the people in the meeting should exist in the spreader detector.
 * @param spreader_detector the spreader detector we wants to add the meeting to.
 * @param meeting the meeting we wants to add to the spreader detector.
 * @return 1 if the meeting was added successfully, 0 otherwise.
 * @if_fails returns 0.
 * @assumption you can not assume anything.
 */
int SpreaderDetectorAddMeeting(SpreaderDetector *spreader_detector, Meeting
*meeting) {
  if (spreader_detector == NULL || meeting == NULL || IdsValidation
      (spreader_detector, meeting) == RETURN_FAILED) {
    return RETURN_FAILED;
  }
  for (size_t i = 0; i < spreader_detector->meeting_size; i++) {
    if (spreader_detector->meetings[i] == meeting) {
      return RETURN_FAILED;
    }
  }
  if (meeting->person_1->num_of_meetings ==
      meeting->person_1->meetings_capacity) {
    if (MeetingReallocForPerson(meeting) == NULL) {
      return RETURN_FAILED;
    }
  }
  meeting->person_1->meetings[meeting->person_1->num_of_meetings] = meeting;
  meeting->person_1->num_of_meetings++;
  if (spreader_detector->meeting_cap == spreader_detector->meeting_size) {
    if (MeetingReallocForSpreaderDec(spreader_detector) == NULL) {
      return RETURN_FAILED;
    }
  }
  spreader_detector->meetings[spreader_detector->meeting_size] = meeting;
  spreader_detector->meeting_size++;
  return RETURN_SUCCESS;
}

/**
 * @brief checks if person is found in the spreader detector people array
 * @param spreader_detector the spreader to check on
 * @param id the is of the person to look for
 * @return return the pointer to person if found, if not found returns NULL
 */
Person *CheckPersonInDetector(SpreaderDetector *spreader_detector, size_t
id) {
  Person *person = NULL;
  for (size_t i = 0; i < spreader_detector->people_size; i++) {
    if (spreader_detector->people[i]->id == id) {
      person = spreader_detector->people[i];
      break;
    }
  }
  return person;
}

/**
 * This function reads the file of the meeting, parses to file into meetings,
 * and inserts it to the spreader detector.
 * @param spreader_detector the spreader detector we wants to read the meetings
 * into.
 * @param path the path to the meetings file.
 * @assumption you can assume that the path to the file is ok (and anything but
 * that).
 */
void SpreaderDetectorReadMeetingsFile(SpreaderDetector *spreader_detector,
                                      const char *path) {
  Person *sec = NULL, *first = NULL;
  size_t id_first, id_sec;
  double dist, measure;
  FILE *meetings = fopen(path, FILE_OPEN_MODE);
  if (meetings == NULL || spreader_detector == NULL ||
      spreader_detector->people == NULL || spreader_detector->people_size ==
      0) {
    exit(EXIT_FAILURE);
  }
  char buffer[MAX_LEN_OF_LINE] = {0}, *token = NULL;
  while (fgets(buffer, MAX_LEN_OF_LINE, meetings) != NULL) {
    token = strtok(buffer, DELIMITER);
    id_first = strtoul(token, NULL, BASE);
    first = CheckPersonInDetector(spreader_detector, id_first);
    token = strtok(NULL, DELIMITER);
    id_sec = strtoul(token, NULL, BASE);
    sec = CheckPersonInDetector(spreader_detector, id_sec);
    if (first == NULL || sec == NULL) {
      continue;
    }
    token = strtok(NULL, DELIMITER);
    dist = strtod(token, NULL);
    token = strtok(NULL, DELIMITER);
    measure = strtod(token, NULL);
    Meeting *meeting = MeetingAlloc(first, sec, measure, dist);
    SpreaderDetectorAddMeeting(spreader_detector, meeting);
    token = strtok(NULL, DELIMITER);
  }
  fclose(meetings);
}

/**
 * This function reads the file of the people, parses to file into person
 * objects,
 * and inserts it to the spreader detector.
 * @param spreader_detector the spreader detector we wants to read the people
 * into.
 * @param path the path to the people file.
 * @assumption you can assume that the path to the file is ok (and anything
 * but that).
 */
void SpreaderDetectorReadPeopleFile(SpreaderDetector *spreader_detector,
                                    const char *path) {
  FILE *people = fopen(path, FILE_OPEN_MODE);
  char *name = NULL, buffer[MAX_LEN_OF_LINE] = {ZERO_INIT}, *token = NULL;
  IdT id;
  size_t age;
  int is_sick;
  if (people == NULL || spreader_detector == NULL) {
    exit(EXIT_FAILURE);
  }
  while (fgets(buffer, MAX_LEN_OF_LINE, people) != NULL) {
    token = strtok(buffer, DELIMITER);
    if (strcmp(token, NEW_LINE_LINUX) == 0
        || strcmp(token, NEW_LINE_WIN) == 0) {
      continue;
    }
    name = token;
    token = strtok(NULL, DELIMITER);
    id = strtoul(token, NULL, BASE);
    if (CheckPersonInDetector(spreader_detector, id) != NULL) {
      continue;
    }
    token = strtok(NULL, DELIMITER);
    age = strtoul(token, NULL, BASE);
    token = strtok(NULL, DELIMITER);
    if (token != NULL && strcmp(token, NEW_LINE_WIN) != 0
        && strcmp(token, NEW_LINE_LINUX) != 0
        && strncmp(token, IS_SICK, SICK_LEN) == 0) {
      is_sick = SICK_INT;
      token = strtok(NULL, DELIMITER);
    } else {
      is_sick = NOT_SICK_INT;
    }
    Person *new_person = PersonAlloc(id, name, age, is_sick);
    SpreaderDetectorAddPerson(spreader_detector, new_person);
  }
  fclose(people);
}

/**
 * @brief looks for the sick person in the spreader detector
 * @param spreader_detector
 * @return returns the sick person pointer, if not found returns NULL
 */
Person *SickPerson(SpreaderDetector *spreader_detector) {
  for (size_t i = 0; i < spreader_detector->people_size; i++) {
    if (spreader_detector->people[i]->is_sick == 1) {
      return spreader_detector->people[i];
    }
  }
  return NULL;
}

/**
 * @brief calculates the crna value for one pair
 * only the value for 2 people, no chain calculation
 * @param distance the distance between the people in meeting
 * @param measure the measure of the meeting
 * @return the crna claculation
 */
double CalculateCrna(double distance, double measure) {
  return (measure * MIN_DISTANCE) / (distance * MAX_MEASURE);
}

/**
 * @brief check and update if needed the crna value for person
 * of age above given threshold
 * @param person2
 */
void CheckAgeCrna(Person *person2) {
  if (person2->age > AGE_THRESHOLD) {
    person2->infection_rate +=
        INFECTION_RATE_ADDITION_DUE_TO_AGE;
  }
}

/**
 * This function runs the algorithm which calculates the infection rates of
 * the people.
 * When this algorithm ends, the user should be able to use the function
 * SpreaderDetectorGetInfectionRateById and get the infection rate of each
 * person.
 * @param spreader_detector a spreader_detector.
 * @assumption you can not assume anything.
 */
void SpreaderDetectorCalculateInfectionChances(SpreaderDetector
                                               *spreader_detector) {
  if (spreader_detector == NULL || spreader_detector->people == NULL
      || spreader_detector->people_size == SIZE) {
    return;
  }
  Person *spreader = SickPerson(spreader_detector);
  spreader->infection_rate = 1.0;
  Person **people_arr =
      (Person **) calloc(spreader_detector->people_size, sizeof(Person *));
  if (people_arr == NULL) {
    return;
  }
  people_arr[ZERO_INIT] = spreader;
  Person *person;
  size_t index = INDEX_ONE;
  for (size_t i = 0; i < spreader_detector->people_size; i++) {
    person = people_arr[i];
    if (person == NULL) {
      continue;
    }
    for (size_t j = 0; j < person->num_of_meetings; j++) {
      if (person->meetings[j]->person_2->infection_rate == INF_RATE_ZERO) {
        person->meetings[j]->person_2->infection_rate = CalculateCrna(
            person->meetings[j]->distance, person->meetings[j]->measure)
            * person->infection_rate;
        CheckAgeCrna(person->meetings[j]->person_2);
        people_arr[index] = person->meetings[j]->person_2;
        index++;
      }
    }
  }
  free(people_arr);
  people_arr = NULL;
}

/**
 * Returns the infection rate of the person with the given id.
 * @param spreader_detector the spreader detector contains the person.
 * @param id the id of the person we are looking for.
 * @return the infection rate of the person, if not person exists -
 * returns -1.
 * @if_fails returns -1.
 * @assumption you can assume anything.
 */
double SpreaderDetectorGetInfectionRateById(SpreaderDetector
                                            *spreader_detector, IdT id) {
  if (spreader_detector == NULL || spreader_detector->people_size == SIZE) {
    return FAILED_FUNC;
  }
  Person *person = CheckPersonInDetector(spreader_detector, id);
  if (person == NULL) {
    return FAILED_FUNC;
  }
  return person->infection_rate;
}

/**
 * Gets the recommendation for treatment for all people based on the parameters
 * above,
 * and prints it to the given file path.
 * @param spreader_detector the spreader detector contains the person.
 * @param file_path the path to the output file.
 * @return returns 1 if printed successfully, 0 otherwise.
 * @if_fails return 0.
 * @assumption you can assume that the path to the file is ok (and anything
 * but that).
 */
int SpreaderDetectorPrintRecommendTreatmentToAll(SpreaderDetector
                                                 *spreader_detector,
                                                 const char *file_path) {
  FILE *output = fopen(file_path, FILE_WRITE_MODE);
  if (output == NULL) {
    return RETURN_FAILED;
  }
  if (spreader_detector == NULL) {
    return RETURN_FAILED;
  }
  for (size_t i = 0; i < spreader_detector->people_size; i++) {
    if (spreader_detector->people[i]->infection_rate >
        MEDICAL_SUPERVISION_THRESHOLD) {
      fprintf(output,
              MEDICAL_SUPERVISION_THRESHOLD_MSG,
              spreader_detector->people[i]->name,
              spreader_detector->people[i]->id,
              spreader_detector->people[i]->age,
              spreader_detector->people[i]->infection_rate);
    } else if (spreader_detector->people[i]->infection_rate >
        REGULAR_QUARANTINE_THRESHOLD) {
      fprintf(output,
              REGULAR_QUARANTINE_MSG,
              spreader_detector->people[i]->name,
              spreader_detector->people[i]->id,
              spreader_detector->people[i]->age,
              spreader_detector->people[i]->infection_rate);
    } else {
      fprintf(output,
              CLEAN_MSG,
              spreader_detector->people[i]->name,
              spreader_detector->people[i]->id,
              spreader_detector->people[i]->age,
              spreader_detector->people[i]->infection_rate);
    }
  }
  fclose(output);
  return RETURN_SUCCESS;
}

/**
 * Returns the number of people which are in the spreader detector.
 * @param spreader_detector the spreader detector object.
 * @return the number of people.
 * @if_fails return 0.
 * @assumption you can not assume anything.
 * @note 0 might represent spreader with no people, and fail both.
 * make sure you don't trust the output of the function.
 */
size_t SpreaderDetectorGetNumOfPeople(SpreaderDetector *spreader_detector) {
  if (spreader_detector == NULL) {
    return RETURN_FAILED;
  }
  return spreader_detector->people_size;
}

/**
 * Returns the number of meetings which are in the spreader detector.
 * @param spreader_detector the spreader detector object.
 * @return the number of meetings.
 * @if_fails return 0.
 * @assumption you can not assume anything.
 * @note 0 might represent spreader with no meetings, and fail both.
 * make sure you don't trust the output of the function.
 */
size_t SpreaderDetectorGetNumOfMeetings(SpreaderDetector *spreader_detector) {
  if (spreader_detector == NULL) {
    return RETURN_FAILED;
  }
  return spreader_detector->meeting_size;
}






