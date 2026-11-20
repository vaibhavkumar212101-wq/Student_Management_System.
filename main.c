#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "students.dat"
#define NAME_LEN 100
#define COURSE_LEN 50
#define PHONE_LEN 20
#define EMAIL_LEN 100

typedef struct {
    char name[NAME_LEN];
    int roll;
    char course[COURSE_LEN];
    int semester;
    float marks;
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
} Student;

void clearInputBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

void readLine(char *buffer, size_t size) {
    if (fgets(buffer, (int)size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            clearInputBuffer();
        }
    }
}

int findIndexByRoll(const Student *arr, int count, int roll) {
    for (int i = 0; i < count; i++) {
        if ((arr + i)->roll == roll) {
            return i;
        }
    }
    return -1;
}

Student* loadStudents(int *count) {
    FILE *fp = fopen(DATA_FILE, "rb");
    *count = 0;

    if (fp == NULL) {
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    rewind(fp);

    int total = (int)(fileSize / sizeof(Student));
    if (total <= 0) {
        fclose(fp);
        return NULL;
    }

    Student *arr = (Student *)malloc((size_t)total * sizeof(Student));
    if (arr == NULL) {
        fclose(fp);
        printf("Memory allocation failed!\n");
        return NULL;
    }

    size_t readCount = fread(arr, sizeof(Student), (size_t)total, fp);
    *count = (int)readCount;
    fclose(fp);
    return arr;
}

int saveStudents(const Student *arr, int count) {
    FILE *fp = fopen(DATA_FILE, "wb");
    if (fp == NULL) {
        printf("Error: Could not open %s for writing.\n", DATA_FILE);
        return 0;
    }

    if (count > 0 && arr != NULL) {
        size_t written = fwrite(arr, sizeof(Student), (size_t)count, fp);
        if (written != (size_t)count) {
            printf("Error: Failed to write all student records.\n");
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    return 1;
}

void printStudentCard(const Student *s) {
    printf("--------------------------------------------------\n");
    printf("Name       : %s\n", s->name);
    printf("Roll No    : %d\n", s->roll);
    printf("Course     : %s\n", s->course);
    printf("Semester   : %d\n", s->semester);
    printf("Marks      : %.2f\n", s->marks);
    printf("Phone      : %s\n", s->phone);
    printf("Email      : %s\n", s->email);
    printf("--------------------------------------------------\n");
}

void addStudent(Student **arr, int *count) {
    Student s;
    printf("\n--- Add New Student ---\n");

    printf("Enter Student Name : ");
    readLine(s.name, sizeof(s.name));

    printf("Enter Roll Number  : ");
    if (scanf("%d", &s.roll) != 1) {
        printf("Invalid roll number!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (findIndexByRoll(*arr, *count, s.roll) != -1) {
        printf("Error: Student with Roll %d already exists!\n", s.roll);
        return;
    }

    printf("Enter Course       : ");
    readLine(s.course, sizeof(s.course));

    printf("Enter Semester     : ");
    if (scanf("%d", &s.semester) != 1 || s.semester <= 0) {
        printf("Invalid semester!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    printf("Enter Marks        : ");
    if (scanf("%f", &s.marks) != 1 || s.marks < 0.0f) {
        printf("Invalid marks!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    printf("Enter Phone        : ");
    readLine(s.phone, sizeof(s.phone));

    printf("Enter Email        : ");
    readLine(s.email, sizeof(s.email));

    Student *temp = (Student *)realloc(*arr, (size_t)(*count + 1) * sizeof(Student));
    if (temp == NULL) {
        printf("Error: Memory reallocation failed!\n");
        return;
    }

    *arr = temp;
    (*arr)[*count] = s;
    (*count)++;

    if (saveStudents(*arr, *count)) {
        printf("Student added and saved successfully.\n");
    }
}

void displayStudents(const Student *arr, int count) {
    printf("\n=== All Student Records (%d) ===\n", count);
    if (count == 0 || arr == NULL) {
        printf("No records found.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printf("\nRecord #%d:\n", i + 1);
        printStudentCard(arr + i);
    }
}

void searchStudent(const Student *arr, int count) {
    int roll;
    printf("\n--- Search Student ---\n");
    printf("Enter Roll Number to search: ");
    if (scanf("%d", &roll) != 1) {
        printf("Invalid input!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int idx = findIndexByRoll(arr, count, roll);
    if (idx == -1) {
        printf("Student with Roll %d not found.\n", roll);
        return;
    }

    printf("\nStudent found:\n");
    printStudentCard(arr + idx);
}

void updateStudent(Student *arr, int count) {
    int roll;
    printf("\n--- Update Student ---\n");
    printf("Enter Roll Number to update: ");
    if (scanf("%d", &roll) != 1) {
        printf("Invalid input!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int idx = findIndexByRoll(arr, count, roll);
    if (idx == -1) {
        printf("Student with Roll %d not found.\n", roll);
        return;
    }

    Student *s = arr + idx;
    printf("Updating Record (Current Name: %s | Roll No: %d)\n", s->name, s->roll);

    printf("Enter New Student Name : ");
    readLine(s->name, sizeof(s->name));

    printf("Enter New Course       : ");
    readLine(s->course, sizeof(s->course));

    printf("Enter New Semester     : ");
    if (scanf("%d", &s->semester) != 1 || s->semester <= 0) {
        printf("Invalid semester! Update cancelled.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    printf("Enter New Marks        : ");
    if (scanf("%f", &s->marks) != 1 || s->marks < 0.0f) {
        printf("Invalid marks! Update cancelled.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    printf("Enter New Phone        : ");
    readLine(s->phone, sizeof(s->phone));

    printf("Enter New Email        : ");
    readLine(s->email, sizeof(s->email));

    if (saveStudents(arr, count)) {
        printf("Student record updated successfully.\n");
    }
}

void deleteStudent(Student **arr, int *count) {
    int roll;
    printf("\n--- Delete Student ---\n");
    printf("Enter Roll Number to delete: ");
    if (scanf("%d", &roll) != 1) {
        printf("Invalid input!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int idx = findIndexByRoll(*arr, *count, roll);
    if (idx == -1) {
        printf("Student with Roll %d not found.\n", roll);
        return;
    }

    for (int i = idx; i < (*count) - 1; i++) {
        *(*arr + i) = *(*arr + i + 1);
    }
    (*count)--;

    if (*count == 0) {
        free(*arr);
        *arr = NULL;
    } else {
        Student *temp = (Student *)realloc(*arr, (size_t)(*count) * sizeof(Student));
        if (temp != NULL) {
            *arr = temp;
        }
    }

    if (saveStudents(*arr, *count)) {
        printf("Student record deleted successfully.\n");
    }
}

void printMenu(void) {
    printf("\n==============================\n");
    printf("   STUDENT MANAGEMENT SYSTEM\n");
    printf("==============================\n");
    printf("1. Add Student\n");
    printf("2. Display All Students\n");
    printf("3. Search Student by Roll\n");
    printf("4. Update Student\n");
    printf("5. Delete Student\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
}

int main(void) {
    int count = 0;
    Student *students = loadStudents(&count);
    int choice = 0;

    do {
        printMenu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid choice! Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                addStudent(&students, &count);
                break;
            case 2:
                displayStudents(students, count);
                break;
            case 3:
                searchStudent(students, count);
                break;
            case 4:
                updateStudent(students, count);
                break;
            case 5:
                deleteStudent(&students, &count);
                break;
            case 6:
                printf("Exiting program. Data saved in %s.\n", DATA_FILE);
                break;
            default:
                printf("Invalid choice! Select between 1 and 6.\n");
                break;
        }
    } while (choice != 6);

    free(students);
    students = NULL;
    return 0;
}
