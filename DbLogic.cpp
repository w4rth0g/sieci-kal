#include "DbLogic.h"
#include <iostream>

DbLogic::DbLogic(const std::string& dbname, const std::string& user, const std::string& password) {
    try {
        conn = std::make_unique<pqxx::connection>("dbname=" + dbname + " user=" + user + " password=" + password);
        if (conn->is_open()) {
            std::cout << "Połączono z bazą danych: " << conn->dbname() << std::endl;
        } else {
            std::cerr << "Nie można połączyć z bazą danych" << std::endl;
        }
    } catch (const pqxx::sql_error& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

bool DbLogic::addEvent(const std::string& user_id, const std::string& title, const std::string& description, const std::string& start_time, const std::string& end_time) {
    try {
        pqxx::work txn(*conn);
        std::string query = "INSERT INTO events (user_id, title, description, start_time, end_time) VALUES (" +
                            txn.quote(user_id) + ", " +
                            txn.quote(title) + ", " +
                            txn.quote(description) + ", " +
                            txn.quote(start_time) + ", " +
                            txn.quote(end_time) + ");";
        txn.exec(query);
        txn.commit();
        return true;
    } catch (const pqxx::sql_error& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return false;
    }
}
