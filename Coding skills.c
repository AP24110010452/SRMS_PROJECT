#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------- FILE NAMES ------------------- */
#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

/* ------------------- STRUCTURE ------------------- */
struct Student
{
    int roll;
    char name[50];
    float marks;
};

/* ------------------- FUNCTION DECLARATIONS ------------------- */
void addStudent();
void displayStudents();
int loginSystem();
void SearchStudent();
void DeleteStudent();
void UpdateStudent();

/* ------------------- MAIN FUNCTION ------------------- */
int main()
{
    if (!loginSystem())
    {
        printf("\nLogin Failed. Exiting...\n");
        return 0;
    }

    int choice;
    do
    {
        printf("\n===== STUDENT MANAGEMENT SYSTEM =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Delete Student\n");
        printf("5. Update Student\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addStudent();
            break;
        case 2:
            displayStudents();
            break;
        case 3:
            SearchStudent();
            break;
        case 4:
            DeleteStudent();
            break;
        case 5:
            UpdateStudent();
            break;
        case 6:
            printf("Exiting Program...\n");
            break;
        default:
            printf("Invalid Choice! Try Again.\n");
        }
    } while (choice != 6);

    return 0;
}

/* ------------------- LOGIN SYSTEM ------------------- */
int loginSystem()
{
    FILE *file = fopen(CREDENTIAL_FILE, "r");

    // If credential file does not exist → create one
    if (file == NULL)
    {
        file = fopen(CREDENTIAL_FILE, "w");
        fprintf(file, "admin admin123\n");
        fclose(file);
        printf("\nLogin file not found — created default credentials.\n");
        printf("Default Username: admin\n");
        printf("Default Password: admin123\n\n");
        file = fopen(CREDENTIAL_FILE, "r");
    }

    char savedUser[50], savedPass[50];
    fscanf(file, "%s %s", savedUser, savedPass);
    fclose(file);

    char user[50], pass[50];
    printf("\n======== LOGIN SCREEN ========\n");
    printf("Username: ");
    scanf("%s", user);
    printf("Password: ");
    scanf("%s", pass);

    if (strcmp(user, savedUser) == 0 && strcmp(pass, savedPass) == 0)
    {
        printf("\nLogin Successful!\n");
        return 1;
    }
    else
    {
        printf("\nInvalid Username or Password!\n");
        return 0;
    }
}

/* ------------------- ADD STUDENT ------------------- */
void addStudent()
{
    struct Student s;
    FILE *file = fopen(STUDENT_FILE, "a");

    printf("Enter Roll No: ");
    scanf("%d", &s.roll);
    printf("Enter Name: ");
    getchar();
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = 0;
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fprintf(file, "%d %s %.2f\n", s.roll, s.name, s.marks);
    fclose(file);

    printf("\nStudent Record Added Successfully!\n");
}

/* ------------------- DISPLAY STUDENTS ------------------- */
void displayStudents()
{
    struct Student s;
    FILE *file = fopen(STUDENT_FILE, "r");

    if (file == NULL)
    {
        printf("\nNo student records found.\n");
        return;
    }

    printf("\n----- STUDENT RECORDS -----\n");
    while (fscanf(file, "%d %s %f", &s.roll, s.name, &s.marks) != EOF)
    {
        printf("Roll: %d | Name: %s | Marks: %.2f\n", s.roll, s.name, s.marks);
    }
    fclose(file);
}

/* ------------------- SEARCH STUDENT ------------------- */
void SearchStudent()
{
    struct Student s;
    int rollToSearch;
    FILE *file = fopen(STUDENT_FILE, "r");
    if (file == NULL)
    {
        printf("\nNo student records found.\n");
        return;
    }
    printf("\nEnter Roll No to Search: ");
    scanf("%d", &rollToSearch);

    int found = 0;
    while (fscanf(file, "%d %s %f", &s.roll, s.name, &s.marks) != EOF)
    {
        if (s.roll == rollToSearch)
        {
            printf("Roll: %d | Name: %s | Marks: %.2f\n", s.roll, s.name, s.marks);
            found = 1;
            break;
        }
    }
    fclose(file);

    if (!found)
    {
        printf("\nStudent with Roll No %d not found.\n", rollToSearch);
    }
}
/* ------------------- DELETE STUDENT ------------------- */
void DeleteStudent()
{
    struct Student s;
    int rollToDelete;
    FILE *file = fopen(STUDENT_FILE, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL)
    {
        printf("\nNo student records found.\n");
        return;
    }
    printf("\nEnter Roll No to Delete: ");
    scanf("%d", &rollToDelete);

    int found = 0;
    while (fscanf(file, "%d %s %f", &s.roll, s.name, &s.marks) != EOF)
    {
        if (s.roll == rollToDelete)
        {
            found = 1;
            continue; // Skip writing this record to delete it
        }
        fprintf(tempFile, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }
    fclose(file);
    fclose(tempFile);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
    {
        printf("\nStudent with Roll No %d deleted successfully.\n", rollToDelete);
    }
    else
    {
        printf("\nStudent with Roll No %d not found.\n", rollToDelete);
    }
}
/* ------------------- UPDATE STUDENT ------------------- */
void UpdateStudent()
{
    struct Student s;
    int rollToUpdate;
    FILE *file = fopen(STUDENT_FILE, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL)
    {
        printf("\nNo student records found.\n");
        return;
    }
    printf("\nEnter Roll No to Update: ");
    scanf("%d", &rollToUpdate);

    int found = 0;
    while (fscanf(file, "%d %s %f", &s.roll, s.name, &s.marks) != EOF)
    {
        if (s.roll == rollToUpdate)
        {
            found = 1;
            printf("Enter New Name: ");
            getchar();
            fgets(s.name, sizeof(s.name), stdin);
            s.name[strcspn(s.name, "\n")] = 0;
            printf("Enter New Marks: ");
            scanf("%f", &s.marks);
        }
        fprintf(tempFile, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }
    fclose(file);
    fclose(tempFile);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
    {
        printf("\nStudent with Roll No %d updated successfully.\n", rollToUpdate);
    }
    else
    {
        printf("\nStudent with Roll No %d not found.\n", rollToUpdate);
    }
}
