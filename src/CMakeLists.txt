set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

add_library(pocketbaselib STATIC
  ../include/pocketbase/client.h client.cpp
  ../include/pocketbase/globals.h
  ../include/pocketbase/services/recordservice.h services/recordservice.cpp
  ../include/pocketbase/services/basecrudservice.h services/basecrudservice.cpp
  ../include/pocketbase/services/crudservice.h services/crudservice.cpp
  ../include/pocketbase/services/baseservice.h ../include/pocketbase/services/clientresponseerror.h
  ../include/pocketbase/services/baseauthstore.h services/baseauthstore.cpp
  ../include/pocketbase/services/collectionservice.h services/collectionservice.cpp
  ../include/pocketbase/services/adminservice.h services/adminservice.cpp
  ../include/pocketbase/services/healthservice.h services/healthservice.cpp
  ../include/pocketbase/services/settingsservice.h services/settingsservice.cpp
  ../include/pocketbase/services/logservice.h services/logservice.cpp
  ../include/pocketbase/services/realtimeservice.h services/realtimeservice.cpp
  ../include/pocketbase/services/sseclient.h services/sseclient.cpp
  ../include/pocketbase/services/event.h services/event.cpp
  ../include/pocketbase/services/fileservice.h services/fileservice.cpp
  ../include/pocketbase/models/basemodel.h models/basemodel.cpp
  ../include/pocketbase/models/listresult.h models/listresult.cpp
  ../include/pocketbase/models/recordmodel.h ../include/pocketbase/models/collectionmodel.h
  ../include/pocketbase/models/AdminModel.h ../include/pocketbase/models/logsmodel.h
)

target_link_libraries(pocketbaselib Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)

target_include_directories(pocketbaselib PRIVATE ../include)

include(GNUInstallDirs)
install(TARGETS pocketbaselib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)



# localauthstore.h localauthstore.cpp
# models/schemafield.h models/schemafield.cpp
# backupservice.h backupservice.cpp
# models/backupmodel.hpp
# models/fileuploadmodel.hpp
