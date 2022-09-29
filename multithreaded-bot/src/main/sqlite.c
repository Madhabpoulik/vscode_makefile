#include <stdio.h>
#include <stdlib.h>
//include <sqlite/sqlite3.h>
#include "../sqlite/sqlite3.h"

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   
   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
}

// char *concatenate(const char *a, const char *b, const char *c) {
//     size_t alen = strlen(a);
//     size_t blen = strlen(b);
//     size_t clen = strlen(c);
//     char *res = malloc(alen + blen + clen + 1);
//     if (res) {
//         memcpy(res, a, alen);
//         memcpy(res + alen, b, blen);
//         memcpy(res + alen + blen, c, clen + 1);
//     }
//     return res;
// }

void create( sqlite3 *db);
void insert( sqlite3 *db, char *port, char *name, char *message);
void operation(sqlite3 *db, char *id, char *name, char *message);
void delete(sqlite3 *db, char *id);

// int main(int argc, char* argv[]) {

//     //create(db);
//     //for(i = 0; i < 10; i++)    operation(db, 7080, "madhab", "Hello");

//    //sqlite3_close(db);
// }

void create( sqlite3 *db) {

    char *zErrMsg = 0;
    int rc;
    char *sql = "CREATE TABLE IF NOT EXISTS CHATBOTNEW_TEST2("  \
      "ID TEXT  NOT NULL," \
      "NAME           TEXT    NOT NULL," \
      "MESSAGE        CHAR(50));" ;

   /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    } else {
      fprintf(stdout, "Table created successfully\n");
    }
}

void insert(sqlite3 *db, char *id, char *name, char *message) {

    char *zErrMsg = 0;
    char mystr[10];
    int rc;
    char * finalquery;
    char *values;

    char sqlQuery[101];

    sprintf(sqlQuery, "INSERT INTO CHATBOTNEW_TEST2 VALUES( '%s' , '%s' , '%s');", id, name, message);
    //char *query = "INSERT INTO CHATBOT (PORT,NAME,MESSAGE) "  \
            "VALUES( '7082', 'MCP', 'HI');";
     
    // strcat(mystr, port);
  //   printf("%s , %s , %s\n", id , name, message);
    rc = sqlite3_exec(db, sqlQuery, callback, 0, &zErrMsg);
    
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL Entry error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Records created successfully\n");
    }
}

// void update(sqlite3 *db, char *id, char *name, char *message) {

//     char *zErrMsg = 0;
//     const char* data = "Callback function called";
//     int rc;
//     char sqlQuery[101];

//     sprintf(sqlQuery, "UPDATE CHATBOT_TEST VALUES( '%s' , '%s' , '%s');", id, name, message);

//     sql = "UPDATE COMPANY set SALARY = 25000.00 where ID=1; " \
//          "SELECT * from COMPANY";

//     rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   
//    if( rc != SQLITE_OK ) {
//       fprintf(stderr, "SQL update error: %s\n", zErrMsg);
//       sqlite3_free(zErrMsg);
//     } else {
//         fprintf(stdout, "Update done successfully\n");
//     }
// }

void delete(sqlite3 *db, char *id) {

    char *zErrMsg = 0;
    int exit;
    char sqlQuery[200];
    sprintf(sqlQuery, "DELETE FROM CHATBOTNEW_TEST2 WHERE ID = '%s';", id);
    printf("%s\n", sqlQuery);
    exit = sqlite3_exec(db, sqlQuery, NULL, 0, &zErrMsg);
    if (exit != SQLITE_OK) {
        fprintf(stderr, "SQL delete error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
        fprintf(stdout, "Records deleted successfully\n");
}

void operation(sqlite3 *db, char *port, char *name, char *message) {

    char *zErrMsg = 0;
    const char* data = "Callback function called";
    int rc;

    char *sql = "SELECT * from CHATBOTNEW_TEST2;";  
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);    
    if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Operation done successfully\n");
    }
}