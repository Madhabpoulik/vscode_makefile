// #include "../sqlite/sqlite3.h"

void create( sqlite3 *db);
void insert(sqlite3 *db, char *id, char *name, char *message);
void operation(sqlite3 *db, char *port, char *name, char *message);
void delete(sqlite3 *db, char *id);
