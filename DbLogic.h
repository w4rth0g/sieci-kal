#ifndef DBLOGIC_H
#define DBLOGIC_H

#include <pqxx/pqxx>
#include <memory>
#include <string>

class DbLogic {
public:
    DbLogic(const std::string& dbname, const std::string& user, const std::string& password);

    bool addEvent(const std::string& user_id, const std::string& title, const std::string& description, const std::string& start_time, const std::string& end_time);

    std::string getEvents();

    std::string getLoggedInUser(std::string username, std::string password);

    bool deleteEvent(const std::string& event_id);
    
private:
    std::unique_ptr<pqxx::connection> conn;
};

#endif // DBLOGIC_H
