#ifndef DBLOGIC_H
#define DBLOGIC_H

#include <pqxx/pqxx>
#include <memory>
#include <string>

class DbLogic {
public:
    DbLogic(const std::string& dbname, const std::string& user, const std::string& password);
    
private:
    std::unique_ptr<pqxx::connection> conn;
};

#endif // DBLOGIC_H
