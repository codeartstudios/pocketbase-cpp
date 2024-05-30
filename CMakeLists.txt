cmake_minimum_required(VERSION 3.14)

project(pocketbase-cpp LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

add_executable(pocketbase-cpp
  main.cpp
  sdk/qpocketbase.h sdk/qpocketbase.cpp
  sdk/baseauthstore.h sdk/baseauthstore.cpp
  sdk/adminservice.h sdk/adminservice.cpp
  sdk/collectionservice.h sdk/collectionservice.cpp
  sdk/fileservice.h sdk/fileservice.cpp
  sdk/realtimeservice.h sdk/realtimeservice.cpp
  sdk/settingsservice.h sdk/settingsservice.cpp
  sdk/logservice.h sdk/logservice.cpp
  sdk/healthservice.h sdk/healthservice.cpp
  sdk/backupservice.h sdk/backupservice.cpp
  sdk/recordservice.h sdk/recordservice.cpp
  sdk/localauthstore.h sdk/localauthstore.cpp
  sdk/utils/basecrudservice.h sdk/utils/basecrudservice.cpp
  sdk/models/utils/basemodel.h sdk/models/utils/basemodel.cpp
  sdk/models/utils/listresult.h sdk/models/utils/listresult.cpp
  sdk/models/utils/schemafield.h sdk/models/utils/schemafield.cpp
  sdk/utils/baseservice.hpp
  sdk/utils/clientresponseerror.h
  sdk/utils/crudservice.h sdk/utils/crudservice.cpp
  sdk/utils/sseclient.h sdk/utils/sseclient.cpp
  sdk/models/AdminModel.h
  sdk/models/backupmodel.hpp
  sdk/models/collectionmodel.hpp
  sdk/models/fileuploadmodel.hpp
)
target_link_libraries(pocketbase-cpp Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)

include(GNUInstallDirs)
install(TARGETS pocketbase-cpp
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
