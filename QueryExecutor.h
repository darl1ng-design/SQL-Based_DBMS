#ifndef QUERYEXECUTOR_H
#define QUERYEXECUTOR_H
#include "Database.h"
class QueryExecutor {
    Database *db;
public:
    QueryExecutor(Database *db);
    std::vector<std::string> QueryToArray(const std::string& input, char SeperateChar);
    std::map<std::string, std::string> parseColumnNameAndType( std::string &name,  std::string& type);
    std::string dataTypeChecker(const std::string& passedType);
    std::variant<int, double, std::string> convertToVariantObj(const std::string& dataType);
    void executeQuery(const std::string& query);
};



#endif //QUERYEXECUTOR_H
