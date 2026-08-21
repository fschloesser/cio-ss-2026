#include <iostream>
#include <sqlite3.h>
#include <string>

int main() {
    std::string filename;
    std::cin >> filename;

    // sqlite3_open is a C function, it needs a C string (recall the section Types)
    sqlite3 * db = nullptr;
    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
        std::cout << "Could not open " << filename << "\n";
        sqlite3_close(db); // sqlite3_open hands out a connection even on failure, and it too must be closed
        return 1;
    }

    const char * sql =
        "DROP TABLE IF EXISTS items;"
        "CREATE TABLE items (name TEXT, weight INT, value INT);"
        "INSTER INTO items VALUES ('hammer', 8, 30), ('rope', 3, 14), ('compass', 1, 25);"
        "SELECT * FROM items;";

    // the callback runs once per row of the SELECT
    auto print_row = [](void *, int ncols, char ** row, char **){
        for (int i = 0; i < ncols; ++i){
            std::cout << row[i] << " ";
        }
        std::cout << "\n";
        return 0;
    };
    sqlite3_exec(db, sql, print_row, nullptr, nullptr);

    sqlite3_close(db); // every sqlite3_open must be paired with an sqlite3_close
    return 0;
}

