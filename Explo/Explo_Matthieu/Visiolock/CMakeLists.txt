cmake_minimum_required(VERSION 3.21.1)

option(LINK_INSIGHT "Link Qt Insight Tracker library" ON)
option(BUILD_QDS_COMPONENTS "Build design studio components" ON)

project(VisiolockApp LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Qml Quick)



if (Qt6_VERSION VERSION_GREATER_EQUAL 6.3)
    qt_standard_project_setup()
endif()

qt_add_executable(VisiolockApp
    src/main.cpp
    src/GuiController.h
    src/gui/gui.o
    src/ai/ai.o
    src/manager/manager.o
    src/guard/guard.o
    src/brain/brain.o
    src/archivist/archivist.o
    src/doorman/doorman.o
    src/rfid/rfid.o
    src/guard/sha256.o
    src/GuiModel.h src/GuiModel.cpp
)

qt_add_resources(VisiolockApp "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wl,-rpath-link,/home/pfe/qt6/pi/lib/${CMAKE_LIBRARY_ARCHITECTURE}" -L"/home/pfe/qt6/pi/lib/${CMAKE_LIBRARY_ARCHITECTURE}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wl,-rpath-link,/home/pfe/qt6/pi/lib/${CMAKE_LIBRARY_ARCHITECTURE}")

target_link_libraries(VisiolockApp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
)

target_link_libraries(VisiolockApp PRIVATE -lsqlite3)

if (BUILD_QDS_COMPONENTS)
    include(${CMAKE_CURRENT_SOURCE_DIR}/qmlcomponents)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/qmlmodules)

if (LINK_INSIGHT)
    include(${CMAKE_CURRENT_SOURCE_DIR}/insight)
endif ()

include(GNUInstallDirs)
install(TARGETS VisiolockApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# make IDEs aware of the QML import path
set(QML_IMPORT_PATH ${PROJECT_BINARY_DIR}/qml CACHE PATH
    "Path to the custom QML components defined by the project")
