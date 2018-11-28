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

#define ROWS_MAX 100

/*
 * Simple demo program that demonstrates the hash of a user-password
 *
 * openssl instructions:
 * 1. download and install the latest version from https://www.openssl.org
 * 2. create symlinks so the compiler can find <openssl/sha.h>
 *      cd /usr/local/include
 *      ln -s ../opt/openssl/include/openssl .
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

void clear_screen()
{
    printf("\033[H\033[2J");
}

int register_user()
{
    char    username[256];
    char    password[256];
    int     username_is_valid = 0;
    int     password_is_valid = 0;
    int     i;

    while (!username_is_valid) {
        /* clear_screen(); */
        printf("Please enter a valid username (A-Za-z0-9, max. 10 characters)");
        printf("\n >  ");
        scanf("%s", username);
        if (strlen(username) > 10) {
            for (i = 0; i < strlen(username); i++) {
                if (!isalnum(username[i]) ) {
                    printf("not a valid username");
                }
            }
        }

    }
    while (!password_is_valid) {
        /* clear_screen(); */
        printf("Username: %s\n", username);
        printf("Please enter a valid password (A-Za-z0-9, max. 8 characters)");
        printf("\n >  ");
        scanf("%s", password);
    }
    if (username_is_valid && password_is_valid) {
        printf("Success! You can now login with username: %s\n", username);
    }

    return 0;
}

int check_password()
{
    return 0;
}

int main(int argc, char *argv[])
{
    char *login_name = "--";
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
                check_password();
                break;
            case 'q':
                exit(0);
            default:
                printf("Not a valid input");
                scanf(" %c", &user_input);
        }
    }

    return 0;
}
