#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>  

#define MAX 100

// structure for machinery info
typedef struct Machinery {
    char chassis[20];
    char make[20];
    char model[20];
    int year;
    float cost;
    float valuation;
    int mileage;
    int nextService;
    char ownerName[50];
    char email[50];
    char phone[20];
    char type[20];
    char breakdowns[30];
    struct Machinery* next;
} Machinery;

// structure for login info
typedef struct Login {
    char username[20];
    char password[20];
    struct Login* next;
} Login;

Machinery* head = NULL;
Login* loginHead = NULL;

// function declarations
void loadLoginFromFile();
void loginPrompt();
void getPassword(char* password);
int validateLogin(char* user, char* pass);
void loadFleetFromFile();
void saveFleetToFile();
void addMachine();
int uniqueChassis(char* chassis);
void displayAll();
void displayMachineDetails();
void updateMachine();
void deleteMachine();
void generateStatistics();
void printReportToFile();
void sortByValuation();
int isValidEmail(char* email);

// main menu
void main() {
    int choice;

    loadLoginFromFile();
    loginPrompt();      
    loadFleetFromFile(); 

    while (1) {
        
        printf("\n===== Machinery Management Menu =====\n");
        printf("1. Add machine\n");
        printf("2. Display all machines\n");
        printf("3. Display machine details by chassis\n");
        printf("4. Update a machine's details\n");
        printf("5. Delete a machine\n");
        printf("6. Generate statistics\n");
        printf("7. Print report to file\n");
        printf("8. List machines by current valuation\n");
        printf("9. Exit and Save\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) addMachine();
        else if (choice == 2) displayAll();
        else if (choice == 3) displayMachineDetails();
        else if (choice == 4) updateMachine();
        else if (choice == 5) deleteMachine();
        else if (choice == 6) generateStatistics();
        else if (choice == 7) printReportToFile();
        else if (choice == 8) sortByValuation();
        else if (choice == 9) {
            saveFleetToFile();
            printf("Exiting. Data saved to fleet.txt\n");
            break;
        }
        else printf("Invalid choice.\n");
    }
}

// load login data from file
void loadLoginFromFile() {
    FILE* f = fopen("login.txt", "r");
    if (!f) {
        printf("Login file not found.\n");
        exit(1);
    }

    char user[20], pass[20];
    while (fscanf(f, "%s %s", user, pass) == 2) {
        Login* newLogin = malloc(sizeof(Login));
        strcpy(newLogin->username, user);
        strcpy(newLogin->password, pass);
        newLogin->next = loginHead;
        loginHead = newLogin;
    }

    fclose(f);
}

// mask password with asterisks
void getPassword(char* password) {
    int i = 0;
    char ch;
    while (1) {
        ch = _getch();
        if (ch == 13) break; 
        if (ch == 8 && i > 0) {
            i--;
            printf("\b \b");
        }
        else if (i < 19 && ch != 8) {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
}

// check login
int validateLogin(char* user, char* pass) {
    Login* temp = loginHead;
    while (temp) {
        if (strcmp(temp->username, user) == 0 && strcmp(temp->password, pass) == 0)
            return 1;
        temp = temp->next;
    }
    return 0;
}

// login screen
void loginPrompt() {
    char user[20], pass[20];
    while (1) {
        printf("Username: ");
        scanf("%s", user); 
        printf("Password: ");
        getPassword(pass); 
        printf("\n");

        if (validateLogin(user, pass)) break;
        else printf("Invalid credentials. Try again.\n");
    }
}

// load machinery data
void loadFleetFromFile() {
    FILE* f = fopen("fleet.txt", "r");
    if (!f) return;

    Machinery* temp;
    while (!feof(f)) {
        temp = malloc(sizeof(Machinery));
        if (fscanf(f, "%s %s %s %d %f %f %d %d %s %s %s %s %s",
            temp->chassis, temp->make, temp->model, &temp->year,
            &temp->cost, &temp->valuation, &temp->mileage,
            &temp->nextService, temp->ownerName, temp->email,
            temp->phone, temp->type, temp->breakdowns) == 13) {
            temp->next = head;
            head = temp;
        }
    }

    fclose(f);
}

// save data back to file
void saveFleetToFile() {
    FILE* f = fopen("fleet.txt", "w");
    Machinery* temp = head;
    while (temp) {
        fprintf(f, "%s %s %s %d %.2f %.2f %d %d %s %s %s %s %s\n",
            temp->chassis, temp->make, temp->model, temp->year,
            temp->cost, temp->valuation, temp->mileage,
            temp->nextService, temp->ownerName, temp->email,
            temp->phone, temp->type, temp->breakdowns);
        temp = temp->next;
    }
    fclose(f);
}

// check if chassis is already used
int uniqueChassis(char* ch) {
    Machinery* temp = head;
    while (temp) {
        if (strcmp(temp->chassis, ch) == 0) return 0;
        temp = temp->next;
    }
    return 1;
}

// check email format
int isValidEmail(char* email) {
    return strstr(email, "@") && strstr(email, ".") && strstr(email, ".com");
}

// add new machine
void addMachine() {
    Machinery* newM = malloc(sizeof(Machinery));

    printf("Enter chassis number: ");
    scanf("%s", newM->chassis);

    if (!uniqueChassis(newM->chassis)) {
        printf("Chassis number must be unique.\n");
        free(newM);
        return;
    }

    printf("Make: "); scanf("%s", newM->make);
    printf("Model: "); scanf("%s", newM->model);
    printf("Year: "); scanf("%d", &newM->year);
    printf("Cost: "); scanf("%f", &newM->cost);
    printf("Current Valuation: "); scanf("%f", &newM->valuation);
    printf("Current Mileage: "); scanf("%d", &newM->mileage);
    printf("Next Service Mileage: "); scanf("%d", &newM->nextService);
    printf("Owner Name: "); scanf("%s", newM->ownerName);

    do {
        printf("Owner Email: ");
        scanf("%s", newM->email);
        if (!isValidEmail(newM->email)) {
            printf("Invalid email. Try again.\n");
        }
    } while (!isValidEmail(newM->email));

    printf("Owner Phone: "); scanf("%s", newM->phone);
    printf("Machine Type (Tractor/Excavator/Roller/Crane/Mixer): ");
    scanf("%s", newM->type);
    printf("Breakdowns this year (Never/LessThan3/LessThan5/MoreThan5): ");
    scanf("%s", newM->breakdowns);

    newM->next = head;
    head = newM;

    printf("Machine added.\n");
}

// show all machines
void displayAll() {
    Machinery* temp = head;
    while (temp) {
        printf("\n[%s] %s %s - %d | Valuation: %.2f | Owner: %s\n",
            temp->chassis, temp->make, temp->model, temp->year,
            temp->valuation, temp->ownerName);
        temp = temp->next;
    }
}

// show one machine
void displayMachineDetails() {
    char ch[20];
    printf("Enter chassis number: ");
    scanf("%s", ch);

    Machinery* temp = head;
    while (temp) {
        if (strcmp(temp->chassis, ch) == 0) {
            // display machine details
            printf("\nChassis: %s\nMake: %s\nModel: %s\nYear: %d\nCost: %.2f\nValuation: %.2f\n",
                temp->chassis, temp->make, temp->model, temp->year,
                temp->cost, temp->valuation);
            printf("Mileage: %d | Next Service: %d\n", temp->mileage, temp->nextService);
            printf("Owner: %s | Email: %s | Phone: %s\n", temp->ownerName, temp->email, temp->phone);
            printf("Type: %s | Breakdowns: %s\n", temp->type, temp->breakdowns);
            return;
        }
        temp = temp->next;
    }
    printf("Machine not found.\n");
}

// change valuation
void updateMachine() {
    char ch[20];
    printf("Enter chassis number to update: ");
    scanf("%s", ch);

    Machinery* temp = head;
    while (temp) {
        if (strcmp(temp->chassis, ch) == 0) {
            printf("Enter new valuation: ");
            scanf("%f", &temp->valuation);
            printf("Updated.\n");
            return;
        }
        temp = temp->next;
    }

    printf("Machine not found.\n");
}

// remove machine
void deleteMachine() {
    char ch[20];
    printf("Enter chassis number to delete: ");
    scanf("%s", ch);

    Machinery* curr = head;
    Machinery* prev = NULL;

    while (curr) {
        if (strcmp(curr->chassis, ch) == 0) {
            if (prev) prev->next = curr->next;
            else head = curr->next;
            free(curr);
            printf("Deleted.\n");
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("Machine not found.\n");
}

// show stats on breakdowns
void generateStatistics() {
    Machinery* temp = head;
    int total = 0, none = 0, lt3 = 0, lt5 = 0, mt5 = 0;

    while (temp) {
        total++;
        if (strcmp(temp->breakdowns, "Never") == 0) none++;
        else if (strcmp(temp->breakdowns, "LessThan3") == 0) lt3++;
        else if (strcmp(temp->breakdowns, "LessThan5") == 0) lt5++;
        else mt5++;
        temp = temp->next;
    }

    if (total == 0) {
        printf("No data.\n");
        return;
    }

    // print % breakdowns
    printf("\nBreakdown Statistics:\n");
    printf("Never: %.2f%%\n", (none * 100.0) / total);
    printf("<3: %.2f%%\n", (lt3 * 100.0) / total);
    printf("<5: %.2f%%\n", (lt5 * 100.0) / total);
    printf(">5: %.2f%%\n", (mt5 * 100.0) / total);
}

// write full report to file
void printReportToFile() {
    FILE* f = fopen("report.txt", "w");
    Machinery* temp = head;

    while (temp) {
        fprintf(f, "%s %s %s %d %.2f %.2f %d %d %s %s %s %s %s\n",
            temp->chassis, temp->make, temp->model, temp->year,
            temp->cost, temp->valuation, temp->mileage, temp->nextService,
            temp->ownerName, temp->email, temp->phone, temp->type, temp->breakdowns);
        temp = temp->next;
    }

    fclose(f);
    printf("Report written to report.txt\n");
}

// sort by valuation (highest first)
void sortByValuation() {
    if (!head || !head->next) return;

    Machinery* sorted = NULL;

    while (head) {
        Machinery* curr = head;
        head = head->next;

        if (!sorted || curr->valuation > sorted->valuation) {
            curr->next = sorted;
            sorted = curr;
        }
        else {
            Machinery* temp = sorted;
            while (temp->next && temp->next->valuation > curr->valuation)
                temp = temp->next;
            curr->next = temp->next;
            temp->next = curr;
        }
    }

    head = sorted;
    printf("Sorted by valuation.\n");
}
