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


void create_message_history( sqlite3 *db);
void create_active_connections( sqlite3 *db);

void insert_message_history( sqlite3 *db, char *port, char *name, char *message);
void insert_active_connections(sqlite3 *db, char *id, char *name);

void operation(sqlite3 *db, char *id, char *name, char *message);

void delete_Message_history(sqlite3 *db, char *id);
void delete_active_connections(sqlite3 *db, char *id);

void cleanup_message_history(sqlite3 *db);
void cleanup_active_messages(sqlite3 *db);


void create_message_history( sqlite3 *db) {

    char *zErrMsg = 0;
    int rc;
    char *sql = "CREATE TABLE IF NOT EXISTS MESSAGE_HISTORY("  \
      "ID TEXT  NOT NULL," \
      "NAME           TEXT    NOT NULL," \
      "MESSAGE        CHAR(50));" ;

   /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    } else {
      fprintf(stdout, "Message-History Table created successfully\n");
    }
}

void create_active_connections( sqlite3 *db) {

    char *zErrMsg = 0;
    int rc;
    char *sql = "CREATE TABLE IF NOT EXISTS ACTIVE_CONNECTIONS("  \
      "ID TEXT  NOT NULL," \
      "NAME           TEXT    NOT NULL);" ;

   /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    } else {
      fprintf(stdout, "Active-connections Table created successfully\n");
    }
}

void insert_message_history(sqlite3 *db, char *id, char *name, char *message) {

    char *zErrMsg = 0;
    char mystr[10];
    int rc;
    char * finalquery;
    char *values;

    char sqlQuery[101];

    sprintf(sqlQuery, "INSERT INTO MESSAGE_HISTORY VALUES( '%s' , '%s' , '%s');", id, name, message);
    rc = sqlite3_exec(db, sqlQuery, callback, 0, &zErrMsg);
    
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL Entry error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Records created successfully\n");
    }
}

void insert_active_connections(sqlite3 *db, char *id, char *name) {

    char *zErrMsg = 0;
    char mystr[10];
    int rc;
    char * finalquery;
    char *values;

    char sqlQuery[101];

    sprintf(sqlQuery, "INSERT INTO ACTIVE_CONNECTIONS VALUES( '%s' , '%s');", id, name);
    rc = sqlite3_exec(db, sqlQuery, callback, 0, &zErrMsg);
    
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL Entry error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Records created successfully\n");
    }
}

void delete_active_connections(sqlite3 *db, char *id) {

    char *zErrMsg = 0;
    int exit;
    char sqlQuery[200];
    sprintf(sqlQuery, "DELETE FROM ACTIVE_CONNECTIONS WHERE ID = '%s';", id);
    printf("%s\n", sqlQuery);
    exit = sqlite3_exec(db, sqlQuery, NULL, 0, &zErrMsg);
    if (exit != SQLITE_OK) {
        fprintf(stderr, "SQL delete error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
        fprintf(stdout, "Records deleted successfully\n");
}

void delete_message_history(sqlite3 *db, char *id) {

    char *zErrMsg = 0;
    int exit;
    char sqlQuery[200];
    sprintf(sqlQuery, "DELETE FROM MESSAGE_HISTORY WHERE ID = '%s';", id);
    exit = sqlite3_exec(db, sqlQuery, NULL, 0, &zErrMsg);
    if (exit != SQLITE_OK) {
        fprintf(stderr, "SQL delete error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
        fprintf(stdout, "Records deleted successfully\n");
}

void operation(sqlite3 *db, char *tableName, char *name, char *message) {

    char *zErrMsg = 0;
    const char* data = "Callback function called";
    int rc;

    char *sql = "SELECT * from CHATBOTNEW_TEST2;";  
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);    
    if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
    else {
      fprintf(stdout, "Operation done successfully\n");
    }
}

void cleanup_message_history(sqlite3 *db) { 

    char *zErrMsg = 0;
    int exit;
    int exit2;
    char sqlQuery1[200];
    char sqlQuery2[200];
    sprintf(sqlQuery1, "DELETE FROM MESSAGE_HISTORY;");
    exit = sqlite3_exec(db, sqlQuery1, NULL, 0, &zErrMsg);
    if (exit != SQLITE_OK) {
        fprintf(stderr, "SQL delete error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
        fprintf(stdout, "Records deleted successfully\n");

    sprintf(sqlQuery2, "VACUUM;");
    exit2 = sqlite3_exec(db, sqlQuery2, NULL, 0, &zErrMsg);
    if (exit2 != SQLITE_OK) {
        fprintf(stderr, "SQL delete error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
        fprintf(stdout, "Message-history cleaned successfully\n");
    
}

void cleanup_active_connections(sqlite3 *db) { 

    char *zErrMsg = 0;
    int exit;
    int exit2;
    char sqlQuery1[200];
    char sqlQuery2[200];
    sprintf(sqlQuery1, "DELETE FROM ACTIVE_CONNECTIONS;");
    exit = sqlite3_exec(db, sqlQuery1, NULL, 0, &zErrMsg);
    if (exit != SQLITE_OK) {
        fprintf(stderr, "SQL delete error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
        fprintf(stdout, "Records deleted successfully\n");

    sprintf(sqlQuery2, "VACUUM;");
    exit2 = sqlite3_exec(db, sqlQuery2, NULL, 0, &zErrMsg);
    if (exit2 != SQLITE_OK) {
        fprintf(stderr, "SQL delete error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
        fprintf(stdout, "vacuumed successfully\n");
    
}