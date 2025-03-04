﻿cmake_minimum_required(VERSION 3.14)

project(ContactPage VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
  
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED Core Widgets WebSockets)


file(GLOB CONTACTPAEGE_SOURCES_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB CONTACTPAGE_SOURCES_QRC ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc)
file(GLOB ADDFRIENDWIDGET_CPP ${CMAKE_CURRENT_SOURCE_DIR}/../AddFriendWidget/*.cpp)


list(APPEND CONTACTPAEGE_FILE ${CONTACTPAEGE_SOURCES_CPP} ${CONTACTPAGE_SOURCES_QRC})

add_library(${PROJECT_NAME} STATIC ${CONTACTPAEGE_FILE})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../AddFriendWidget)


target_link_libraries(${PROJECT_NAME} PRIVATE  Qt6::Core Qt6::Widgets Qt6::WebSockets Util)

