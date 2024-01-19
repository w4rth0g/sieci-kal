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

