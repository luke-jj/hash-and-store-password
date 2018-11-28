/*
 * Copyright (c) 2018 - Luca Joos - MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * @author  Luca Joos
 * @date    2018-11-28
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/sha.h>

/*
 * Simple demo program that demonstrates hashing and storing a user-password.
 * Cesar-Cipher vs SHA-256 vs bcrypt
 *
 * WARNING: This program does intentionally not implement error handling
 * such as checking whether a file was opened correctly, user input is valid,
 * or memory was allocated/deallocated correctly.
 *
 * openssl instructions:
 * 1. download and install the latest version from https://www.openssl.org
 * 2. create symlinks so the compiler can find <openssl/sha.h>
 *      cd /usr/local/include
 *      ln -s ../opt/openssl/include/openssl .
 */
#define ROWS_MAX 100

/*
 * The following three data structures are used to persist user data
 * in a file on the local disk.
 */
struct Userdata {
    int     id;
    char    username[11];
    char    password[9];
};

struct Database {
    struct Userdata     rows[ROWS_MAX];
};

struct Connection {
    FILE                *file;
    struct Database     *db;
};



/*
 * Open connection to the database file
 */
struct Connection *connect(const char *filename)
{
    struct Connection *connection = malloc(sizeof(struct Connection));
    connection->db = malloc(sizeof(struct Database));
    connection->file = fopen(filename, "r+");

    return connection;
}


char *Userdata_tostring(struct Userdata *data)
{
    char *format_string;
    sprintf(format_string,
            "%d,%s,%s\n",
            data->id,
            data->username,
            data->password);
    return format_string;
}





/* TODO: open connection */


void store(char *username, char *encrypted_password)
{
}


/*
 * Hashing the input with cesars cipher.
 */
char *hash(char *password)
{
    char *cipher = malloc(sizeof(password));
    // caesar-cipher just for fun ^_^
    for (int i = 0; i < strlen(password); i++) {
        cipher[i] = password[i] + 1;
    }
    return cipher;
}

/*
 * fully functional on unix compatible systems and shells
 * limited support for windows
 */
void clear_screen()
{
    printf("\033[H\033[2J");
}

/*
 * main menu feature that guides the user through creating a new login
 * @return      int     standard error code
 */
int register_user()
{
    char    username[256];
    char    password[256];
    int     username_is_valid = 0;
    int     password_is_valid = 0;
    int     is_alnum;
    int     i;

    while (!username_is_valid) {
        /* clear_screen(); */
        printf("Please enter a valid username (A-Za-z0-9, max. 10 characters)");
        printf("\n >  ");
        scanf("%s", username);
        is_alnum = 1;

        /* check that username is shorter than 11 characters and alphanumeric */
        for (i = 0; i < strlen(username); i++) {
            if (!isalnum(username[i])) {
                is_alnum = 0;
            }
        }
        if (strlen(username) < 10 && is_alnum) {
            username_is_valid = 1;
        }

    }
    while (!password_is_valid) {
        /* clear_screen(); */
        printf("Username: %s\n", username);
        printf("Please enter a valid password (A-Za-z0-9, max. 8 characters)");
        printf("\n >  ");
        scanf("%s", password);
        is_alnum = 1;

        /* check that password is shorter than 9 characters and alphanumeric */
        for (i = 0; i < strlen(password); i++) {
            if (!isalnum(password[i])) {
                is_alnum = 0;
            }
        }
        if (strlen(password) < 10 && is_alnum) {
            password_is_valid = 1;
        }
    }
    if (username_is_valid && password_is_valid) {
        char *hashed_password = hash(password);
        store(username, hashed_password);
        printf("Success! You can now login with username: %s\n", username);
        scanf(" %s", username);
    }

    free(hashed_password);
    return 0;
}

/*
 * main-menu feature that asks for and validates a users login details
 * @return      int     standard error code
 */
int login()
{
    return 0;
}

/*
 * print the main menu on screen and parse user input
 */
void main_menu()
{
    char login_name[32] = "--";
    char user_input;

    while(1) {
        clear_screen();
        printf("You are logged in as:  %s\n", login_name);
        printf("Register new user: [R]\n");
        printf("Log-in as existing user: [L]\n");
        printf("Exit: [Q]\n");
        printf(" >  ");
        scanf(" %c", &user_input);
        user_input = tolower(user_input);
        switch (user_input) {
            case 'r':
                register_user();
                break;
            case 'l':
                login();
                break;
            case 'q':
                exit(0);
            default:
                printf("Not a valid input");
                scanf(" %c", &user_input);
        }
    }
}

int main(int argc, char *argv[])
{
    main_menu();

    return 0;
}
