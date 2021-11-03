
 * Cipher algorithm
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief numbers of expected arguments in the program arguments for the
 * program to run properly
 */
#define NUM_ARGS 5
/**
 * @brief a valid command given in program arguments
 */
#define COMMAND_ENCODE "encode"
/**
 * @brief a valid command given in program arguments
 */
#define COMMAND_DECODE "decode"
/**
 * @brief index of command in program args array
 */
#define COMMAND 1
/**
 * @brief error if not correct numbers of arguments given
 */
#define ERROR_ARGS "Usage: cipher <encode|decode> <k> <source path file> "\
"<output path file>\n"
/**
 * @brief error if the command is invalid
 */
#define ERROR_COMMAND "The given command is invalid\n"
/**
 * @brief the index of the shift input in program args
 */
#define ARGUMENT_SHIFT 2
/**
 * @brief error if shift is invalid <0
 */
#define ERROR_SHIFT "The given shifts value is invalid\n"
/**
 * @brief error if input file doesn't exist/if output file name is invalid
 * syntax
 */
#define FILE_ERROR "The given file is invalid\n"
/**
 * @brief index of input file in array of program args.
 */
#define INPUT_FILE_PATH 3
/**
 * @brief index of output file in array of program args.
 */
#define OUTPUT_FILE_PATH 4
/**
 * @brief ascii number of 'a'
 */
#define LOWER_CASE_MIN 97
/**
 * @brief ascii number of 'z'
 */
#define LOWER_CASE_MAX 122
/**
 * @brief ascii number of 'A'
 */
#define UPPER_CASE_MIN 65
/**
 * @brief ascii number of 'Z'
 */
#define UPPER_CASE_MAX 90
/**
 * @brief number of english letter in the alphabet
 */
#define ENGLISH_LETTERS_NUM 26

/**
 * @brief check whether the command given in main arguments is a valid command
 * for the program
 * @param arg the command string
 * @return true if the command is correct, else false
 */
bool CheckArgCommand(char *arg) {
    bool is_valid = false;
    if (strcmp(arg, COMMAND_DECODE) == 0) {
        is_valid = true;
    }
    if (strcmp(arg, COMMAND_ENCODE) == 0) {
        is_valid = true;
    }
    return is_valid;
}

/**
 * @brief called when num of arguments is not correct
 * checks to close open files
 * @param in input file
 * @param out output file
 */
void CloseFiles(FILE **in, FILE **out) {
    if (*out != NULL) {
        fclose(*out);
    }
    if (*in != NULL) {
        fclose(*in);
    }
}

/**
 * @brief validates the inputs for main program arguments by
 * program desired behavior
 * @param argc number of arguments received
 * @param argv array of arguments
 * @return true if all valid, else false if at least one error occurred
 */
bool InputValidation(int argc, char *argv[], FILE **input) {
    bool is_valid = true;
    if (argc != NUM_ARGS) {
        fprintf(stderr, ERROR_ARGS);
        return false;
    }
    if (atoi(argv[ARGUMENT_SHIFT]) < 0) {
        fprintf(stderr, ERROR_SHIFT);
        return false;
    }
    if (CheckArgCommand(argv[COMMAND]) != true) {
        fprintf(stderr, ERROR_COMMAND);
        return false;
    }
    if (*input == NULL) {
        fprintf(stderr, FILE_ERROR);
        return false;
    }
    return is_valid;
}

/**
 * @brief enocdes one char with the given shift number k
 * @param character the character to encode
 * @param shift_k the shifting number to encode with
 * @return the new enocded character in DEC
 */
int EncodeChar(int character, int shift_k) {
    shift_k %= ENGLISH_LETTERS_NUM;
    if (character <= LOWER_CASE_MAX && character >= LOWER_CASE_MIN) {
        character = (character + shift_k - LOWER_CASE_MIN) %
                ENGLISH_LETTERS_NUM + LOWER_CASE_MIN;
    } else if (character <= UPPER_CASE_MAX && character >= UPPER_CASE_MIN) {
        character = (character + shift_k - UPPER_CASE_MIN) %
                ENGLISH_LETTERS_NUM + UPPER_CASE_MIN;
    }
    return character;
}
/**
 * @brief enocoding main function - goes over the input file and encodes it
 * @param input the input file to encode
 * @param output the name of file to put the encoded text
 * @param k the shift  number for encoding
 */
void EncodeInput(FILE **input, FILE **output, int k) {
    int letter;
    int output_letter;
    //printing into output file the encoded text until end of input file reached
    while ((letter = fgetc(*input)) != EOF) {
        output_letter = EncodeChar(letter, k);
        fprintf(*output, "%c", output_letter);
    }
}

/**
 * @brief decoding one char with the given shift k number, uses encode fucntion
 * @param character the character to decode
 * @param shiftK the shift number to decode
 * @return the decoded character in DEC
 */
int DecodeChar(int character, int shift_k) {
    shift_k %= ENGLISH_LETTERS_NUM;
    return EncodeChar(character, ENGLISH_LETTERS_NUM - shift_k);
}

/**
 * @brief main decoding function, gets input file and decodes it into output
 * file
 * @param input the input file to decode
 * @param output the output file to output the decoded text to
 * @param k the shifting number for decoding
 */
void DecodeInput(FILE **input, FILE **output, int k) {
    int letter;
    int output_letter;
    //printing into output file the encoded text until end of input file reached
    while ((letter = fgetc(*input)) != EOF) {
        output_letter = DecodeChar(letter, k);
        fprintf(*output, "%c", output_letter);
    }
}

/**
 * @brief checking if file exists in path
 * @param file_name the file path/name to check
 * @return true if exists, otherwise false
 */
bool CheckFileExists(char *file_name) {
    FILE *file;
    file = fopen(file_name, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

/**
 * @brief main function - gets program argumnets from user to perform desired
 * action, checks input output files, does validation on the user input,
 * proceeds with the decoding/encoding selected file
 * @param argc number of arguments
 * @param argv array of the arguments
 * @return EXIT_SUCCESS if the algorithm performed correctly, otherwise
 * EXIT_FAILURE
 */
int main(int argc, char *argv[]) {
    FILE *input_file = fopen(argv[INPUT_FILE_PATH], "r");
    FILE *output_file = NULL;
    if (CheckFileExists(argv[OUTPUT_FILE_PATH])) {
        output_file = fopen(argv[OUTPUT_FILE_PATH], "w+");
    } else {
        output_file = fopen(argv[OUTPUT_FILE_PATH], "a+");
    }

    if (InputValidation(argc, argv, &input_file) == false) {
        CloseFiles(&input_file, &output_file);
        return EXIT_FAILURE;
    }

    if (output_file == NULL) {
        fprintf(stderr, FILE_ERROR);
        return EXIT_FAILURE;
    }
    //proceed to algorithm
    if (strcmp(argv[COMMAND], COMMAND_ENCODE) == 0) {
        EncodeInput(&input_file, &output_file, \
        atoi(argv[ARGUMENT_SHIFT]));
        CloseFiles(&input_file, &output_file);
        return EXIT_SUCCESS;
    }
    if (strcmp(argv[COMMAND], COMMAND_DECODE) == 0) {
        DecodeInput(&input_file, &output_file, \
        atoi(argv[ARGUMENT_SHIFT]));
        CloseFiles(&input_file, &output_file);
        return EXIT_SUCCESS;
    }
    CloseFiles(&input_file, &output_file);
}

