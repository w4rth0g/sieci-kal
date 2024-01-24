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

std::string DbLogic::getEvents() {
    std::stringstream ss;
    try {
        pqxx::work txn(*conn);
        pqxx::result r = txn.exec("SELECT event_id, user_id, title, description, start_time, end_time FROM events");

        for (auto row : r) {
            ss << row["event_id"].as<std::string>() << " | "
               << row["user_id"].as<std::string>() << " | "
               << row["title"].as<std::string>() << " | "
               << row["description"].as<std::string>() << " | "
               << row["start_time"].as<std::string>() << " | "
               << row["end_time"].as<std::string>() << "\n";
        }
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return ss.str();
}

std::string DbLogic::getLoggedInUser(std::string username, std::string password) {
    std::string userId;
    try {
        pqxx::work txn(*conn);
        pqxx::result r = txn.exec("SELECT user_id FROM users WHERE username = "
                + txn.quote(username) + " AND password = "
                + txn.quote(password));

        if (r.size() == 1) {
            userId = r[0]["user_id"].as<std::string>();
        } else {
            userId = "";
        }
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return userId;
}

std::string DbLogic::getUserById(std::string userId) {
    std::string username;
    try {
        pqxx::work txn(*conn);
        pqxx::result r = txn.exec("SELECT username FROM users WHERE user_id = "
                + txn.quote(userId));

        if (r.size() == 1) {
            username = r[0]["username"].as<std::string>();
        } else {
            username = "";
        }
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return username;
}

bool DbLogic::deleteEvent(const std::string& event_id) {
    try {
        pqxx::work txn(*conn);
        std::string query = "DELETE FROM events WHERE event_id = " + txn.quote(event_id) + ";";
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

