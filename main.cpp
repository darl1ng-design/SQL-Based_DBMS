#include <iostream>
#include "Database.h"
#include "QueryExecutor.h"
//BIBLIOGRAFIA
// https://www.geeksforgeeks.org/std-variant-in-cpp-17/
// https://cplusplus.com/reference/sstream/stringstream/stringstream/
// https://www.geeksforgeeks.org/how-to-split-a-string-in-cc-python-and-java/
// https://www.geeksforgeeks.org/how-to-split-string-by-delimiter-in-cpp/?ref=ml_lbp
// https://www.programiz.com/cpp-programming/string-int-conversion
// https://www.ispectra.co/blog/unraveling-magic-behind-string-double-conversion
// https://en.cppreference.com/w/cpp/algorithm/ranges/find
// https://www.geeksforgeeks.org/readwrite-class-objects-fromto-file-c/



int main() {
    Database db;
    QueryExecutor query_executor(&db);
    std::string dbQuery;
    std::cout << "PJC DBMS project\n";
    while(true) {
        std::getline(std::cin, dbQuery);
        if(dbQuery == "SHUTDOWN") {
            break;
        }
        try {
            query_executor.executeQuery(dbQuery);
        }catch(const std::exception& e) {
            std::cout << e.what() << "\n";
        }
    }

}
