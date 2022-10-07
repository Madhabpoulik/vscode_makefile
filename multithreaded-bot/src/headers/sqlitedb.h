// #include "../sqlite/sqlite3.h"

void create_message_history( sqlite3 *db);
void create_active_connections( sqlite3 *db);

void insert_message_history( sqlite3 *db, char *port, char *name, char *message);
void insert_active_connections(sqlite3 *db, char *id, char *name);

void operation(sqlite3 *db, char *id, char *name, char *message);

void delete_Message_history(sqlite3 *db, char *id);
void delete_active_connections(sqlite3 *db, char *id);

void cleanup_active_connections(sqlite3 *db);
void cleanup_message_history(sqlite3 *db);
