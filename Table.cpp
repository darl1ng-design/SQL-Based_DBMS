#include <iostream>
#include <algorithm>
#include <utility>
#include <map>
#include "Table.h"
Table::Table(std::string tableName,
             const std::vector<Column>& columns): tableName(std::move(tableName)), columns(columns){}
Table::Table(){}

std::ostream& operator<<(std::ostream& os, const Table& table){
    for(auto const& column : table.columns){
        os << column;
    }
    return os;
}

void Table::displayColumns(const std::string& col){
    auto columnsIt = std::ranges::find_if(columns.begin(), columns.end(), [&col](Column& column) {
        return column.getcolName() == col;;
    });
    if(columnsIt != columns.end()) {
        std::cout <<  "Name :" << columnsIt->getcolName() << " type: " << columnsIt->getType() << "\n";
    }
    else {
        std::cout << "Columns not found \n";
    }
}


void Table::addColumn(const Column& column){
    columns.emplace_back(column);
    std::cout << "Successfully added column \n";
}


void Table::dropColumn(const std::string &colName) {
    auto columnsIt = std::ranges::find_if(columns.begin(), columns.end(), [&colName](Column& column) {
        return column.getcolName() == colName;
    });
    if(columnsIt != columns.end()) {
        columns.erase(columnsIt);
        std::cout << "Successfully dropped column \n";
    }
    else {
        std::cout << "Please make sure that the requested table or column exist. \n";
    }
}

void Table::renameColumn(const std::string &oldName, const std::string &newName) {
    auto columnsIt = std::ranges::find_if(columns.begin(), columns.end(), [&oldName](Column& column) {
       return column.getcolName() == oldName;
   });
    if(columnsIt != columns.end()) {
        columnsIt->getcolName() = newName;
        std::cout << "Successfully changed column name \n";
    }else {
        std::cout << "Column " << oldName << " not found \n";
    }
}


void Table::changeDataType(const std::string &columnName, const std::string &newDataType) {
    auto columnsIt = std::ranges::find_if(columns.begin(), columns.end(), [&columnName](Column& column) {
       return column.getcolName() == columnName;
   });
    if(columnsIt != columns.end()) {
        columnsIt->setType(newDataType);
        std::cout << "Successfully changed data type \n";
    }
    else {
        std::cout << "Column " << columnName << " does not exist! \n";
    }
}
void Table::insertRows(const std::string &tname, const std::map<std::string, std::variant<int, double, std::string>> &records) {
    if(tname == tableName){
        for(const auto& [colName, record] : records) {
            auto columnsIt = std::ranges::find_if(columns.begin(), columns.end(), [&colName](Column& column) {
             return column.getcolName() == colName;
         });
            if(columnsIt != columns.end()) {
                columnsIt->getData().emplace_back(record);
                std::cout << "Sucessfully inserted data \n";
            }else {
                std::cout << "Column " << colName << " does not exist! \n";
            }
        }
    }
    else {
        std::cout << "Table " << tname << " not found in database! \n";
    }
}
void Table::updateCol(const std::string &tname,
                      const std::string &columnName,
                      const std::variant<int, double, std::string> &updatedValue,
                      const std::string &whereColumn,
                      const std::variant<int, double, std::string> &whereValue) {
    if (tname == tableName) {
        auto colIt = std::find_if(columns.begin(), columns.end(), [&](Column& col) {
            return col.getcolName() == columnName;
        });

        if (colIt != columns.end()) {
            auto& colData = colIt->getData();
            auto whereIt = std::find_if(colData.begin(), colData.end(), [&](auto& record) {
                return record == whereValue;
            });
            if (whereIt != colData.end()) {
                *whereIt = updatedValue;
                std::cout << "Successfully updated record.\n";
            } else {
                std::cout << "Record not found for given WHERE condition.\n";
            }
        } else {
            std::cout << "Column not found.\n";
        }
    } else {
        std::cout << "Table not found.\n";
    }
}




void Table::selectWhere(const std::string &tablename, const std::vector<std::string> &columnNames, const std::map<std::string, std::variant<int, double, std::string>> &where) {
    if(tablename != tableName) {
        std::cout << "Table " << tablename << "does not exist in database! \n";
        return;
    }
    for(auto i = 0; i < columns.front().getData().size(); ++i) {
        bool match = true;
        for(const auto& [colName, whereVal] : where) {
            auto colIt = std::ranges::find_if(columns.begin(), columns.end(), [&colName](Column& col) {
            return col.getcolName() == colName;
            });
            if (colIt == columns.end() || colIt->getData()[i] != whereVal) {
                match = false;
                break;
            }
        }
        if(match) {
            for(const auto& columnName : columnNames) {
                auto columnsIt = std::ranges::find_if(columns.begin(), columns.end(), [&columnName](Column& column) {
                    return column.getcolName() == columnName;
             });
                if(columnsIt != columns.end()) {
                    std::cout << columnsIt->getcolName() << ": ";
                    std::visit([](const auto& val){ std::cout << val << " \n"; }, columnsIt->getData()[i]);
                }
            }
        }
    }
}
void Table::deleteCol(const std::string &tname, const std::map<std::string, std::variant<int, double, std::string>> &where) {
    if(tname == tableName) {
        for(auto& col : columns) {
            auto whereStatementMatch = [&](const auto& val) { // lambda method which return true if passed record meets all the conditions, otherwise it returns false
                for(const auto& [colname, record] : where) {
                    if((colname == col.getcolName() && val != record) || (colname != col.getcolName() && val == record))  return false;
                }
                return true;
            };
        col.getData().erase(
            std::remove_if(col.getData().begin(), col.getData().end(), whereStatementMatch),
            col.getData().end());
        }
        std::cout << "Successfully deleted records";
    }else{
        std::cout << "Table " << tname << "does not exist in database! \n";
    }
}

