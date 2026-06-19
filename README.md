# Randomized Exam Booklet & Answer Sheet Generator
System Programming Course – Project #4


1. Project Overview

This project is a console-based exam booklet generator implemented in C. The program reads a single input exam file containing multiple-choice questions and automatically generates multiple randomized exam booklets along with their corresponding answer sheets.

Each generated booklet contains a randomly selected subset of questions, where both the order of questions and the order of answer options (A–D) are randomized. The correct answers are updated accordingly after option shuffling.

This project focuses on file I/O, structures, arrays, string manipulation, and randomization techniques in C.


2. Technical Implementation

Data Structures

The program uses two main structures as required by the project specification.

Question Structure
The `Question` structure represents a single multiple-choice question.
```c
typedef struct {
    int number;               // Original question number
    char body[1000];          // Question text
    char optionA[1000];       // Option A
    char optionB[1000];       // Option B
    char optionC[1000];       // Option C
    char optionD[1000];       // Option D
    char answer;              // Correct answer (A-D)
} Question;
```

Question Bank Structure
The `Question_Bank` structure stores all questions read from the input file.
```c
typedef struct {
    Question questions[200];  // Array of all loaded questions
    int count;                // Total number of questions
} Question_Bank;
```

Core Technologies Used
- Structures to model exam questions
- Arrays for storing question data
- Random number generation using `rand()` and `srand(time(NULL))`
- File I/O using `fopen`, `fgets`, `fprintf`, `fclose`
- String handling using standard C string functions
- Modular design with separate functions for major operations

3. Program Workflow

1. Load all questions from the input file (`sample.txt`)
2. Ask the user for:
   - Number of booklets to generate (m)
   - Number of questions per booklet (n)
3. For each booklet:
   - Randomly select n distinct questions
   - Shuffle the order of selected questions
   - Shuffle the answer options (A–D) for each question
   - Update the correct answer after shuffling
4. Generate two output files per booklet:
   - One exam booklet file
   - One answer sheet file


4. How to Compile and Run

Using CLion
1. Open the project in CLion
2. Ensure that `sample.txt` is located in the working directory
3. Click the **Run** button or press `Shift + F10`

Using GCC (Windows)
```cmd
gcc -Wall -Wextra -std=c11 main.c -o exam_generator.exe
exam_generator.exe
```

5. Input File Format

File Name

sample.txt


Format Specification
Each question must follow this strict format:

Q<number>: <question_text>
a.	<option_A_text>
b.	<option_B_text>
c.	<option_C_text>
d.	<option_D_text>
ANS: <correct_answer>


Example

Q1: Each class you create becomes a new ________ that can be used to declare variables and create objects.
a.	package
b.	instance
c.	library
d.	type.
ANS: d

Q2: You can declare new classes as needed; this is one reason Java is known as a(n) ________ language.
a.	portable
b.	incremental
c.	extensible
d.	virtual
ANS: c


Important Notes:
- Questions must start with `Q` followed by the question number and colon
- Options must be labeled with `a.`, `b.`, `c.`, `d.` followed by a tab
- The answer line must start with `ANS:` followed by the correct letter
- Blank lines between questions are automatically ignored
- The file format must be preserved exactly for correct parsing



6. Output Files

For each generated booklet, two output files are created:

booklet_1.txt, answer_1.txt
booklet_2.txt, answer_2.txt
booklet_3.txt, answer_3.txt

Booklet File
- Contains randomized questions
- Options appear in shuffled order
- Formatted with clear headers and numbering

Answer Sheet File
- Contains the correct answers after option shuffling
- Matches the corresponding booklet exactly
- Simple numbered list format


7. Randomization Strategy

The program implements three levels of randomization:

Question Selection
Random selection of questions without repetition within the same booklet

Question Order
Fisher-Yates shuffle algorithm to randomize the order of selected questions

Option Order
Options (A–D) are shuffled while tracking the correct answer index to ensure correctness after shuffling



8. Input Validation and Error Handling

The program includes validation and error handling for:
- Missing or inaccessible input file
- Invalid user input for booklet count or question count
- Question count exceeding available questions
- Invalid or malformed input file lines
- File creation errors during output generation
- Non-positive input values

9. Sample Program Execution

Loaded 32 questions from file.
How many booklets to generate? 2
How many questions per booklet (max 32)? 5
Generated: booklet_1.txt and answer_1.txt
Generated: booklet_2.txt and answer_2.txt

All booklets generated successfully!


10. Function Overview
```c
void loadQuestions(const char *filename, Question_Bank *bank);
// Reads and parses all questions from input file

void shuffleQuestions(Question *questions, int n);
// Randomizes question order using Fisher-Yates algorithm

void shuffleOptions(Question *q);
// Shuffles A-D options and updates correct answer

void generateBooklet(Question_Bank *bank, int booklet_num, int n);
// Generates booklet and answer sheet files

void removeNewline(char *str);
// Removes newline and carriage return characters

void trimLeading(char *str);
// Removes leading spaces and tabs

void extractAfterDot(const char *line, char *out);
// Extracts text after first dot in option lines

int isBlankLine(const char *line);
// Checks if line contains only whitespace
```



11. Known Limitations

1. Maximum number of questions is fixed by `MAX_QUESTIONS` (200)
2. Maximum text length per field is fixed by `MAX_TEXT` (1000)
3. Input file must strictly follow the required format
4. Uses text-based file storage only
5. No graphical user interface
6. No prevention of question repetition across different booklets



12. Project Requirements Compliance

This project satisfies all project requirements:

✅ Uses structures to represent questions  
✅ Stores data in arrays  
✅ Implements random selection and shuffling  
✅ Generates multiple exam booklets  
✅ Produces corresponding answer sheets  
✅ Uses file input and output  
✅ Modular and readable program structure  
✅ Comprehensive error handling  
✅ Detailed inline comments for every line of code  



13. Files Included
project/
├── main.c              # Source code
├── sample.txt          # Question bank (input)
├── README.md           # This documentation
├── booklet_X.txt       # Generated booklets (output)
└── answer_X.txt        # Generated answers (output)


14. Troubleshooting

| Problem | Solution |
|---------|----------|
| "Loaded 0 questions from file" | Check if `sample.txt` exists and format is correct |
| "Cannot open input file" | Ensure `sample.txt` is in the same directory |
| "Not enough questions in bank!" | Reduce questions per booklet or add more questions |
| "Invalid input!" | Enter positive integers only |


End of Documentation