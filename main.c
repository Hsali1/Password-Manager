#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 100
#define MAX_CREDENTIALS 100
#define ENCRYPTION_KEY 'X' // XOR key
#define FILE_NAME "password_file.dat"
#define MASTER_PASSWORD "MASTER1"

typedef struct {
    char service[MAX_LEN];
    char username[MAX_LEN];
    char password[MAX_LEN];
} Credential;

Credential credentials[MAX_CREDENTIALS];
int credential_count = 0;

// Function declarations
void load_credentials();
void save_credentials();
void xor_encryption(char *text);
int authenticate();
void menu();
void add_credential();
void list_services();
void get_credential();
void delete_credential();
void search_services();

// Helper function
void to_lowercase(char *dest, const char *src) {
    for (int i = 0; src[i]; i++) {
        // equivalent to *(dest + i) = tolower(*(src + i));
        dest[i] = tolower(src[i]);
    }
    dest[strlen(src)] = '\0';
}


int main() {
    printf("Command-Line Password Manager\n");

    if (!authenticate()) {
        printf("Authentication failed. Exiting.\n");
        return 1;
    }
    load_credentials();
    menu();
    save_credentials();

    return 0;
}

// returns 1 if authentication succeeds
// returns 0 if it fails
int authenticate() {
    char input[MAX_LEN];
    printf("Enter master password: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    return strcmp(input, MASTER_PASSWORD) == 0;
}

// XOR again with the same key decrypts the text
void xor_encryption(char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        text[i] ^= ENCRYPTION_KEY;
    }
    text[strlen(text)] = '\0';
}

void load_credentials() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        // No file exists yet
        return;
    }

    char line[(3 * MAX_LEN) + 3]; // each line has service|username|password\0
    /*
        fgets() reads a single line from the file into the buffer line.
        It will stop reading when it:
            - encounters a newline \n
            - Or it fills the buffer (up to sizeof(line) - 1)
        It will also incluse the newline character at the end
        It will null terminate
    */
    while (fgets(line, sizeof(line), file)) {
        // Remove newline
        // strcspn(line, "\n") returns the index of the first occurrence of \n in line.
        line[strcspn(line, "\n")] = 0;

        // Decrypt line
        xor_encryption(line);
        
        // strtok returns a pointer to the first char
        // strncpy(destination, source, max_chars_to_copy);
        char *token = strtok(line, "|");
        if (token) strncpy(credentials[credential_count].service, token, MAX_LEN);

        token = strtok(NULL, "|");
        if (token) strncpy(credentials[credential_count].username, token, MAX_LEN);

        token = strtok(NULL, "|");
        if (token) strncpy(credentials[credential_count].password, token, MAX_LEN);

        credential_count++;
    }

    fclose(file);
}

void menu() {
    int choice;
    char input[MAX_LEN];

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Add Credential\n");
        printf("2. List Services\n");
        printf("3. Get Credential\n");
        printf("4. Delete Credential\n");
        printf("5. Search\n");
        printf("6. Save and Exit\n");
        printf("Choose an option: ");

        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (sscanf(input, "%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1: add_credential(); break;
            case 2: list_services(); break;
            case 3: get_credential(); break;
            case 4: delete_credential(); break;
            case 5: search_services(); break;
            case 6: return;
            default: printf("Invalid choice.\n");
        }
    }
}

void add_credential() {
    if (credential_count >= MAX_CREDENTIALS) {
        printf("Max Credentials Reached!\n");
        return;
    }

    char service[MAX_LEN], username[MAX_LEN], password[MAX_LEN];

    printf("-----------\n");
    printf("Enter service name: ");
    fgets(service, sizeof(service), stdin);
    service[strcspn(service, "\n")] = '\0';

    // Make lowercase
    char service_lower[MAX_LEN], cred_service_lower[MAX_LEN];

    to_lowercase(service_lower, service);

    // Check for duplicate
    for (int i = 0; i < credential_count; i++) {
        to_lowercase(cred_service_lower, credentials[i].service);
        if (strcmp(cred_service_lower, service_lower) == 0) {
            printf("-----------\n");
            printf("Service already exists. Try Updating or Deleting\n");
            return;
        }
    }

    printf("-----------\n");
    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("-----------\n");
    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    strncpy(credentials[credential_count].service, service, MAX_LEN);
    strncpy(credentials[credential_count].username, username, MAX_LEN);
    strncpy(credentials[credential_count].password, password, MAX_LEN);

    credential_count++;
    printf("-----------\n");
    printf("Credential added!\n");
}

void save_credentials() {
    // Open (or create) the file "password_file.dat" in write mode ("w")
    FILE *file = fopen(FILE_NAME, "w");
    if (!file) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    for (int i = 0; i < credential_count; i++) {
        char line[3 * MAX_LEN + 3];

        // Format: service|username|password
        snprintf(line, sizeof(line), "%s|%s|%s",
                 credentials[i].service,
                 credentials[i].username,
                 credentials[i].password);

        // Encrypt before saving
        xor_encryption(line);

        fprintf(file, "%s\n", line);
    }

    fclose(file);
    printf("-----------\n");
    printf("Credentials saved successfully.\n");
}


void list_services() {
    if (credential_count == 0) {
        printf("-----------\n");
        printf("Nothing to show\n");
        return;
    } else {
        printf("-----------\n");
        for (int i = 0; i < credential_count; i++) {
            printf("%d. %s\t|\n", i + 1, credentials[i].service);
        }
    }
}

void get_credential() {
    char service[MAX_LEN];

    printf("-----------\n");
    printf("Enter Service: ");
    fgets(service, sizeof(service), stdin);
    service[strcspn(service, "\n")] = '\0';

    // Make lowercase
    char service_lower[MAX_LEN], cred_service_lower[MAX_LEN];

    to_lowercase(service_lower, service);

    printf("-----------\n");

    for (int i = 0; i < credential_count; i++) {
        to_lowercase(cred_service_lower, credentials[i].service);
        if (strcmp(service_lower, cred_service_lower) == 0) {
            printf("Username: %s\n", credentials[i].username);
            printf("Password: %s\n", credentials[i].password);
            return;
        }
    }
    printf("-----------\n");
    printf("Service not found\n");
}

void delete_credential() {
    char service[MAX_LEN];

    printf("-----------\n");
    printf("Enter Service: ");
    fgets(service, sizeof(service), stdin);
    service[strcspn(service, "\n")] = '\0';

    // Make lowercase
    char service_lower[MAX_LEN], cred_service_lower[MAX_LEN];

    to_lowercase(service_lower, service);

    for (int i = 0; i < credential_count; i++) {
        to_lowercase(cred_service_lower, credentials[i].service);
        if (strcmp(service_lower, cred_service_lower) == 0) {
            for (int j = i; j < credential_count - 1; j++) {
                credentials[j] = credentials[j + 1];
            }
            credential_count--;
            printf("-----------\n");
            printf("Deleted successfully\n");
            return;
        }
    }
    printf("-----------\n");
    printf("Service not found\n");
}

void search_services() {
    char query[MAX_LEN];
    char lowered_query[MAX_LEN];
    char lowered_service[MAX_LEN];
    int found = 0;

    printf("-----------\n");
    printf("Enter partial service name to search: ");
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = '\0';
    to_lowercase(lowered_query, query);

    printf("-----------\n");
    printf("Matching services:\n");

    for (int i = 0; i < credential_count; i++) {
        to_lowercase(lowered_service, credentials[i].service);
        if (strstr(lowered_service, lowered_query)) {
            printf("- %s\n", credentials[i].service);
            found = 1;
        }
    }

    if (!found) {
        printf("No matches found.\n");
    }
}
