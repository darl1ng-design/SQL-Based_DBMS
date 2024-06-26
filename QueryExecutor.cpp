#include "QueryExecutor.h"
#include <ranges>
#include <sstream>
#include <bits/stl_algo.h>
QueryExecutor::QueryExecutor(Database *db) : db(db){}

std::vector<std::string> QueryExecutor::QueryToArray(const std::string &input, char SeperateChar) {
    std::vector<std::string> query_element_vector;
    std::string element;
    std::stringstream elementStream(input);
    while(std::getline(elementStream, element, SeperateChar) ) {
        query_element_vector.push_back(element);
    }
    return query_element_vector;
}


std::map<std::string, std::string> QueryExecutor::parseColumnNameAndType( std::string &name,  std::string& type) {
    std::map<std::string, std::string> parsedColumnMap;
    parsedColumnMap.insert(std::make_pair(name, type));
    return parsedColumnMap;
}

std::string QueryExecutor::dataTypeChecker(const std::string &passedType) {
    if(passedType == "std::string") return "std::string";
    if(passedType == "int") return "int";
    if(passedType == "double") return "double";
    return "Make sure your data type is valid! \n";
}

void QueryExecutor::executeQuery(const std::string &query) {
    auto splitQuery = QueryToArray(query, ' ');

    if(splitQuery.empty() ) {
        std::cout << "Query cannot remain empty! \n";
    }

    //INIT TABLE tablename { {client_id,int}, {client_name, std::string}.... }
    if(splitQuery[0] == "INIT" && splitQuery[1] == "TABLE") {
        std::string tableName = splitQuery[2];
        std::vector<std::pair<std::string, std::string>> pairedData;
        for(int i = 3; i< splitQuery.size(); i+=2) {
            auto splitColumn = parseColumnNameAndType(splitQuery[i], splitQuery[i+1]);
            for(const auto& [name, type]: splitColumn) {
                pairedData.emplace_back(name, dataTypeChecker(type));
            }
        }
        std::vector<Column> TableCols;
        for(const auto& [x, type] : pairedData) {
            TableCols.emplace_back(x, type);
        }
        db->addTable(tableName, TableCols);
    }

    //DELETE TABLE tablename
    else if(splitQuery[0] == "DELETE" && splitQuery[1] == "TABLE") {
        std::string tablename = splitQuery[2];
        db->dropTable(tablename);
    }

    //RENAME TABLE table oldname newname
    else if(splitQuery[0] == "RENAME" && splitQuery[1] == "TABLE") {
        std::string oldname = splitQuery[2];
        std::string newName = splitQuery[3];
        db->renameTable(oldname, newName);
    }

    //SELECT column1, column2 from tablename where column1 ....
    else if(splitQuery[0] == "CHOOSE") {
        auto fromKeyword = std::ranges::find(splitQuery.begin(), splitQuery.end(), "FROM");
        if(fromKeyword == splitQuery.end()){
            std::cout << "Invalid CHOOSE query: Missing FROM clause \n";
        }else if((fromKeyword+1) == splitQuery.end()) {
            std::cout << "INVALID CHOOSE query: Missing table name \n";
        }
        else if(splitQuery.begin()+1 == fromKeyword) {
            std::cout << "Invalid CHOOSE query: No columns specified \n";
        }
        std::vector<std::string> columnNames(splitQuery.begin() + 1, fromKeyword);
        std::string tableName = *(fromKeyword + 1);

        std::map<std::string, std::variant<int, double, std::string>> whereStatement;
        auto whereKeyword = std::ranges::find(splitQuery.begin(), splitQuery.end(), "WHERE");

        if(whereKeyword != splitQuery.end()) {
            for(auto it = whereKeyword + 1 ; it != splitQuery.end(); it+=3) {
                if ((it + 2) <= splitQuery.end()) {
                    whereStatement[*it] = *(it+2);
                }else {
                    std::cout << "Invalid WHERE clause format \n";
                }

            }
        }
        auto tableIt = db->getTables()->find(tableName);
        if(tableIt == db->getTables()->end()) {
            std::cout << "Table not found " << tableName << "\n";
        }
        tableIt->second.selectWhere(tableName, columnNames, whereStatement);
    }


    if (splitQuery[0] == "UPDATE") {
        if (splitQuery.size() < 9) {
            std::cout << "Invalid UPDATE query format.\n";
            return;
        }

        std::string tableName = splitQuery[1];
        std::string setKeyword = splitQuery[2];

        if (setKeyword != "SET") {
            std::cout << "Invalid UPDATE query format.\n";
            return;
        }

        std::string columnName = splitQuery[3];
        std::string updatedValueStr = splitQuery[5];

        if (splitQuery[6] != "WHERE") {
            std::cout << "Invalid UPDATE query format.\n";
            return;
        }

        std::string whereColumnName = splitQuery[7];
        std::string whereValueStr = splitQuery[9];

        auto tableIt = db->getTables()->find(tableName);
        if (tableIt == db->getTables()->end()) {
            std::cout << "Table not found: " << tableName << "\n";
            return;
        }

        std::variant<int, double, std::string> updatedValue = updatedValueStr;
        std::variant<int, double, std::string> whereValue = whereValueStr;

        Table* table = &tableIt->second;
        table->updateCol(tableName, columnName, updatedValue, whereColumnName, whereValue);
    }

    //INSERT RECORDS tablename client_id 1 client_name "john"
    else if((splitQuery[0] == "INSERT") && splitQuery[1] == "RECORDS") {
        std::string tableName = splitQuery[2];
        std::map<std::string, std::variant<int, double, std::string>> records;
        Table *tableObject = &db->getTables()->at(tableName);
        for(auto i = 3; i<splitQuery.size(); ++i) {
            records[splitQuery[i]] = splitQuery[i+1];
            break;
        }
        for(const auto& [ColNameToInsert, ColValToInsert] : records) {
            records[ColNameToInsert] = ColValToInsert;
            break;
        }
        tableObject->insertRows(tableName, records);
    }

    //MODIFY TABLE tablename ADD columnname columnType
    else if(splitQuery[0] == "MODIFY" && splitQuery[1] == "TABLE" && splitQuery[3] == "ADD") {
        std::string tablename = splitQuery[2];
        Table *tableObject = &db->getTables()->at(tablename);
        for(auto i = 4; i < splitQuery.size(); ++i) {
            Column col(splitQuery[i], splitQuery[i+1]);
            tableObject->addColumn(col);
            break;
        }
    }
    //MODIFY TABLE tablename REMOVE columnname
    else if(splitQuery[0] == "MODIFY" && splitQuery[1] == "TABLE" && splitQuery[3] == "REMOVE") {
        std::string tablename = splitQuery[2];
        Table *tableObject = &db->getTables()->at(tablename);
        for(auto i = 4; i < splitQuery.size(); ++i) {
            tableObject->dropColumn(splitQuery[i]);
            break;
        }
    }

    //MODIFY TABLE tablename RENAME oldname newname
    else if(splitQuery[0] == "MODIFY" && splitQuery[1] == "TABLE" && splitQuery[3] == "RENAME") {
        std::string tablename = splitQuery[2];
        Table *tableObject = &db->getTables()->at(tablename);
        for(auto i = 4; i < splitQuery.size(); ++i) {
            tableObject->renameColumn(splitQuery[i], splitQuery[i+1]);
            break;
        }
    }

    //MODIFY TABLE tablename MODIFY column_name, datatype
    else if(splitQuery[0] == "MODIFY" && splitQuery[1] == "TABLE" && splitQuery[3] == "MODIFY") {
        std::string tablename = splitQuery[2];
        Table *tableObject = &db->getTables()->at(tablename);
        for(auto i = 4; i < splitQuery.size(); ++i) {
            tableObject->changeDataType(splitQuery[i], splitQuery[i+1]);
            break;
        }
    }

    //REMOVE FROM tablename
    else if(splitQuery[0] == "REMOVE" && splitQuery[1] == "FROM") {
        std::string tablename = splitQuery[2];
        Table *tableObject = &db->getTables()->at(tablename);
        std::map<std::string, std::variant<int, double, std::string>> whereStatement;
        auto whereKeyword = std::ranges::find(splitQuery.begin(), splitQuery.end(), "WHERE");
        if(whereKeyword != splitQuery.end()) {
            for(auto it = whereKeyword+1; it != splitQuery.end(); it +=2) {
                whereStatement[*it] = *(it+1);
            }
        }
    tableObject->deleteCol(tablename, whereStatement);
    }

    else if(splitQuery[0] == "ECHO" && splitQuery[1] == "TABLES") {
        db->printTables();
    }
    else if(splitQuery[0] == "ECHO" && splitQuery[1] == "COLUMNS" && splitQuery[3] == "FROM") {
        std::string tableName = splitQuery[4];
        db->getTables()->at(tableName).displayColumns(splitQuery[2]);
    }

}





