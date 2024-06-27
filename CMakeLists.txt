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
  sdk/globals.h
  sdk/client.h sdk/client.cpp
  sdk/services/recordservice.h sdk/services/recordservice.cpp
  sdk/services/basecrudservice.h sdk/services/basecrudservice.cpp
  sdk/services/crudservice.h sdk/services/crudservice.cpp
  sdk/services/baseservice.h sdk/services/clientresponseerror.h
  sdk/services/baseauthstore.h sdk/services/baseauthstore.cpp
  sdk/services/collectionservice.h sdk/services/collectionservice.cpp
  sdk/services/adminservice.h sdk/services/adminservice.cpp
  sdk/dtos/basemodel.h sdk/dtos/basemodel.cpp
  sdk/dtos/listresult.h sdk/dtos/listresult.cpp
  sdk/dtos/recordmodel.h sdk/dtos/collectionmodel.h
  sdk/dtos/AdminModel.h

  # sdk/localauthstore.h sdk/localauthstore.cpp
  # sdk/models/utils/schemafield.h sdk/models/utils/schemafield.cpp
)
target_link_libraries(pocketbase-cpp Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)

include(GNUInstallDirs)
install(TARGETS pocketbase-cpp
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


# sdk/utils/sseclient.h sdk/utils/sseclient.cpp
# sdk/fileservice.h sdk/fileservice.cpp
# sdk/realtimeservice.h sdk/realtimeservice.cpp
# sdk/settingsservice.h sdk/settingsservice.cpp
# sdk/logservice.h sdk/logservice.cpp
# sdk/healthservice.h sdk/healthservice.cpp
# sdk/backupservice.h sdk/backupservice.cpp
# sdk/models/backupmodel.hpp
# sdk/models/fileuploadmodel.hpp
  # sdk/utils/eventloop.h sdk/utils/eventloop.cpp
  # sdk/event.h sdk/event.cpp
