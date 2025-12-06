#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE      "student.txt"
#define CREDENTIALS_FILE  "credentials.txt"

struct Student {
    int roll;
    char name[50];
    float marks;
};

char currentRole[10];
char currentUser[50];

/* Prototypes */
int loginSystem(void);
void mainMenu(void);

void adminMenu(void);
void staffMenu(void);
void userMenu(void);
void guestMenu(void);

void addStudent(void);
void displayStudents(void);
void searchStudent(void);
void updateStudent(void);
void deleteStudent(void);
int  isDuplicateRoll(int);

/* New prototype */
void changePassword(void);

/* ================= MAIN ================= */
int main(void) {
    if (loginSystem())
        mainMenu();
    else
        printf("\nLogin Failed!\n");

    return 0;
}

/* ================= LOGIN ================= */
int loginSystem(void) {
    char username[20], password[20];
    char fu[20], fp[20], fr[10];

    printf("\n=== LOGIN ===\n");
    printf("Username: ");
    scanf("%19s", username);
    printf("Password: ");
    scanf("%19s", password);

    FILE *file = fopen(CREDENTIALS_FILE, "r");
    if (!file) {
        printf("credentials.txt missing!\n");
        return 0;
    }

    while (fscanf(file, "%19s %19s %9s", fu, fp, fr) == 3) {
        if (strcmp(username, fu) == 0 && strcmp(password, fp) == 0) {
            strcpy(currentUser, fu);
            strcpy(currentRole, fr);
            fclose(file);
            printf("\nWelcome %s (%s)\n", currentUser, currentRole);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

/* ================= MAIN MENU ================= */
void mainMenu(void) {
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if (strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else if (strcmp(currentRole, "USER") == 0)
        userMenu();
    else
        guestMenu();
}

/* ================= ADMIN ================= */
void adminMenu(void) {
    int ch;
    do {
        printf("\n--- ADMIN MENU ---\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search\n");
        printf("4. Update\n");
        printf("5. Delete\n");
        printf("6. Change Password\n");
        printf("7. Logout\n");
        printf("Choice: ");
        if (scanf("%d", &ch) != 1) { /* simple input guard */
            while (getchar() != '\n'); /* flush */
            ch = 0;
        }

        switch (ch) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: changePassword(); break;
            case 7: /* logout */ break;
            default: if (ch != 7) printf("Invalid choice. Try again.\n");
        }
    } while (ch != 7);
}

/* ================= STAFF ================= */
void staffMenu(void) {
    int ch;
    do {
        printf("\n--- STAFF MENU ---\n");
        printf("1. Display\n2. Search\n3. Update\n4. Logout\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: updateStudent(); break;
        }
    } while (ch != 4);
}

/* ================= USER ================= */
void userMenu(void) {
    int ch, roll, found;
    struct Student st;
    FILE *fp;

    do {
        printf("\n--- USER MENU ---\n");
        printf("1. View My Record\n2. Logout\n");
        printf("Choice: ");
        scanf("%d", &ch);

        if (ch == 1) {
            printf("Enter your Roll Number: ");
            scanf("%d", &roll);

            fp = fopen(STUDENT_FILE, "r");
            if (!fp) {
                printf("No records found!\n");
                continue;
            }

            found = 0;
            while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
                if (st.roll == roll) {
                    printf("\nROLL : %d\nNAME : %s\nMARKS: %.2f\n",
                           st.roll, st.name, st.marks);
                    found = 1;
                    break;
                }
            }
            if (!found) printf("Record not found!\n");
            fclose(fp);
        }

    } while (ch != 2);
}

/* ================= GUEST ================= */
void guestMenu(void) {
    printf("\n--- GUEST VIEW (READ ONLY) ---\n");
    displayStudents();
    printf("\nGuest session ended.\n");
}

/* ================= UTILS ================= */
int isDuplicateRoll(int roll) {
    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) return 0;
    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

/* ================= ADD ================= */
void addStudent(void) {
    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "a");

    if (!fp) {
        printf("File error!\n");
        return;
    }

    printf("Roll: ");
    if (scanf("%d", &st.roll) != 1) {
        printf("Invalid roll number!\n");
        fclose(fp);
        while (getchar() != '\n');
        return;
    }

    if (isDuplicateRoll(st.roll)) {
        printf("Roll exists!\n");
        fclose(fp);
        return;
    }

    printf("Name: ");
    scanf("%49s", st.name);
    printf("Marks: ");
    scanf("%f", &st.marks);

    fprintf(fp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    fclose(fp);
    printf("Added successfully!\n");
}

/* ================= DISPLAY ================= */
void displayStudents(void) {
    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("No data available!\n");
        return;
    }

    printf("\nRoll\tName\tMarks\n");
    printf("-------------------\n");

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3)
        printf("%d\t%s\t%.2f\n", st.roll, st.name, st.marks);

    fclose(fp);
}

/* ================= SEARCH ================= */
void searchStudent(void) {
    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    int roll, found = 0;

    if (!fp) {
        printf("No records!\n");
        return;
    }

    printf("Search Roll: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            printf("Found => %d %s %.2f\n",
                   st.roll, st.name, st.marks);
            found = 1;
            break;
        }
    }

    if (!found) printf("Not found!\n");
    fclose(fp);
}

/* ================= UPDATE ================= */
void updateStudent(void) {
    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *tmp = fopen("temp.txt", "w");
    int roll, found = 0;

    if (!fp || !tmp) {
        if (fp) fclose(fp);
        if (tmp) fclose(tmp);
        printf("File error!\n");
        return;
    }

    printf("Roll to Update: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            printf("New Name: ");
            scanf("%49s", st.name);
            printf("New Marks: ");
            scanf("%f", &st.marks);
            found = 1;
        }
        fprintf(tmp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp); fclose(tmp);
    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found) printf("Updated!\n");
    else printf("Not found!\n");
}

/* ================= DELETE ================= */
void deleteStudent(void) {
    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *tmp = fopen("temp.txt", "w");
    int roll, found = 0;

    if (!fp || !tmp) {
        if (fp) fclose(fp);
        if (tmp) fclose(tmp);
        printf("File error!\n");
        return;
    }

    printf("Roll to delete: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            found = 1;
            continue;
        }
        fprintf(tmp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp); fclose(tmp);
    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found) printf("Deleted.\n");
    else printf("Not found.\n");
}

/* ================= CHANGE PASSWORD ================= */
/* This function updates the current user's password in credentials.txt */
void changePassword(void) {
    char oldPass[20], newPass[20];
    char fu[20], fpw[20], fr[10];
    int found = 0;

    FILE *fp = fopen(CREDENTIALS_FILE, "r");
    if (!fp) {
        printf("Credentials file not found!\n");
        return;
    }

    FILE *tmp = fopen("temp_cred.txt", "w");
    if (!tmp) {
        printf("Unable to open temporary file!\n");
        fclose(fp);
        return;
    }

    printf("Enter old password: ");
    scanf("%19s", oldPass);

    while (fscanf(fp, "%19s %19s %9s", fu, fpw, fr) == 3) {
        if (strcmp(fu, currentUser) == 0) {
            if (strcmp(fpw, oldPass) == 0) {
                /* match - prompt for new password */
                printf("Enter new password: ");
                scanf("%19s", newPass);
                fprintf(tmp, "%s %s %s\n", fu, newPass, fr);
                found = 1;
            } else {
                /* username matched but old password incorrect - keep original */
                fprintf(tmp, "%s %s %s\n", fu, fpw, fr);
            }
        } else {
            /* other users - copy as is */
            fprintf(tmp, "%s %s %s\n", fu, fpw, fr);
        }
    }

    fclose(fp);
    fclose(tmp);

    /* Replace original credentials file with updated temp file */
    if (remove(CREDENTIALS_FILE) != 0) {
        printf("Error updating credentials file!\n");
        /* cleanup temp */
        remove("temp_cred.txt");
        return;
    }

    if (rename("temp_cred.txt", CREDENTIALS_FILE) != 0) {
        printf("Error renaming credentials file!\n");
        return;
    }

    if (found)
        printf("Password changed successfully!\n");
    else
        printf("Old password incorrect or user not found. No change made.\n");
}
