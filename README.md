# Password Manager Project

## Features/Objectives:

    - Add, delete, get credentials - Based on unique service names (eg: Google, Netflix, etc.) 
    - Save to encrypted file - XOR-based encryption on disk
    - Load from encrypted file - Decryption on program start
    - Master password - Required to access the program
    - Prevent duplicate services - Doesn't allow repeated entries for the same service
    - Search functionality - Search by partial match of service
    - Partial Search - Allow partial searches to find all services
    - Command-line UI - Clear menu and formatted output

## Sample File

Data will be stored in a file with the format:
```
service|username|password
gmail|hassan@gmail.com|StronkPasZ
netflix|hassan123|password123
```
But encrypted.

### For my reference

Functions:
    string.h
        - strncpy(dest, src, n) - Copies up to n characters from src to dest. Used for copying strings safely.
        - strcmp(a, b) - Compares two strings. Returns 0 if they are equal.
        - strtok(str, delim) - Tokenizes a string — splits it into chunks by a delimiter
        - strcspn(str, "\n") - Returns the index of the first newline character \n. I use it to strip trailing newlines.
    stdio.h	
        - fgets(buffer, size, stdin) - Reads a line of input from stdin (or file), including spaces, and stops at newline or buffer size.
        - fgets(buffer, size, file)	- Reads a line from a file.
        - sscanf(input, "%d", &var)	- Parses an integer (or other formatted value) from a string. Safer than scanf().
        - snprintf(dest, size, format, ...)	- Safely formats a string into a buffer.
        - fopen(filename, mode)	- Opens a file (for reading or writing).
        - fclose(file) - Closes an open file.
        - fprintf(file, format, ...) - Writes formatted output to a file (like printf but for files).

## Notes

Could use Macros to make the code cleaner.

## Example run:
```
Command-Line Password Manager
Enter master password: MASTER1

--- MENU ---
1. Add Credential
2. List Services
3. Get Credential
4. Delete Credential
5. Search
6. Save and Exit
Choose an option: 2
-----------
1. Amazon       |
2. Netflix      |

--- MENU ---
1. Add Credential
2. List Services
3. Get Credential
4. Delete Credential
5. Search
6. Save and Exit
Choose an option: 1
-----------
Enter service name: Google
-----------
Enter username: googler
-----------
Enter password: 33444
-----------
Credential added!

--- MENU ---
1. Add Credential
2. List Services
3. Get Credential
4. Delete Credential
5. Search
6. Save and Exit
Choose an option: 3
-----------
Enter Service: GoOgLe
-----------
Username: googler
Password: 33444

--- MENU ---
1. Add Credential
2. List Services
3. Get Credential
4. Delete Credential
5. Search
6. Save and Exit
Choose an option: 1
-----------
Enter service name: amaaaaoo
-----------
Enter username: ssssss
-----------
Enter password: 222
-----------
Credential added!

--- MENU ---
1. Add Credential
2. List Services
3. Get Credential
4. Delete Credential
5. Search
6. Save and Exit
Choose an option: 5
-----------
Enter partial service name to search: ama
-----------
Matching services:
- Amazon
- amaaaaoo

--- MENU ---
1. Add Credential
2. List Services
3. Get Credential
4. Delete Credential
5. Search
6. Save and Exit
Choose an option: 4
-----------
Enter Service: amAAAAoo
-----------
Deleted successfully

--- MENU ---
1. Add Credential
2. List Services
3. Get Credential
4. Delete Credential
5. Search
6. Save and Exit
Choose an option: 2
-----------
1. Amazon       |
2. Netflix      |
3. Google       |

--- MENU ---
1. Add Credential
2. List Services
3. Get Credential
4. Delete Credential
5. Search
6. Save and Exit
Choose an option: 6
-----------
Credentials saved successfully.
```