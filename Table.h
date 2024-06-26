#ifndef TABLE_H
#define TABLE_H
#include <vector>
#include <string>
#include <iostream>
#include "Column.h"
class Table{
    std::string tableName;
    std::vector<Column> columns;
public:
    friend std::ostream& operator<<(std::ostream& os, const Table& table);
    Table(); // domyslny konstruktor na rzecz funkcji renameTable, ktora wywoluje pusty konstruktor table
    Table(
            std::string tableName,
            const std::vector<Column>& columns
    );
    void addColumn(const Column& column);
    void dropColumn(const std::string& colName);
    void renameColumn(const std::string &oldName,const std::string &newName);
    void changeDataType(const std::string &columnName, const std::string &newDataType);
    void insertRows(const std::string &tName, const std::map<std::string, std::variant<int, double, std::string>> &records);
    /*void updateCol(const std::string &tName,
    const std::map<std::string, std::variant<int, double, std::string>> &records,
    const std::map<std::string, std::variant<int, double, std::string>> &where)*/;
    void updateCol(const std::string &tname,
                      const std::string &columnName,
                      const std::variant<int, double, std::string> &updatedValue,
                      const std::string &whereColumn,
                      const std::variant<int, double, std::string> &whereValue);
    void selectWhere(const std::string& tname,
    const std::vector<std::string> &columnNames, const std::map<std::string,
    std::variant<int, double, std::string>> &where);
    /*void selectWhere(const std::vector<std::string>& columnNames, const std::map<std::string, std::variant<int, double, std::string>>& whereClause);*/
    void deleteCol(const std::string &tname, const std::map<std::string,  std::variant<int, double, std::string>> &where);
    void displayColumns(const std::string& col);
};
#endif //TABLE_H
