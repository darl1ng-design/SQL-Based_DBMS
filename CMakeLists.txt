cmake_minimum_required(VERSION 3.28)
project(ProjektSemestralny)

set(CMAKE_CXX_STANDARD 20)

add_executable(ProjektSemestralny main.cpp
        Column.cpp
        Column.h
        Table.cpp
        Table.h
        Database.cpp
        Database.h
        QueryExecutor.cpp
        QueryExecutor.h
        QueryExecutor.h
)
