#include <stdio.h>
#include <stdlib.h>
#include <sqlite/sqlite3.h>
//#include "sqlite/sqlite3.h"

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   
   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
}

int main(int argc, char* argv[]) {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;

   rc = sqlite3_open("chatbot.db", &db);

   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }

   create

   sqlite3_close(db);
}

void create( sqlite3 *db, ) {

    char *zErrMsg = 0;
    sql = "CREATE TABLE CHATBOT("  \
      "PORT INT PRIMARY KEY     NOT NULL," \
      "NAME           TEXT    NOT NULL," \
      "MESSAGE        CHAR(50);" ;

   /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    } else {
      fprintf(stdout, "Table created successfully\n");
    }
}

void insert(sqlite3 *db, int port, char *name, char *message) {

    char *zErrMsg = 0;
    sql = "INSERT INTO CHATBOT (PORT,NAME,MESSAGE) "  \
            "VALUES(" \
    
    sql = strcat(sql, name + ",");
    sql = strcat(sql, message + ",");


            // "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
            // "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
            // "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
            // "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
            // "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
            // "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL Entry error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Records created successfully\n");
    }
}

void update() {

    char *zErrMsg = 0;
    sql = "UPDATE COMPANY set SALARY = 25000.00 where ID=1; " \
         "SELECT * from COMPANY";

    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   
   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL update error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Operation done successfully\n");
    }
}

void delete() {

    char *zErrMsg = 0;
    sql = "DELETE FROM PERSON WHERE ID = 2;";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &zErrMsg);
    if (exit != SQLITE_OK) {
        fprintf(stderr, "SQL delete error: %s\n", zErrMsg);
        sqlite3_free(messaggeError);
    }
    else
        fprintf(stdout, "Records deleted successfully\n");
}