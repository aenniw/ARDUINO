cmake_minimum_required(VERSION 3.2)
project(arduino_projects_root)

execute_process(COMMAND ${CMAKE_HOME_DIRECTORY}/configure ${CMAKE_HOME_DIRECTORY})
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(skarsta)
add_subdirectory(yarn-winder)
