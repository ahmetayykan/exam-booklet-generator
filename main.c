#include <stdio.h> // Include standard input/output library
#include <stdlib.h> // Include standard library for memory allocation and process control
#include <string.h> // Include string manipulation functions
#include <time.h> // Include time functions for random number generation

#define MAX_QUESTIONS 200 // Maximum number of questions that can be stored
#define MAX_TEXT 1000 // Maximum length for question text and options

// Structure to represent a single question
typedef struct {
    int num; // Original question number
    char body[MAX_TEXT]; // Question text
    char optionA[MAX_TEXT]; // Option A text
    char optionB[MAX_TEXT]; // Option B text
    char optionC[MAX_TEXT]; // Option C text
    char optionD[MAX_TEXT]; // Option D text
    char answer; // Correct answer ('A', 'B', 'C', or 'D')
} Question;

// Structure to store all questions from input file
typedef struct {
    Question questions[MAX_QUESTIONS]; // Array of all questions
    int count; // Total number of questions loaded
} Question_Bank;

// Function to remove newline and carriage return characters from end of string
void removeNewline(char *str) {
    int len = (int)strlen(str); // Get length of string
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) { // Check if last character is newline or carriage return
        str[len - 1] = '\0'; // Replace with null terminator
        len--; // Decrease length counter
    }
}

// Function to remove leading spaces and tabs from string
void trimLeading(char *str) {
    while (str[0] == ' ' || str[0] == '\t') { // While first character is space or tab
        memmove(str, str + 1, strlen(str)); // Shift entire string left by one position
    }
}

// Function to extract text after first dot in a line
void extractAfterDot(const char *line, char *out) {
    const char *dot = strchr(line, '.'); // Find first dot in line
    if (dot != NULL) { // If dot found
        strncpy(out, dot + 1, MAX_TEXT - 1); // Copy text after dot
        out[MAX_TEXT - 1] = '\0'; // Ensure null termination
    } else { // If no dot found
        strncpy(out, line, MAX_TEXT - 1); // Copy entire line
        out[MAX_TEXT - 1] = '\0'; // Ensure null termination
    }
    trimLeading(out); // Remove leading spaces
    removeNewline(out); // Remove newline characters
}

// Function to check if a line is blank or contains only whitespace
int isBlankLine(const char *line) {
    for (int i = 0; line[i] != '\0'; i++) { // Loop through each character
        if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\r') { // If character is not whitespace
            return 0; // Line is not blank
        }
    }
    return 1; // Line is blank
}

// Function to read all questions from input file
void loadQuestions(const char *filename, Question_Bank *bank) {
    FILE *file = fopen(filename, "r"); // Open input file for reading
    if (file == NULL) { // Check if file opened successfully
        perror("Error: Cannot open input file"); // Print error message
        exit(1); // Exit program with error code
    }

    bank->count = 0; // Initialize question count to zero

    char line[MAX_TEXT]; // Buffer to read lines from file

    while (fgets(line, MAX_TEXT, file) != NULL) { // Read each line until end of file
        if (isBlankLine(line)) { // If line is blank
            continue; // Skip to next line
        }

        if (line[0] == 'Q') { // If line starts with 'Q', it's a question
            if (bank->count >= MAX_QUESTIONS) { // Check if maximum questions reached
                printf("Warning: MAX_QUESTIONS reached, extra questions ignored.\n"); // Print warning
                break; // Stop reading questions
            }

            Question *q = &bank->questions[bank->count]; // Get pointer to current question

            q->num = 0; // Initialize question number
            q->body[0] = '\0'; // Initialize question body to empty string
            q->optionA[0] = '\0'; // Initialize option A to empty string
            q->optionB[0] = '\0'; // Initialize option B to empty string
            q->optionC[0] = '\0'; // Initialize option C to empty string
            q->optionD[0] = '\0'; // Initialize option D to empty string
            q->answer = 'A'; // Initialize answer to 'A' as default

            sscanf(line, "Q%d:", &q->num); // Extract question number from line

            char *colon = strchr(line, ':'); // Find colon in line
            if (colon != NULL) { // If colon found
                strncpy(q->body, colon + 1, MAX_TEXT - 1); // Copy text after colon as question body
                q->body[MAX_TEXT - 1] = '\0'; // Ensure null termination
                trimLeading(q->body); // Remove leading spaces
                removeNewline(q->body); // Remove newline characters
            } else { // If no colon found
                strncpy(q->body, line, MAX_TEXT - 1); // Copy entire line as question body
                q->body[MAX_TEXT - 1] = '\0'; // Ensure null termination
                removeNewline(q->body); // Remove newline characters
            }

            if (fgets(line, MAX_TEXT, file) == NULL) break; // Read option A line, exit if end of file
            extractAfterDot(line, q->optionA); // Extract option A text after dot

            if (fgets(line, MAX_TEXT, file) == NULL) break; // Read option B line, exit if end of file
            extractAfterDot(line, q->optionB); // Extract option B text after dot

            if (fgets(line, MAX_TEXT, file) == NULL) break; // Read option C line, exit if end of file
            extractAfterDot(line, q->optionC); // Extract option C text after dot

            if (fgets(line, MAX_TEXT, file) == NULL) break; // Read option D line, exit if end of file
            extractAfterDot(line, q->optionD); // Extract option D text after dot

            if (fgets(line, MAX_TEXT, file) == NULL) break; // Read answer line, exit if end of file

            char ans_char = '\0'; // Variable to store answer character
            if (sscanf(line, "ANS: %c", &ans_char) == 1) { // Try to extract answer character
                if (ans_char >= 'a' && ans_char <= 'd') { // If answer is lowercase
                    q->answer = (char)('A' + (ans_char - 'a')); // Convert to uppercase
                } else if (ans_char >= 'A' && ans_char <= 'D') { // If answer is already uppercase
                    q->answer = ans_char; // Use as is
                } else { // If invalid character
                    q->answer = 'A'; // Default to 'A'
                }
            } else { // If cannot extract answer
                q->answer = 'A'; // Default to 'A'
            }

            bank->count++; // Increment question counter
        }
    }

    fclose(file); // Close input file
    printf("Loaded %d questions from file.\n", bank->count); // Print success message
}

// Function to shuffle the order of questions in an array
void shuffleQuestions(Question *questions, int n) {
    for (int i = n - 1; i > 0; i--) { // Loop from last element to first
        int j = rand() % (i + 1); // Generate random index from 0 to i
        Question temp = questions[i]; // Store current question in temporary variable
        questions[i] = questions[j]; // Swap current with random question
        questions[j] = temp; // Complete the swap
    }
}

// Function to shuffle options of a single question
void shuffleOptions(Question *q) {
    char options[4][MAX_TEXT]; // Array to hold all four options
    int correct_index = q->answer - 'A'; // Find index of correct answer (0-3)

    strcpy(options[0], q->optionA); // Copy option A to array
    strcpy(options[1], q->optionB); // Copy option B to array
    strcpy(options[2], q->optionC); // Copy option C to array
    strcpy(options[3], q->optionD); // Copy option D to array

    for (int i = 3; i > 0; i--) { // Shuffle the options array
        int j = rand() % (i + 1); // Generate random index

        char temp_option[MAX_TEXT]; // Temporary storage for option text
        strcpy(temp_option, options[i]); // Copy current option
        strcpy(options[i], options[j]); // Swap current with random
        strcpy(options[j], temp_option); // Complete swap

        if (correct_index == i) { // If correct answer was at position i
            correct_index = j; // Update correct answer position to j
        } else if (correct_index == j) { // If correct answer was at position j
            correct_index = i; // Update correct answer position to i
        }
    }

    strcpy(q->optionA, options[0]); // Assign shuffled option to A
    strcpy(q->optionB, options[1]); // Assign shuffled option to B
    strcpy(q->optionC, options[2]); // Assign shuffled option to C
    strcpy(q->optionD, options[3]); // Assign shuffled option to D

    q->answer = (char)('A' + correct_index); // Update correct answer based on new position
}

// Function to generate one booklet and its answer sheet
void generateBooklet(Question_Bank *bank, int booklet_num, int n) {
    Question selected[MAX_QUESTIONS]; // Array to store selected questions
    int used[MAX_QUESTIONS]; // Array to track which questions are used

    for (int i = 0; i < MAX_QUESTIONS; i++) { // Initialize used array
        used[i] = 0; // Set all to zero (not used)
    }

    for (int i = 0; i < n; i++) { // Select n distinct random questions
        int rand_index = rand() % bank->count; // Generate random question index
        while (used[rand_index] == 1) { // If question already used
            rand_index = rand() % bank->count; // Generate new random index
        }
        used[rand_index] = 1; // Mark this question as used
        selected[i] = bank->questions[rand_index]; // Copy selected question
    }

    shuffleQuestions(selected, n); // Randomize order of selected questions

    for (int i = 0; i < n; i++) { // Shuffle options for each question
        shuffleOptions(&selected[i]); // Randomize A, B, C, D order
    }

    char booklet_filename[50]; // Buffer for booklet file name
    sprintf(booklet_filename, "booklet_%d.txt", booklet_num); // Create booklet file name

    FILE *booklet_file = fopen(booklet_filename, "w"); // Open booklet file for writing
    if (booklet_file == NULL) { // Check if file opened successfully
        perror("Error: Cannot create booklet file"); // Print error message
        return; // Exit function if error
    }

    char answer_filename[50]; // Buffer for answer sheet file name
    sprintf(answer_filename, "answer_%d.txt", booklet_num); // Create answer file name

    FILE *answer_file = fopen(answer_filename, "w"); // Open answer file for writing
    if (answer_file == NULL) { // Check if file opened successfully
        perror("Error: Cannot create answer file"); // Print error message
        fclose(booklet_file); // Close booklet file before exiting
        return; // Exit function if error
    }

    fprintf(booklet_file, "EXAM BOOKLET #%d\n", booklet_num); // Write booklet header
    fprintf(booklet_file, "==================\n\n"); // Write separator line

    fprintf(answer_file, "ANSWER SHEET #%d\n", booklet_num); // Write answer sheet header
    fprintf(answer_file, "==================\n\n"); // Write separator line

    for (int i = 0; i < n; i++) { // Write each question to files
        fprintf(booklet_file, "Question %d:\n", i + 1); // Write question number
        fprintf(booklet_file, "%s\n", selected[i].body); // Write question text
        fprintf(booklet_file, "A) %s\n", selected[i].optionA); // Write option A
        fprintf(booklet_file, "B) %s\n", selected[i].optionB); // Write option B
        fprintf(booklet_file, "C) %s\n", selected[i].optionC); // Write option C
        fprintf(booklet_file, "D) %s\n", selected[i].optionD); // Write option D
        fprintf(booklet_file, "\n"); // Add blank line between questions

        fprintf(answer_file, "%d. %c\n", i + 1, selected[i].answer); // Write answer to answer sheet
    }

    fclose(booklet_file); // Close booklet file
    fclose(answer_file); // Close answer file

    printf("Generated: %s and %s\n", booklet_filename, answer_filename); // Print success message
}

// Main function
int main() {
    srand((unsigned int)time(NULL)); // Initialize random number generator with current time

    Question_Bank bank; // Create question bank structure
    loadQuestions("sample.txt", &bank); // Load all questions from input file

    int m, n; // Variables for number of booklets and questions per booklet

    printf("How many booklets to generate? "); // Prompt user for booklet count
    if (scanf("%d", &m) != 1) return 1; // Read number of booklets, exit if invalid input

    printf("How many questions per booklet (max %d)? ", bank.count); // Prompt for questions per booklet
    if (scanf("%d", &n) != 1) return 1; // Read number of questions, exit if invalid input

    if (m <= 0 || n <= 0) { // Validate that inputs are positive
        printf("Error: Invalid input!\n"); // Print error message
        return 1; // Exit with error code
    }

    if (n > bank.count) { // Validate that n is not greater than available questions
        printf("Error: Not enough questions in bank!\n"); // Print error message
        return 1; // Exit with error code
    }

    for (int i = 1; i <= m; i++) { // Generate each booklet
        generateBooklet(&bank, i, n); // Create booklet and answer sheet
    }

    printf("\nAll booklets generated successfully!\n"); // Print completion message
    return 0; // Exit program successfully
}