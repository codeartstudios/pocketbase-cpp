cmake_minimum_required(VERSION 3.14)

project(pocketbase-cpp LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(pocketbase-cpp
  main.cpp
  sdk/qpocketbase.h sdk/qpocketbase.cpp
  sdk/authstore.h sdk/authstore.cpp
  sdk/adminservice.h sdk/adminservice.cpp
  sdk/collectionservice.h sdk/collectionservice.cpp
  sdk/fileservice.h sdk/fileservice.cpp
  sdk/realtimeservice.h sdk/realtimeservice.cpp
  sdk/settingsservice.h sdk/settingsservice.cpp
  sdk/logservice.h sdk/logservice.cpp
  sdk/healthservice.h sdk/healthservice.cpp
  sdk/backupservice.h sdk/backupservice.cpp
  sdk/recordservice.h sdk/recordservice.cpp
)
target_link_libraries(pocketbase-cpp Qt${QT_VERSION_MAJOR}::Core)

include(GNUInstallDirs)
install(TARGETS pocketbase-cpp
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
