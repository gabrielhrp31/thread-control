cmake_minimum_required(VERSION 3.14)
project(trabalho_thread C)

set(CMAKE_C_STANDARD 99 )
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(trabalho_thread main.c util.h util.c)