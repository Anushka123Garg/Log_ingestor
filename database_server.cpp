#include <iostream>
#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include <cstdlib>
#include "cpp-httplib\httplib.h"
#include "json\single_include\nlohmann\json.hpp"
#include "include\sqlite3.h"

#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>

using json = nlohmann::json;
using namespace std;

// SQLite database connection
sqlite3* db;

void initDatabase() {
    int rc = sqlite3_open("logs.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        exit(1);
    }

    // Create a table for logs if it doesn't exist
    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS logs ("
                                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                "level TEXT,"
                                "message TEXT,"
                                "resourceId TEXT,"
                                "timestamp TEXT,"
                                "traceId TEXT,"
                                "spanId TEXT,"
                                "commit TEXT,"
                                "parentResourceId TEXT"
                                ");";

    rc = sqlite3_exec(db, createTableSQL, 0, 0, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't create table: " << sqlite3_errmsg(db) << std::endl;
        exit(1);
    }
}

void insertLog(const json& log) {
    std::string insertSQL = "INSERT INTO logs (level, message, resourceId, timestamp, traceId, spanId, commit, parentResourceId) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Prepare statement failed: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, log["level"].get<std::string>().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, log["message"].get<std::string>().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, log["resourceId"].get<std::string>().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, log["timestamp"].get<std::string>().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, log["traceId"].get<std::string>().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, log["spanId"].get<std::string>().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, log["commit"].get<std::string>().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, log["metadata"]["parentResourceId"].get<std::string>().c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void closeDatabase() {
    sqlite3_close(db);
}

int main() {
    initDatabase();

    httplib::Server svr;

    svr.Post("/ingest", [](const httplib::Request& req, httplib::Response& res) {
        json logData = json::parse(req.body);
        insertLog(logData);

        // Respond with a success message
        res.set_content("Log received and stored", "text/plain");
    });

    // Start the HTTP server on port 3000
    svr.listen("localhost", 3000);

    closeDatabase();

    return 0;
}