/*
 * Copyright (c) 2018 - MIT License
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
 * @author  Luca J
 * @date    2018-11-28
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/sha.h>

/*
 * Simple demo program that demonstrates hashing and storing a user-password.
 * Caesar-Cipher vs SHA-256 vs bcrypt
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
    int                 id;
    char                username[11];
    char                password[9];
    int                 set;
};

struct Database {
    struct Userdata     rows[ROWS_MAX];
};

struct Connection {
    FILE                *file;
    struct Database     *db;
};



/***********************************
 * Stream & Connectivity Functions *
 *                                 *
 ***********************************/

/*
 * Open connection to the database file and initialize file stream
 * @param       *char           path to the database file
 * @return      *Connection     fully initialized connection
 */
struct Connection *connect(const char *filename)
{
    struct Connection *connection = malloc(sizeof(struct Connection));
    connection->db = malloc(sizeof(struct Database));
    connection->file = fopen(filename, "r+");

    return connection;
}

/*
 * Read the database file contents into memory through the 'file' stream
 * Populate the Database structure in 'connection' with data read from file
 */
void Database_load(struct Connection *connection)
{
    fread(connection->db, sizeof(struct Database), 1, connection->file);
}

/*
 * Create a new database in memory
 * database is indexed and otherwise empty,
 * number of rows is specified by the ROWS_MAX constant
 */
void Database_create(struct Connection *connection)
{
    for (int i = 0; i < ROWS_MAX; i++) {
        struct Userdata data;
        data.id = i;
        connection->db->rows[i] = data;
    }
}

/*
 * Write database in memory to file stream
 */
void Database_write(struct Connection *connection)
{
    rewind(connection->file);
    fwrite(connection->db, sizeof(struct Database), 1, connection->file);
    fflush(connection->file);
}

/*
 * Close database file stream, free database in memory, and connection
 */
void Database_close(struct Connection *connection)
{
    fclose(connection->file);
    free(connection->db);
    free(connection);
}



/***********************************
 * CRUD functions for database     *
 * - get                           *
 * - append                        *
 * - linear_search                 *
 ***********************************/

/*
 * get userdata by index
 * TODO: tests
 */
struct Userdata *Database_get(struct Connection *connection, int index)
{
    return &connection->db->rows[index];
}

/*
 * append newly registered user data to loaded database
 */
void Database_append(struct Connection *connection, char *user, char *pass)
{
    struct Userdata *data;
    for (int i = 0; i < ROWS_MAX; i++) {
        data = &connection->db->rows[i];
        if (!data->set) {
            strcpy(data->username, user);
            strcpy(data->password, pass);
            data->set = 1;
            break;
        }
    }
}

/*
 * search for username
 * @param   char*   username
 * @return  int     index associated with username
 * TODO: tests
 */
int Database_linear_search(struct Connection *connection, char *user)
{
    printf("linear search username: %s\n", connection->db->rows[0].username);
    for(int i = 0; i < ROWS_MAX; i++) {
        if (connection->db->rows[i].username == user) {
            return i;
        }
    }
    return -1;
}




/*
 * print contents of a usedata record
 */
void Userdata_print(struct Userdata *data)
{
    printf("Index: %d, Username: %s, Password: %s\n",
            data->id, data->username, data->password);
}

/*
 * Hashing the input with caesars cipher.
 */
char *hash(char *password)
{
    char *cipher = malloc(sizeof(password));
    for (int i = 0; i < strlen(password); i++) {
        cipher[i] = password[i] + 1;
    }
    return cipher;
}

/*
 * Store new user registration details in database
 */
void store(char *username, char *password)
{
    struct Connection *connection = connect("assets/user.db");
    Database_load(connection);
    Database_append(connection, username, password);
    Database_write(connection);
    Database_close(connection);
}

/*
 * Verify user login details
 * @return      int     true or false
 */
int verify(char *username, char *password)
{
    int index;
    char *hashed_password;
    struct Userdata *data;

    struct Connection *connection = connect("assets/user.db");
    Database_load(connection);
    printf("entered username: %s\n", username);
    printf("entered password: %s\n", password);
    index = Database_linear_search(connection, username);
    printf("Index returned: %d\n", index);
    data = Database_get(connection, index);
    hashed_password = hash(password);
    printf("hashed_password: %s\n", hashed_password);
    printf("returned password: %s\n", data->password);
    if (data->password == hashed_password) {
        free(hashed_password);
        Database_close(connection);
        return 1;
    }
    free(hashed_password);
    Database_close(connection);
    return 0;
}

/*
 * print all database entries
 */
void list()
{
    struct Userdata *data;
    struct Connection *connection = connect("assets/user.db");

    Database_load(connection);
    for (int i = 0; i < ROWS_MAX; i++) {
        data = Database_get(connection, i);
        Userdata_print(data);
    }
    Database_close(connection);
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
        if (strlen(username) < 11 && is_alnum) {
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
        if (strlen(password) < 9 && is_alnum) {
            password_is_valid = 1;
        }
    }
    if (username_is_valid && password_is_valid) {
        char    *hashed_password;

        hashed_password = hash(password);
        store(username, hashed_password);
        printf("Success! You can now login with username: %s\n", username);
        scanf(" %s", username);
        free(hashed_password);
    }

    return 0;
}

/*
 * main-menu feature that asks for and validates a users login details
 *
 * @return      int     standard error code
 */
int login(char * login_name)
{
    char username[64];
    char password[64];
    int is_verified;

    printf("Login username:\n");
    printf(" >  ");
    scanf("%s", username);
    printf("Password:\n");
    printf(" >  ");
    scanf("%s", password);

    is_verified = verify(username, password);
    if (is_verified) {
        login_name = username;
        printf("login successful\n");
    } else {
        printf("username or password did not match\n");
    }
    return 0;
}

/*
 * main-menu feature - Create new database file
 * this function will delete the old database file
 * @return      int     standard error code
 */
int create_db()
{
    struct Connection *connection = connect("assets/user.db");
    Database_create(connection);
    Database_write(connection);
    Database_close(connection);
    printf("New database file created\n");

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
        /* clear_screen(); */
        printf("You are logged in as:  %s\n", login_name);
        printf("Register new user: [R]\n");
        printf("Sign-in as existing user: [S]\n");
        printf("List database record: [L]\n");
        printf("Reset and re-initialize database (deletes old records): [C]\n");
        printf("Exit: [Q]\n");
        printf(" >  ");
        scanf(" %c", &user_input);
        user_input = tolower(user_input);
        switch (user_input) {
            case 'r':
                register_user();
                break;
            case 's':
                login(login_name);
                break;
            case 'l':
                list();
                break;
            case 'c':
                create_db();
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
