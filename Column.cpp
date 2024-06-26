#include <iostream>
#include <utility>
#include <variant>
#include "Column.h"
Column::Column(const std::string& columnName, const std::string& type,
    const std::vector<std::variant<int, double, std::string>>& data) :
    columnName(columnName), type(type),
    data(data) {}

Column::Column(const std::string& columnName, const std::string& type) :
    columnName(columnName), type(type), data() {}

std::string &Column::getType() {
    return type;
}
std::string& Column::getcolName(){
    return columnName;
}
void Column::setColName(const std::string& newName) {
    columnName = newName;
}
void Column::setType(const std::string &dataType) {
    type = dataType;
}
std::vector<std::variant<int, double, std::string> > &Column::getData() {
    return data;
}
std::ostream& operator<<(std::ostream& os, const Column& column){
    os << column.columnName << " ";
    /*os << column.type << " ";*/
    for(const auto& x : column.data) {
            std::visit([&os](const auto& val) {
                os << val << "\n";
            }, x);
    }
    return os;
}
