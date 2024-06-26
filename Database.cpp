#include "Database.h"
Database::Database(std::map<std::string, Table>& tables) : tables(tables){}
Database::Database() = default;
std::map<std::string, Table>* Database::getTables() {
    return &tables;
}
void Database::addTable(const std::string &name, const std::vector<Column> &columns) {
    if(tables.contains(name)){
        std::cout << "Cannot add table " << name << ", it already exists! \n";
    }
    tables.insert({name, Table(name, columns)});
}

void Database::dropTable(const std::string &name) {
    for(const auto& pair : tables) {
            if(!pair.first.empty() && name == pair.first) {
                tables.erase(name);
                return;
            }
        }
    std::cout << "Cannot drop table that does not exist! \n";
}

void Database::renameTable(const std::string &oldName, const std::string &newName) {
    if(tables.contains(newName) || oldName == newName){
        std::cout << "Cannot rename to " << newName << ", it already exists! \n";
    }
    auto tableIterator = tables.find(oldName);
    if(tableIterator != tables.end()) {
        tables[newName] = tableIterator->second; //wywowalnie pustego konstruktora table
        tables.erase(oldName);
    }
    else {
        std::cout << "Table " << oldName << " does not exist! \n";
    }
}

void Database::printTables() const{
    for(const auto& pair : tables) {
            if(pair.first.empty()) {
                std::cout << "No tables were created! \n";
            }
        std::cout << "Table name: " << pair.first << " columns: " << pair.second << "\n";
    }
}



