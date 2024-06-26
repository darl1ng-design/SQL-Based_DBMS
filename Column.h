#ifndef COLUMN_H
#define COLUMN_H
#include <string>
#include <variant>
#include <vector>

class Column{
    std::string columnName;
    std::string type;
    std::vector<std::variant<int, double, std::string>> data;
public:
    std::string& getcolName();
    std::string& getType();
    std::vector<std::variant<int, double, std::string>>& getData();
    void setColName(const std::string &newName);
    void setType(const std::string &dataType);
    friend std::ostream& operator<<(std::ostream& os, const Column& column);
    Column(const std::string &columnName, const std::string &type, const std::vector<std::variant<int, double, std::string>>& data);
    Column(const std::string &columnName, const std::string &type);
    Column() = default;
};

#endif //COLUMN_H
