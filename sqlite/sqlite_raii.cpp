#include <iostream>
#include <memory>
#include <sqlite3.h>
#include <string>

// RAII stands for Resource Acquisition Is Initialization

struct SqliteCloser{
    // 1. We create a `promise` to close the connection once the owner goes out of scope
    void operator()(sqlite3 * db) const noexcept { sqlite3_close(db); }
};

int main(){
    std::string filename;
    std::cin >> filename;

    // 2. Open into a raw pointer, then immediately hand ownership over, with the promise attached
    sqlite3 * raw = nullptr;
    int rc = sqlite3_open(filename.c_str(), &raw);
    std::unique_ptr<sqlite3, SqliteCloser> db(raw);
    if (rc != SQLITE_OK){
        std::cout << "Could not open " << filename << "\n";
        return 1; // the promise still closes the handle that the failed open handed out
    }

    const char * sql =
        "DROP TABLE IF EXISTS items;"
        "CREATE TABLE items (name TEXT, weight INT, value INT);"
        "INSERT INTO items VALUES ('hammer', 8, 30), ('rope', 3, 14), ('compass', 1, 25);"
        "SELECT * FROM items;";

    auto print_row = [](void *, int ncols, char ** row, char **){
        for (int i = 0; i < ncols; ++i){
            std::cout << row[i] << " ";
        }
        std::cout << "\n";
        return 0;
    };
    // 3. .get() hands the raw pointer to a C function without giving up ownership.
    if (sqlite3_exec(db.get(), sql, print_row, nullptr, nullptr) != SQLITE_OK){
        std::cout << "Query failed: " << sqlite3_errmsg(db.get()) << "\n";
        return 1; // early exit needs no special case, the promise is kept
    }
    return 0; // db goes out of scope, the promise is kept, sqlite3_close runs
}gg
