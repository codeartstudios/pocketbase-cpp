set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

add_library(pocketbaselib STATIC
  client.h client.cpp globals.h
  services/recordservice.h services/recordservice.cpp
  services/basecrudservice.h services/basecrudservice.cpp
  services/crudservice.h services/crudservice.cpp
  services/baseservice.h services/clientresponseerror.h
  services/baseauthstore.h services/baseauthstore.cpp
  services/collectionservice.h services/collectionservice.cpp
  services/adminservice.h services/adminservice.cpp
  services/healthservice.h services/healthservice.cpp
  services/settingsservice.h services/settingsservice.cpp
  services/logservice.h services/logservice.cpp
  services/realtimeservice.h services/realtimeservice.cpp
  services/sseclient.h services/sseclient.cpp
  services/event.h services/event.cpp
  models/basemodel.h models/basemodel.cpp
  models/listresult.h models/listresult.cpp
  models/recordmodel.h models/collectionmodel.h
  models/AdminModel.h models/logsmodel.h
)

target_link_libraries(pocketbaselib Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)

target_include_directories(pocketbaselib PRIVATE ../src)

include(GNUInstallDirs)
install(TARGETS pocketbaselib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)



# localauthstore.h localauthstore.cpp
# models/schemafield.h models/schemafield.cpp
# fileservice.h fileservice.cpp
# backupservice.h backupservice.cpp
# models/backupmodel.hpp
# models/fileuploadmodel.hpp
