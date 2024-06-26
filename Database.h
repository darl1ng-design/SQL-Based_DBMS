#ifndef DATABASE_H
#define DATABASE_H
#include <map>
#include <string>
#include <vector>
#include "Table.h"
class Database{
    std::map<std::string, Table> tables;
public:
    explicit Database(std::map<std::string, Table>& tables);
    Database();
    void renameTable(const std::string &oldName, const std::string &newName);
    std::map<std::string, Table>* getTables();
    void printTables() const;
    void addTable(const std::string& name, const std::vector<Column>& columns);
    void dropTable(const std::string& name);

};
#endif //DATABASE_H
